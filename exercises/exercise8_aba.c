/*
 * Exercise 8 ABA: Lock-Free Stack with ABA Problem Demonstration
 *
 * This exercise demonstrates the ABA problem in lock-free programming.
 * It shows what happens when concurrent push/pop operations occur and
 * how deferring memory reclamation prevents crashes (but causes memory leaks).
 *
 * Key Concepts:
 * - ABA problem in lock-free data structures
 * - Memory reclamation challenges
 * - Trade-off: defer free (leak) vs immediate free (crash)
 * - Why production code needs hazard pointers or epoch-based reclamation
 *
 * WARNING: This version has concurrent push/pop which can trigger ABA!
 *          We defer free() to avoid crashes, causing memory leaks.
 *          See exercise8.c for the "safe" phased version.
 *
 * Compile: gcc -O2 -pthread exercise8_aba.c -o exercise8_aba
 * Run: ./exercise8_aba
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
    _Atomic uint64_t leaked_nodes;  /* Track nodes we didn't free (memory leak) */
} LockFreeStack;

void lockfree_stack_init(LockFreeStack* stack) {
    atomic_store(&stack->head, NULL);
    atomic_store(&stack->cas_retries, 0);
    atomic_store(&stack->leaked_nodes, 0);
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

    /* DEFERRED MEMORY RECLAMATION:
     * We do NOT free the node here to avoid the ABA problem and crashes.
     * This causes a memory leak, but demonstrates why production lock-free
     * code needs proper memory reclamation strategies:
     * - Hazard pointers
     * - Epoch-based reclamation
     * - Reference counting
     * - RCU (Read-Copy-Update)
     *
     * Uncommenting the free() below will cause crashes due to ABA:
     */
     //free(old_head);  // COMMENTED OUT - would cause double-free!

    atomic_fetch_add(&stack->leaked_nodes, 1);  /* Track the leak */

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

/* Thread function for performance testing - CONCURRENT (triggers ABA!) */
void* performance_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    clock_gettime(CLOCK_MONOTONIC, &data->start_time);

    /* CONCURRENT push/pop operations
     * This WILL trigger the ABA problem! We defer free() to avoid crashes.
     * 50% push, 50% pop - this interleaving causes the ABA scenario:
     * - Thread 1 reads node A
     * - Thread 2 pops A, frees it
     * - malloc reuses A's memory for Thread 3's push
     * - Thread 1's CAS succeeds with "stale" A pointer
     * - Without deferred free, this causes double-free crashes!
     */
    for (int i = 0; i < data->ops_count; i++) {
        if (i % 2 == 0) {
            /* Push */
            if (data->is_lockfree) {
                lockfree_stack_push((LockFreeStack*)data->stack, i);
            } else {
                mutex_stack_push((MutexStack*)data->stack, i);
            }
        } else {
            /* Pop */
            int value;
            if (data->is_lockfree) {
                lockfree_stack_pop((LockFreeStack*)data->stack, &value);
            } else {
                mutex_stack_pop((MutexStack*)data->stack, &value);
            }
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
        uint64_t leaked = atomic_load(&lf_stack->leaked_nodes);
        printf("  CAS retries: %lu (%.1f%%)\n", retries,
               (retries * 100.0) / total_ops);
        printf("  Leaked nodes: %lu (deferred reclamation)\n", leaked);
        if (leaked > 0) {
            printf("  ⚠ Memory leak: %lu nodes not freed to avoid ABA crashes!\n", leaked);
        }
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
    printf("=== Exercise 8 ABA: Lock-Free Stack with ABA Problem ===\n");
    printf("\n⚠ WARNING: This exercise demonstrates the ABA problem!\n");
    printf("   Memory is deliberately leaked to avoid crashes.\n");
    printf("   See exercise8.c for the safe phased version.\n");
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
    printf("Part 2: Performance Testing (Concurrent - ABA Risk!)\n");
    printf("-----------------------------------------------------\n");
    printf("Scenario: %d threads, %d operations each\n", NUM_THREADS, OPS_PER_THREAD);
    printf("Note: Concurrent push/pop triggers ABA problem!\n");
    printf("      We defer free() to prevent crashes (causes memory leak)\n");

    test_performance(false);  /* Mutex-based */
    test_performance(true);   /* Lock-free */

    /* Summary */
    printf("\n");
    printf("=== Analysis ===\n");
    printf("\n");
    printf("⚠ ABA Problem Demonstrated:\n");
    printf("  • Concurrent push/pop causes ABA scenario\n");
    printf("  • Without deferred reclamation: CRASHES (double-free)\n");
    printf("  • With deferred reclamation: WORKS but LEAKS memory\n");
    printf("\n");
    printf("Production Solutions:\n");
    printf("  1. Hazard Pointers - mark nodes as \"in use\"\n");
    printf("  2. Epoch-Based Reclamation - defer free to safe epochs\n");
    printf("  3. Reference Counting - track active references\n");
    printf("  4. Tagged Pointers - include version counter with pointer\n");
    printf("  5. RCU (Read-Copy-Update) - Linux kernel approach\n");
    printf("\n");
    printf("Key Takeaway:\n");
    printf("  Lock-free data structures need careful memory management!\n");
    printf("  Simple malloc/free is NOT safe without proper reclamation.\n");

    return 0;
}
