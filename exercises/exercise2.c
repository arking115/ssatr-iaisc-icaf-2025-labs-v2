/*
 * Exercise 2: Scheduling Policy Comparison
 *
 * This exercise demonstrates the differences between Linux scheduling policies
 * by running identical tasks under SCHED_OTHER, SCHED_FIFO, and SCHED_RR.
 *
 * Concepts:
 * - SCHED_OTHER: Default time-sharing (CFS - Completely Fair Scheduler)
 * - SCHED_FIFO: Real-time FIFO (runs until yields/blocks)
 * - SCHED_RR: Real-time Round-Robin (time-sliced RT)
 * - Priority-based scheduling
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define WORK_ITERATIONS 100000000  // Computational work amount
#define NUM_THREADS 3

// Thread information structure
typedef struct {
    int thread_id;
    const char *policy_name;
    int policy;
    int priority;
    struct timespec start_time;
    struct timespec end_time;
    long duration_ns;
} thread_info_t;

thread_info_t threads[NUM_THREADS];

// Calculate time difference in nanoseconds
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * 1000000000L) + diff_nsec;
}

// Simulate computational work
void do_work(void) {
    volatile long sum = 0;
    for (long i = 0; i < WORK_ITERATIONS; i++) {
        sum += i;
    }
}

// Thread function
void* thread_function(void *arg) {
    thread_info_t *info = (thread_info_t *)arg;

    // Set scheduling policy
    struct sched_param param;
    param.sched_priority = info->priority;

    if (sched_setscheduler(0, info->policy, &param) != 0) {
        // If setting RT policy fails (need privileges), continue anyway
        if (info->policy != SCHED_OTHER) {
            printf("Warning: Thread %d - Could not set %s policy (need --privileged). Using SCHED_OTHER.\n",
                   info->thread_id, info->policy_name);
        }
    }

    // Record start time
    clock_gettime(CLOCK_MONOTONIC, &info->start_time);

    printf("Thread %d (%s, priority %d) started\n",
           info->thread_id, info->policy_name, info->priority);

    // Perform work
    do_work();

    // Record end time
    clock_gettime(CLOCK_MONOTONIC, &info->end_time);

    // Calculate duration
    info->duration_ns = timespec_diff_ns(&info->start_time, &info->end_time);

    printf("Thread %d (%s) completed in %.3f ms\n",
           info->thread_id, info->policy_name, info->duration_ns / 1000000.0);

    return NULL;
}

int main(void) {
    pthread_t thread_handles[NUM_THREADS];
    struct timespec program_start, program_end;

    printf("=== Exercise 2: Scheduling Policy Comparison ===\n\n");

    // Check if we can use RT scheduling
    int max_rt_prio = sched_get_priority_max(SCHED_FIFO);
    if (max_rt_prio < 0) {
        printf("Warning: RT scheduling not available\n");
        printf("Run container with: docker-compose run --rm --privileged rt-lab\n\n");
    } else {
        printf("RT scheduling available: max priority = %d\n\n", max_rt_prio);
    }

    // Configure threads
    threads[0] = (thread_info_t){
        .thread_id = 0,
        .policy_name = "SCHED_OTHER",
        .policy = SCHED_OTHER,
        .priority = 0
    };

    threads[1] = (thread_info_t){
        .thread_id = 1,
        .policy_name = "SCHED_FIFO",
        .policy = SCHED_FIFO,
        .priority = 98
    };

    threads[2] = (thread_info_t){
        .thread_id = 2,
        .policy_name = "SCHED_RR",
        .policy = SCHED_RR,
        .priority = 98
    };

    printf("Starting threads...\n\n");

    clock_gettime(CLOCK_MONOTONIC, &program_start);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&thread_handles[i], NULL, thread_function, &threads[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &program_end);

    // Display results
    printf("\n=== Results Summary ===\n\n");

    // Sort threads by completion time
    thread_info_t sorted[NUM_THREADS];
    memcpy(sorted, threads, sizeof(threads));

    for (int i = 0; i < NUM_THREADS - 1; i++) {
        for (int j = 0; j < NUM_THREADS - i - 1; j++) {
            if (sorted[j].duration_ns > sorted[j + 1].duration_ns) {
                thread_info_t temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    printf("Execution order (by completion time):\n");
    printf("Rank | Thread | Policy       | Priority | Duration (ms)\n");
    printf("-----|--------|--------------|----------|-------------\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("  %d  |   %d    | %-12s |    %2d    |   %8.3f\n",
               i + 1,
               sorted[i].thread_id,
               sorted[i].policy_name,
               sorted[i].priority,
               sorted[i].duration_ns / 1000000.0);
    }

    long total_time = timespec_diff_ns(&program_start, &program_end);
    printf("\nTotal execution time: %.3f ms\n", total_time / 1000000.0);

    // Analysis
    printf("\n=== Analysis ===\n");

    // Find RT and normal threads
    long rt_time = 0;
    long normal_time = 0;
    int rt_count = 0;
    int normal_count = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        if (threads[i].policy == SCHED_FIFO || threads[i].policy == SCHED_RR) {
            rt_time += threads[i].duration_ns;
            rt_count++;
        } else {
            normal_time += threads[i].duration_ns;
            normal_count++;
        }
    }

    if (rt_count > 0 && normal_count > 0) {
        double rt_avg = (rt_time / rt_count) / 1000000.0;
        double normal_avg = (normal_time / normal_count) / 1000000.0;

        printf("Average RT thread time:     %.3f ms\n", rt_avg);
        printf("Average NORMAL thread time: %.3f ms\n", normal_avg);

        if (rt_avg < normal_avg) {
            double speedup = (normal_avg / rt_avg - 1.0) * 100.0;
            printf("✓ RT threads completed %.1f%% faster\n", speedup);
        } else {
            printf("⚠ No RT advantage detected (container may lack RT privileges)\n");
        }
    }

    printf("\n=== Key Takeaways ===\n");
    printf("• SCHED_FIFO/RR threads get priority over SCHED_OTHER\n");
    printf("• RT scheduling ensures important tasks run first\n");
    printf("• FIFO runs until it yields; RR adds time-slicing\n");
    printf("• Same priority RT tasks: FIFO ≈ RR (in this test)\n");

    printf("\n=== Experiments to Try ===\n");
    printf("1. Run under load: stress-ng --cpu 4 & ./exercise2\n");
    printf("2. Modify priorities in source and recompile\n");
    printf("3. Check scheduling: ps -eLo pid,tid,class,rtprio,comm | grep exercise2\n");

    return 0;
}
