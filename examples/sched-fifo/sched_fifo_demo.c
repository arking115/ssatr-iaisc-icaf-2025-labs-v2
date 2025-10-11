/*
 * SCHED_FIFO Scheduling Demonstration
 *
 * This example demonstrates the behavior of SCHED_FIFO (First-In-First-Out)
 * real-time scheduling policy in Linux.
 *
 * Key Concepts Demonstrated:
 * 1. SCHED_FIFO runs until the thread yields, blocks, or is preempted
 * 2. Higher priority RT threads preempt lower priority threads immediately
 * 3. No time slicing - thread monopolizes CPU until done
 * 4. Priority-based deterministic scheduling
 *
 * Learning Objectives:
 * - Understand SCHED_FIFO execution model
 * - See priority-based preemption in action
 * - Learn when to use SCHED_FIFO vs other policies
 * - Understand RT scheduling requirements and limitations
 *
 * Architecture:
 * Three threads with different priorities execute computational work:
 * - High Priority Thread (90): Runs first, preempts others
 * - Medium Priority Thread (70): Runs after high priority
 * - Low Priority Thread (50): Runs last
 *
 * Expected Behavior:
 * With SCHED_FIFO: Execution follows strict priority order (high → medium → low)
 * Without RT: Threads may interleave based on CFS scheduler decisions
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

#define WORK_ITERATIONS 150000000  // Amount of computational work
#define NUM_THREADS 3
#define NSEC_PER_SEC 1000000000L

// Thread information structure
typedef struct {
    int id;
    const char *name;
    int priority;
    int policy;
    const char *policy_name;
    struct timespec start_time;
    struct timespec end_time;
    struct timespec first_run_time;
    long duration_ns;
    int execution_order;
} thread_info_t;

thread_info_t threads[NUM_THREADS];
int next_execution_order = 1;
pthread_mutex_t order_mutex = PTHREAD_MUTEX_INITIALIZER;

// Calculate time difference in nanoseconds
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * NSEC_PER_SEC) + diff_nsec;
}

// Simulate computational work
void do_work(void) {
    volatile long sum = 0;
    for (long i = 0; i < WORK_ITERATIONS; i++) {
        sum += i * i;  // Make it slightly more compute-intensive
    }
}

// Thread function
void* thread_function(void *arg) {
    thread_info_t *info = (thread_info_t *)arg;

    // Set scheduling policy and priority
    struct sched_param param;
    param.sched_priority = info->priority;

    if (sched_setscheduler(0, info->policy, &param) != 0) {
        if (info->policy != SCHED_OTHER) {
            fprintf(stderr, "Warning: Thread %d (%s) - Could not set %s policy: %s\n",
                   info->id, info->name, info->policy_name, strerror(errno));
            fprintf(stderr, "         Continuing with SCHED_OTHER. Run with --privileged for RT.\n");
        }
    }

    // Record start time
    clock_gettime(CLOCK_MONOTONIC, &info->start_time);

    // Record execution order (first time thread actually runs)
    pthread_mutex_lock(&order_mutex);
    info->execution_order = next_execution_order++;
    clock_gettime(CLOCK_MONOTONIC, &info->first_run_time);
    pthread_mutex_unlock(&order_mutex);

    printf("[T%d] %s (priority %d) started executing\n",
           info->id, info->name, info->priority);

    // Perform computational work (no voluntary yields)
    do_work();

    // Record end time
    clock_gettime(CLOCK_MONOTONIC, &info->end_time);
    info->duration_ns = timespec_diff_ns(&info->start_time, &info->end_time);

    printf("[T%d] %s completed (duration: %.3f ms)\n",
           info->id, info->name, info->duration_ns / 1000000.0);

    return NULL;
}

// Display system RT capabilities
void display_rt_capabilities(void) {
    printf("=== System RT Scheduling Capabilities ===\n\n");

    int min_prio = sched_get_priority_min(SCHED_FIFO);
    int max_prio = sched_get_priority_max(SCHED_FIFO);

    if (min_prio < 0 || max_prio < 0) {
        printf("⚠ RT scheduling not available or not accessible\n");
        printf("  Error: %s\n", strerror(errno));
        printf("\nTo enable RT scheduling:\n");
        printf("  Docker: docker-compose run --rm --privileged rt-lab\n");
        printf("  Linux:  sudo ./sched_fifo_demo\n\n");
    } else {
        printf("✓ SCHED_FIFO available\n");
        printf("  Priority range: %d - %d\n", min_prio, max_prio);
        printf("  Using priorities: 50 (low), 70 (medium), 90 (high)\n\n");
    }
}

// Display results and analysis
void display_results(struct timespec *program_start) {
    printf("\n=== Execution Results ===\n\n");

    // Sort by execution order
    thread_info_t sorted[NUM_THREADS];
    memcpy(sorted, threads, sizeof(threads));

    for (int i = 0; i < NUM_THREADS - 1; i++) {
        for (int j = 0; j < NUM_THREADS - i - 1; j++) {
            if (sorted[j].execution_order > sorted[j + 1].execution_order) {
                thread_info_t temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    printf("Order | Thread | Priority | Policy       | Start Time | Duration\n");
    printf("------|--------|----------|--------------|------------|----------\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        long start_offset = timespec_diff_ns(program_start, &sorted[i].first_run_time);
        printf("  %d   |   %d    |    %2d    | %-12s | %8.3f ms | %7.3f ms\n",
               sorted[i].execution_order,
               sorted[i].id,
               sorted[i].priority,
               sorted[i].policy_name,
               start_offset / 1000000.0,
               sorted[i].duration_ns / 1000000.0);
    }

    printf("\n=== Analysis ===\n\n");

    // Check if execution followed priority order
    int correct_order = 1;
    for (int i = 0; i < NUM_THREADS - 1; i++) {
        if (sorted[i].priority < sorted[i + 1].priority) {
            correct_order = 0;
            break;
        }
    }

    if (correct_order && strcmp(sorted[0].policy_name, "SCHED_FIFO") == 0) {
        printf("✓ SCHED_FIFO Behavior Confirmed:\n");
        printf("  • Threads executed in strict priority order\n");
        printf("  • High priority (90) → Medium priority (70) → Low priority (50)\n");
        printf("  • Each thread ran to completion without preemption\n");
        printf("  • No time slicing - deterministic execution\n");
    } else if (strcmp(threads[0].policy_name, "SCHED_OTHER") == 0) {
        printf("⚠ Running with SCHED_OTHER (normal scheduling):\n");
        printf("  • Execution order may not follow priority\n");
        printf("  • CFS scheduler decides based on fairness, not RT priority\n");
        printf("  • Threads may be interleaved (time-sliced)\n");
        printf("  • Run with --privileged to see SCHED_FIFO behavior\n");
    } else {
        printf("⚠ Unexpected execution order:\n");
        printf("  • May indicate scheduling anomaly or system load\n");
        printf("  • RT scheduling requires low system load for deterministic behavior\n");
    }

    // Calculate time between thread starts (shows preemption)
    printf("\n=== Preemption Analysis ===\n\n");
    if (sorted[0].policy == SCHED_FIFO) {
        printf("Time gaps between thread starts:\n");
        for (int i = 1; i < NUM_THREADS; i++) {
            long gap = timespec_diff_ns(&sorted[i-1].first_run_time, &sorted[i].first_run_time);
            printf("  %s → %s: %.3f ms\n",
                   sorted[i-1].name, sorted[i].name, gap / 1000000.0);
        }
        printf("\nWith SCHED_FIFO:\n");
        printf("  • Small gaps indicate immediate context switch after completion\n");
        printf("  • Next highest priority thread runs immediately\n");
        printf("  • No waiting for scheduler time slice\n");
    } else {
        printf("Running under normal scheduling - preemption analysis not applicable\n");
    }
}

int main(int argc, char *argv[]) {
    pthread_t thread_handles[NUM_THREADS];
    struct timespec program_start;
    int use_rt = 1;  // Default to RT

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--normal") == 0) {
            use_rt = 0;
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [OPTIONS]\n", argv[0]);
            printf("\nDemonstrates SCHED_FIFO real-time scheduling behavior.\n\n");
            printf("Options:\n");
            printf("  --normal    Use SCHED_OTHER instead of SCHED_FIFO (for comparison)\n");
            printf("  --help      Show this help message\n\n");
            printf("Default: Attempts to use SCHED_FIFO (requires --privileged in Docker)\n");
            return 0;
        }
    }

    printf("=== SCHED_FIFO Scheduling Demonstration ===\n\n");

    if (use_rt) {
        printf("Mode: Real-Time Scheduling (SCHED_FIFO)\n");
        display_rt_capabilities();
    } else {
        printf("Mode: Normal Scheduling (SCHED_OTHER) - for comparison\n\n");
    }

    // Configure threads
    threads[0] = (thread_info_t){
        .id = 0,
        .name = "Low Priority",
        .priority = use_rt ? 50 : 0,
        .policy = use_rt ? SCHED_FIFO : SCHED_OTHER,
        .policy_name = use_rt ? "SCHED_FIFO" : "SCHED_OTHER"
    };

    threads[1] = (thread_info_t){
        .id = 1,
        .name = "Medium Priority",
        .priority = use_rt ? 70 : 0,
        .policy = use_rt ? SCHED_FIFO : SCHED_OTHER,
        .policy_name = use_rt ? "SCHED_FIFO" : "SCHED_OTHER"
    };

    threads[2] = (thread_info_t){
        .id = 2,
        .name = "High Priority",
        .priority = use_rt ? 90 : 0,
        .policy = use_rt ? SCHED_FIFO : SCHED_OTHER,
        .policy_name = use_rt ? "SCHED_FIFO" : "SCHED_OTHER"
    };

    printf("=== Thread Configuration ===\n\n");
    printf("Thread | Name            | Priority | Policy\n");
    printf("-------|-----------------|----------|-------------\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("  %d    | %-15s |    %2d    | %s\n",
               threads[i].id, threads[i].name,
               threads[i].priority, threads[i].policy_name);
    }

    printf("\n=== Starting Execution ===\n");
    printf("\nCreating threads in order: Low → Medium → High\n");
    printf("(Order of creation doesn't matter with SCHED_FIFO priority)\n\n");

    clock_gettime(CLOCK_MONOTONIC, &program_start);

    // Create threads in REVERSE priority order (low first, high last)
    // This demonstrates that SCHED_FIFO priority determines execution, not creation order
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&thread_handles[i], NULL, thread_function, &threads[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
        // Small delay to ensure threads are created in order
        usleep(1000);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    // Display results
    display_results(&program_start);

    // Educational summary
    printf("\n=== Key Takeaways: SCHED_FIFO ===\n\n");
    printf("1. Priority-Based Execution:\n");
    printf("   • Highest priority RT thread always runs first\n");
    printf("   • Lower priority threads only run when higher priority threads block\n\n");

    printf("2. No Time Slicing:\n");
    printf("   • SCHED_FIFO threads run until completion (or until they yield/block)\n");
    printf("   • No fairness - high priority can starve low priority\n");
    printf("   • Deterministic and predictable\n\n");

    printf("3. Preemption Model:\n");
    printf("   • Higher priority preempts lower priority immediately\n");
    printf("   • Same priority: FIFO order (first scheduled runs to completion)\n");
    printf("   • Different from SCHED_RR which adds time slicing at same priority\n\n");

    printf("4. Use Cases:\n");
    printf("   ✓ Hard real-time tasks with deadlines\n");
    printf("   ✓ Critical control loops\n");
    printf("   ✓ When deterministic timing is required\n");
    printf("   ✗ NOT for general-purpose applications\n\n");

    printf("=== Safety Warnings ===\n\n");
    printf("⚠ SCHED_FIFO can lock up your system!\n");
    printf("  • Infinite loop in RT thread = system freeze\n");
    printf("  • Always include yield/sleep in long-running RT threads\n");
    printf("  • Keep priority < 99 (kernel threads use 99)\n");
    printf("  • Test with lower priorities first\n\n");

    printf("=== Experiments to Try ===\n\n");
    printf("1. Compare RT vs Normal:\n");
    printf("   ./sched_fifo_demo              # SCHED_FIFO\n");
    printf("   ./sched_fifo_demo --normal     # SCHED_OTHER\n\n");

    printf("2. Add system load:\n");
    printf("   stress-ng --cpu 4 & ./sched_fifo_demo\n\n");

    printf("3. Check thread scheduling:\n");
    printf("   ps -eLo pid,tid,class,rtprio,comm | grep sched_fifo\n\n");

    printf("4. Modify priorities in source code:\n");
    printf("   • Try same priority for all threads\n");
    printf("   • Observe FIFO behavior within same priority\n\n");

    printf("=== Related Exercises ===\n");
    printf("• Exercise 2: Compares SCHED_OTHER vs SCHED_FIFO vs SCHED_RR\n");
    printf("• Exercise 3: Periodic tasks with SCHED_FIFO (use --rt flag)\n");
    printf("• sample_3threads: Multi-threaded RT application with Rate Monotonic Scheduling\n");

    return 0;
}
