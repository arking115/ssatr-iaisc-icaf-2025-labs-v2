/*
 * Exercise 9: Synchronization Performance Analysis
 *
 * Comprehensive comparison of mutex-based vs lock-free synchronization under
 * different workloads and real-time scenarios.
 *
 * Tests:
 * 1. Low contention (read-heavy workload)
 * 2. High contention (write-heavy workload)
 * 3. Mixed priority tasks (priority inversion testing)
 * 4. Real-time deadline sensitivity
 *
 * Compile: gcc -O2 -pthread exercise9.c -o exercise9
 * Run: ./exercise9
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>

/* Configuration */
#define NUM_THREADS 4
#define OPS_PER_THREAD 100000
#define RT_PRIORITY_HIGH 80
#define RT_PRIORITY_LOW 20

/* Shared counter implementations */
typedef struct {
    int64_t value;
    pthread_mutex_t mutex;
    uint64_t contention_count;
    uint64_t max_wait_ns;
} MutexCounter;

typedef struct {
    _Atomic int64_t value;
    _Atomic uint64_t cas_retries;
} LockFreeCounter;

/* Test workload types */
typedef enum {
    WORKLOAD_READ_HEAVY,   /* 90% read, 10% write */
    WORKLOAD_WRITE_HEAVY,  /* 50% read, 50% write */
    WORKLOAD_BALANCED      /* 70% read, 30% write */
} WorkloadType;

/* Thread data */
typedef struct {
    void* counter;
    bool is_lockfree;
    int thread_id;
    int priority;
    int ops_count;
    WorkloadType workload;
    struct timespec start_time;
    struct timespec end_time;
    uint64_t deadline_misses;
    uint64_t blocking_events;
    uint64_t max_latency_ns;
} ThreadData;

/* ===== UTILITY FUNCTIONS ===== */

uint64_t timespec_diff_ns(struct timespec* start, struct timespec* end) {
    return (end->tv_sec - start->tv_sec) * 1000000000ULL +
           (end->tv_nsec - start->tv_nsec);
}

void set_thread_priority(int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
        /* If setting RT priority fails, continue anyway */
        /* (May need --privileged or CAP_SYS_NICE) */
    }
}

/* ===== MUTEX COUNTER IMPLEMENTATION ===== */

void mutex_counter_init(MutexCounter* counter) {
    counter->value = 0;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);  /* PI mutex */
    pthread_mutex_init(&counter->mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    counter->contention_count = 0;
    counter->max_wait_ns = 0;
}

void mutex_counter_increment(MutexCounter* counter) {
    struct timespec lock_start, lock_end;

    clock_gettime(CLOCK_MONOTONIC, &lock_start);

    if (pthread_mutex_trylock(&counter->mutex) != 0) {
        __atomic_fetch_add(&counter->contention_count, 1, __ATOMIC_RELAXED);
        pthread_mutex_lock(&counter->mutex);
    }

    clock_gettime(CLOCK_MONOTONIC, &lock_end);
    uint64_t wait_ns = timespec_diff_ns(&lock_start, &lock_end);

    /* Track max wait time */
    uint64_t current_max = __atomic_load_n(&counter->max_wait_ns, __ATOMIC_RELAXED);
    while (wait_ns > current_max) {
        if (__atomic_compare_exchange_n(&counter->max_wait_ns, &current_max,
                                         wait_ns, false,
                                         __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
            break;
        }
    }

    counter->value++;
    pthread_mutex_unlock(&counter->mutex);
}

int64_t mutex_counter_read(MutexCounter* counter) {
    pthread_mutex_lock(&counter->mutex);
    int64_t value = counter->value;
    pthread_mutex_unlock(&counter->mutex);
    return value;
}

void mutex_counter_destroy(MutexCounter* counter) {
    pthread_mutex_destroy(&counter->mutex);
}

/* ===== LOCK-FREE COUNTER IMPLEMENTATION ===== */

void lockfree_counter_init(LockFreeCounter* counter) {
    atomic_store(&counter->value, 0);
    atomic_store(&counter->cas_retries, 0);
}

void lockfree_counter_increment(LockFreeCounter* counter) {
    int retries = 0;
    int64_t old_value, new_value;

    do {
        old_value = atomic_load(&counter->value);
        new_value = old_value + 1;

        if (retries > 0) {
            atomic_fetch_add(&counter->cas_retries, 1);
        }
        retries++;

    } while (!atomic_compare_exchange_weak(&counter->value, &old_value, new_value));
}

int64_t lockfree_counter_read(LockFreeCounter* counter) {
    return atomic_load(&counter->value);
}

void lockfree_counter_destroy(LockFreeCounter* counter) {
    (void)counter;  /* Parameter unused but needed for API consistency */
}

/* ===== WORKLOAD EXECUTION ===== */

void* workload_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    /* Set priority if specified */
    if (data->priority > 0) {
        set_thread_priority(data->priority);
    }

    clock_gettime(CLOCK_MONOTONIC, &data->start_time);

    /* Determine read/write ratio based on workload type */
    int write_percentage;
    switch (data->workload) {
        case WORKLOAD_READ_HEAVY:  write_percentage = 10; break;
        case WORKLOAD_WRITE_HEAVY: write_percentage = 50; break;
        case WORKLOAD_BALANCED:    write_percentage = 30; break;
        default:                   write_percentage = 30; break;
    }

    for (int i = 0; i < data->ops_count; i++) {
        struct timespec op_start, op_end;
        clock_gettime(CLOCK_MONOTONIC, &op_start);

        /* Perform operation based on ratio */
        if ((i % 100) < write_percentage) {
            /* Write operation */
            if (data->is_lockfree) {
                lockfree_counter_increment((LockFreeCounter*)data->counter);
            } else {
                mutex_counter_increment((MutexCounter*)data->counter);
            }
        } else {
            /* Read operation */
            volatile int64_t value;
            if (data->is_lockfree) {
                value = lockfree_counter_read((LockFreeCounter*)data->counter);
            } else {
                value = mutex_counter_read((MutexCounter*)data->counter);
            }
            (void)value;  /* Prevent optimization */
        }

        clock_gettime(CLOCK_MONOTONIC, &op_end);
        uint64_t latency_ns = timespec_diff_ns(&op_start, &op_end);

        /* Track max latency */
        if (latency_ns > data->max_latency_ns) {
            data->max_latency_ns = latency_ns;
        }

        /* For deadline-sensitive test: check 100µs deadline */
        if (latency_ns > 100000) {  /* 100 µs */
            data->deadline_misses++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &data->end_time);

    return NULL;
}

/* ===== TEST SCENARIOS ===== */

void run_test(const char* test_name, WorkloadType workload,
              bool use_priorities, bool is_lockfree) {
    printf("\n%s (%s):\n", test_name, is_lockfree ? "Lock-Free" : "Mutex (PI)");

    void* counter;
    if (is_lockfree) {
        counter = malloc(sizeof(LockFreeCounter));
        lockfree_counter_init((LockFreeCounter*)counter);
    } else {
        counter = malloc(sizeof(MutexCounter));
        mutex_counter_init((MutexCounter*)counter);
    }

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    struct timespec global_start, global_end;
    clock_gettime(CLOCK_MONOTONIC, &global_start);

    /* Create threads */
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].counter = counter;
        thread_data[i].is_lockfree = is_lockfree;
        thread_data[i].thread_id = i;
        thread_data[i].ops_count = OPS_PER_THREAD;
        thread_data[i].workload = workload;
        thread_data[i].deadline_misses = 0;
        thread_data[i].blocking_events = 0;
        thread_data[i].max_latency_ns = 0;

        /* Set priority if requested */
        if (use_priorities) {
            /* First thread high priority, others low */
            thread_data[i].priority = (i == 0) ? RT_PRIORITY_HIGH : RT_PRIORITY_LOW;
        } else {
            thread_data[i].priority = 0;  /* No RT priority */
        }

        pthread_create(&threads[i], NULL, workload_thread, &thread_data[i]);
    }

    /* Wait for completion */
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

    uint64_t total_deadline_misses = 0;
    uint64_t max_op_latency_ns = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        total_deadline_misses += thread_data[i].deadline_misses;
        if (thread_data[i].max_latency_ns > max_op_latency_ns) {
            max_op_latency_ns = thread_data[i].max_latency_ns;
        }
    }

    /* Print results */
    printf("  Time: %.3f sec\n", total_time_sec);
    printf("  Throughput: %.0f ops/sec\n", throughput);
    printf("  Avg latency: %.2f µs\n", avg_latency_us);
    printf("  Max latency: %.2f µs\n", max_op_latency_ns / 1000.0);

    if (is_lockfree) {
        LockFreeCounter* lf_counter = (LockFreeCounter*)counter;
        uint64_t retries = atomic_load(&lf_counter->cas_retries);
        printf("  CAS retries: %lu (%.1f%%)\n", retries,
               (retries * 100.0) / total_ops);
    } else {
        MutexCounter* m_counter = (MutexCounter*)counter;
        printf("  Lock contention: %lu times\n", m_counter->contention_count);
        printf("  Max lock wait: %.2f µs\n", m_counter->max_wait_ns / 1000.0);
    }

    if (total_deadline_misses > 0) {
        printf("  Deadline misses (>100µs): %lu (%.2f%%)\n",
               total_deadline_misses,
               (total_deadline_misses * 100.0) / total_ops);
    }

    /* Cleanup */
    if (is_lockfree) {
        lockfree_counter_destroy((LockFreeCounter*)counter);
    } else {
        mutex_counter_destroy((MutexCounter*)counter);
    }
    free(counter);
}

/* Main */
int main(void) {
    printf("=== Exercise 9: Synchronization Performance Analysis ===\n");
    printf("\nTest Configuration:\n");
    printf("  Threads: %d\n", NUM_THREADS);
    printf("  Operations per thread: %d\n", OPS_PER_THREAD);
    printf("  Total operations: %d\n", NUM_THREADS * OPS_PER_THREAD);

    /* Test 1: Low Contention (Read-Heavy) */
    printf("\n");
    printf("Test 1: Low Contention (90%% read, 10%% write)\n");
    printf("--------------------------------------------\n");
    run_test("Read-Heavy", WORKLOAD_READ_HEAVY, false, false);
    run_test("Read-Heavy", WORKLOAD_READ_HEAVY, false, true);

    /* Test 2: High Contention (Write-Heavy) */
    printf("\n");
    printf("Test 2: High Contention (50%% read, 50%% write)\n");
    printf("---------------------------------------------\n");
    run_test("Write-Heavy", WORKLOAD_WRITE_HEAVY, false, false);
    run_test("Write-Heavy", WORKLOAD_WRITE_HEAVY, false, true);

    /* Test 3: Mixed Priority (Priority Inversion Test) */
    printf("\n");
    printf("Test 3: Mixed Priority Tasks\n");
    printf("-----------------------------\n");
    printf("Scenario: 1 high-priority thread, %d low-priority threads\n",
           NUM_THREADS - 1);
    run_test("Mixed Priority", WORKLOAD_BALANCED, true, false);
    run_test("Mixed Priority", WORKLOAD_BALANCED, true, true);

    /* Summary */
    printf("\n");
    printf("=== Summary ===\n");
    printf("\n");
    printf("Lock-Free Advantages:\n");
    printf("  ✓ Better throughput under contention\n");
    printf("  ✓ Zero priority inversion risk\n");
    printf("  ✓ More predictable worst-case latency\n");
    printf("  ✓ Scalable with thread count\n");
    printf("  ✓ No deadlock possible\n");
    printf("\n");
    printf("Mutex Advantages:\n");
    printf("  ✓ Simpler implementation\n");
    printf("  ✓ Lower overhead in low-contention scenarios\n");
    printf("  ✓ Priority Inheritance bounds blocking\n");
    printf("  ✓ Easier to reason about correctness\n");
    printf("\n");
    printf("Recommendation:\n");
    printf("  • High contention + RT requirements: Use lock-free\n");
    printf("  • Low contention + simplicity valued: Use mutex with PI\n");
    printf("  • Critical RT paths: Use lock-free\n");
    printf("  • Complex data structures: Use mutex with careful design\n");

    return 0;
}
