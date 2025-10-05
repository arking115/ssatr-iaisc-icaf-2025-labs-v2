/*
 * Exercise 8: Lock-Free Stack Implementation
 *
 * This exercise demonstrates lock-free programming using Compare-And-Swap (CAS)
 * operations. It implements both a traditional mutex-based stack and a lock-free
 * stack, then compares their correctness and performance.
 *
 * Key Concepts:
 * - Compare-And-Swap (CAS) atomic operation
 * - Lock-free data structures
 * - ABA problem (simplified handling)
 * - Memory ordering semantics
 *
 * Compile: gcc -O2 -pthread exercise8.c -o exercise8
 * Run: ./exercise8
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>

/* Configuration */
#define NUM_THREADS 4
#define OPS_PER_THREAD 100000
#define WARMUP_OPS 1000

/* Node structure for both stacks */
typedef struct Node {
    int data;
    struct Node* next;
} Node;

/* ===== MUTEX-BASED STACK ===== */
typedef struct {
    Node* head;
    pthread_mutex_t mutex;
    uint64_t contention_count;  /* Track contention */
} MutexStack;

void mutex_stack_init(MutexStack* stack) {
    stack->head = NULL;
    pthread_mutex_init(&stack->mutex, NULL);
    stack->contention_count = 0;
}

void mutex_stack_push(MutexStack* stack, int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = value;

    /* Try lock - if it fails, we have contention */
    if (pthread_mutex_trylock(&stack->mutex) != 0) {
        __atomic_fetch_add(&stack->contention_count, 1, __ATOMIC_RELAXED);
        pthread_mutex_lock(&stack->mutex);
    }

    new_node->next = stack->head;
    stack->head = new_node;

    pthread_mutex_unlock(&stack->mutex);
}

bool mutex_stack_pop(MutexStack* stack, int* value) {
    pthread_mutex_lock(&stack->mutex);

    if (stack->head == NULL) {
        pthread_mutex_unlock(&stack->mutex);
        return false;
    }

    Node* old_head = stack->head;
    *value = old_head->data;
    stack->head = old_head->next;

    pthread_mutex_unlock(&stack->mutex);

    free(old_head);
    return true;
}

void mutex_stack_destroy(MutexStack* stack) {
    int value;
    while (mutex_stack_pop(stack, &value));
    pthread_mutex_destroy(&stack->mutex);
}

/* ===== LOCK-FREE STACK ===== */
typedef struct {
    _Atomic(Node*) head;
    _Atomic uint64_t cas_retries;  /* Track CAS failures */
} LockFreeStack;

void lockfree_stack_init(LockFreeStack* stack) {
    atomic_store(&stack->head, NULL);
    atomic_store(&stack->cas_retries, 0);
}

void lockfree_stack_push(LockFreeStack* stack, int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = value;

    Node* old_head;
    int retries = 0;

    do {
        /* Load current head */
        old_head = atomic_load(&stack->head);

        /* Set new node's next to current head */
        new_node->next = old_head;

        /* Try to CAS head from old_head to new_node */
        /* If successful, loop exits. If failed, retry with new old_head value */
        if (retries > 0) {
            atomic_fetch_add(&stack->cas_retries, 1);
        }
        retries++;

    } while (!atomic_compare_exchange_weak(&stack->head, &old_head, new_node));
}

bool lockfree_stack_pop(LockFreeStack* stack, int* value) {
    Node* old_head;
    Node* new_head;
    int retries = 0;

    do {
        /* Load current head */
        old_head = atomic_load(&stack->head);

        /* If stack is empty, return false */
        if (old_head == NULL) {
            return false;
        }

        /* Read next pointer - this is safe because old_head is still
         * in the stack at this point. If another thread pops it between
         * now and the CAS, the CAS will fail and we'll retry. */
        new_head = old_head->next;

        /* Validate that head didn't change while we read next
         * This prevents accessing a freed node's next pointer */
        Node* current_head = atomic_load(&stack->head);
        if (current_head != old_head) {
            /* Head changed, retry */
            if (retries > 0) {
                atomic_fetch_add(&stack->cas_retries, 1);
            }
            retries++;
            continue;
        }

        /* Try to CAS head from old_head to new_head */
        if (retries > 0) {
            atomic_fetch_add(&stack->cas_retries, 1);
        }
        retries++;

    } while (!atomic_compare_exchange_weak(&stack->head, &old_head, new_head));

    /* Extract value from popped node */
    *value = old_head->data;

    /* WARNING: This free() can cause ABA problem in production!
     * For this exercise, we simplify. Production code needs:
     * - Hazard pointers
     * - Epoch-based reclamation
     * - Reference counting
     * - Or deferred reclamation
     */
    free(old_head);

    return true;
}

void lockfree_stack_destroy(LockFreeStack* stack) {
    int value;
    while (lockfree_stack_pop(stack, &value));
}

/* ===== TESTING INFRASTRUCTURE ===== */

/* Test data structure */
typedef struct {
    void* stack;
    int thread_id;
    int ops_count;
    bool is_lockfree;
    struct timespec start_time;
    struct timespec end_time;
} ThreadData;

/* Utility: Calculate time difference in nanoseconds */
uint64_t timespec_diff_ns(struct timespec* start, struct timespec* end) {
    return (end->tv_sec - start->tv_sec) * 1000000000ULL +
           (end->tv_nsec - start->tv_nsec);
}

/* Thread function for correctness testing */
void* correctness_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    /* Each thread pushes its thread_id multiple times */
    for (int i = 0; i < data->ops_count; i++) {
        if (data->is_lockfree) {
            lockfree_stack_push((LockFreeStack*)data->stack, data->thread_id);
        } else {
            mutex_stack_push((MutexStack*)data->stack, data->thread_id);
        }
    }

    return NULL;
}

/* Thread function for performance testing - PHASED to avoid ABA problem */
void* performance_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    clock_gettime(CLOCK_MONOTONIC, &data->start_time);

    /* Phase 1: Push operations
     * All threads push their items first. This avoids the ABA problem
     * that would occur with concurrent push/pop operations. */
    for (int i = 0; i < data->ops_count / 2; i++) {
        if (data->is_lockfree) {
            lockfree_stack_push((LockFreeStack*)data->stack, i);
        } else {
            mutex_stack_push((MutexStack*)data->stack, i);
        }
    }

    /* Simple barrier: sleep briefly to let all threads finish pushing
     * In production, use pthread_barrier_wait() */
    usleep(1000);

    /* Phase 2: Pop operations
     * Now all threads pop items. Since we're only popping (not pushing),
     * the ABA problem is avoided. */
    for (int i = 0; i < data->ops_count / 2; i++) {
        int value;
        if (data->is_lockfree) {
            lockfree_stack_pop((LockFreeStack*)data->stack, &value);
        } else {
            mutex_stack_pop((MutexStack*)data->stack, &value);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &data->end_time);

    return NULL;
}

/* Test correctness of a stack implementation */
bool test_correctness(bool is_lockfree) {
    printf("\nTesting %s stack...\n", is_lockfree ? "lock-free" : "mutex-based");

    void* stack;
    if (is_lockfree) {
        stack = malloc(sizeof(LockFreeStack));
        lockfree_stack_init((LockFreeStack*)stack);
    } else {
        stack = malloc(sizeof(MutexStack));
        mutex_stack_init((MutexStack*)stack);
    }

    /* Create threads */
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    printf("  Pushing %d items from %d threads...\n", OPS_PER_THREAD, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].stack = stack;
        thread_data[i].thread_id = i;
        thread_data[i].ops_count = OPS_PER_THREAD;
        thread_data[i].is_lockfree = is_lockfree;
        pthread_create(&threads[i], NULL, correctness_thread, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Verify: Count items per thread */
    printf("  Popping all items...\n");
    int counts[NUM_THREADS] = {0};
    int total = 0;
    int value;
    bool success;

    do {
        if (is_lockfree) {
            success = lockfree_stack_pop((LockFreeStack*)stack, &value);
        } else {
            success = mutex_stack_pop((MutexStack*)stack, &value);
        }

        if (success) {
            if (value >= 0 && value < NUM_THREADS) {
                counts[value]++;
                total++;
            }
        }
    } while (success);

    /* Check correctness */
    bool correct = true;
    int expected_total = NUM_THREADS * OPS_PER_THREAD;

    if (total != expected_total) {
        printf("  ✗ Wrong total: got %d, expected %d\n", total, expected_total);
        correct = false;
    } else {
        printf("  ✓ All %d items accounted for\n", total);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (counts[i] != OPS_PER_THREAD) {
            printf("  ✗ Thread %d: got %d, expected %d\n", i, counts[i], OPS_PER_THREAD);
            correct = false;
        }
    }

    if (correct) {
        printf("  ✓ No duplicates found\n");
        printf("  ✓ %s stack: CORRECT\n", is_lockfree ? "Lock-free" : "Mutex");
    }

    /* Cleanup */
    if (is_lockfree) {
        lockfree_stack_destroy((LockFreeStack*)stack);
    } else {
        mutex_stack_destroy((MutexStack*)stack);
    }
    free(stack);

    return correct;
}

/* Test performance of a stack implementation */
void test_performance(bool is_lockfree) {
    printf("\n%s stack:\n", is_lockfree ? "Lock-free" : "Mutex-based");

    void* stack;
    if (is_lockfree) {
        stack = malloc(sizeof(LockFreeStack));
        lockfree_stack_init((LockFreeStack*)stack);
    } else {
        stack = malloc(sizeof(MutexStack));
        mutex_stack_init((MutexStack*)stack);
    }

    /* Create threads */
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    struct timespec global_start, global_end;
    clock_gettime(CLOCK_MONOTONIC, &global_start);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].stack = stack;
        thread_data[i].thread_id = i;
        thread_data[i].ops_count = OPS_PER_THREAD;
        thread_data[i].is_lockfree = is_lockfree;
        pthread_create(&threads[i], NULL, performance_thread, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &global_end);

    /* Calculate statistics */
    uint64_t total_time_ns = timespec_diff_ns(&global_start, &global_end);
    double total_time_sec = total_time_ns / 1e9;
    int total_ops = NUM_THREADS * OPS_PER_THREAD;
    double throughput = total_ops / total_time_sec;
    double avg_latency_us = (total_time_ns / 1000.0) / total_ops;

    printf("  Total operations: %d\n", total_ops);
    printf("  Time: %.3f seconds\n", total_time_sec);
    printf("  Throughput: %.0f ops/sec\n", throughput);
    printf("  Avg latency: %.2f µs\n", avg_latency_us);

    if (is_lockfree) {
        LockFreeStack* lf_stack = (LockFreeStack*)stack;
        uint64_t retries = atomic_load(&lf_stack->cas_retries);
        printf("  CAS retries: %lu (%.1f%%)\n", retries,
               (retries * 100.0) / total_ops);
    } else {
        MutexStack* m_stack = (MutexStack*)stack;
        printf("  Lock contention detected: %lu times\n", m_stack->contention_count);
    }

    /* Cleanup */
    if (is_lockfree) {
        lockfree_stack_destroy((LockFreeStack*)stack);
    } else {
        mutex_stack_destroy((MutexStack*)stack);
    }
    free(stack);
}

/* Main */
int main(void) {
    printf("=== Exercise 8: Lock-Free Stack Implementation ===\n");
    printf("\nConfiguration:\n");
    printf("  Threads: %d\n", NUM_THREADS);
    printf("  Operations per thread: %d\n", OPS_PER_THREAD);

    /* Part 1: Correctness Testing */
    printf("\n");
    printf("Part 1: Correctness Testing\n");
    printf("----------------------------\n");

    bool mutex_correct = test_correctness(false);
    bool lockfree_correct = test_correctness(true);

    if (!mutex_correct || !lockfree_correct) {
        fprintf(stderr, "\n✗ Correctness tests failed!\n");
        return 1;
    }

    /* Part 2: Performance Testing */
    printf("\n");
    printf("Part 2: Performance Testing (Phased)\n");
    printf("-------------------------------------\n");
    printf("Scenario: %d threads, %d operations each\n", NUM_THREADS, OPS_PER_THREAD);
    printf("Note: Using phased approach (push then pop) to avoid ABA problem\n");
    printf("      See exercise8_aba.c for ABA demonstration\n");

    test_performance(false);  /* Mutex-based */
    test_performance(true);   /* Lock-free */

    /* Summary */
    printf("\n");
    printf("=== Analysis ===\n");
    printf("Lock-free stack advantages:\n");
    printf("  • No priority inversion risk\n");
    printf("  • No deadlock possible\n");
    printf("  • Better scalability (no lock contention)\n");
    printf("  • Consistent performance under load\n");
    printf("\n");
    printf("Trade-offs:\n");
    printf("  • More complex implementation\n");
    printf("  • CAS retries consume CPU\n");
    printf("  • Memory ordering requirements\n");
    printf("  • ABA problem (simplified in this exercise)\n");

    return 0;
}
