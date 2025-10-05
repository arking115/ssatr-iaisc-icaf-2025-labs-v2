/*
 * Exercise 3: Periodic Task Simulation
 *
 * This exercise simulates a real-time periodic task (like a control loop)
 * and demonstrates deadline detection and RT scheduling benefits.
 *
 * Concepts:
 * - Periodic tasks with fixed intervals
 * - Deadline detection
 * - WCET (Worst Case Execution Time)
 * - Utilization factor
 * - RT vs normal scheduling comparison
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>
#include <limits.h>

// Task configuration
#define PERIOD_MS 10                   // 10ms period
#define COMPUTATION_USEC 2000          // 2ms computation time (20% utilization)
#define DURATION_SEC 5                 // Run for 5 seconds
#define NSEC_PER_SEC 1000000000L
#define NSEC_PER_MSEC 1000000L
#define NSEC_PER_USEC 1000L
#define RT_PRIORITY 80

// Calculate number of cycles
#define NUM_CYCLES ((DURATION_SEC * 1000) / PERIOD_MS)

// Deadline miss tracking
typedef struct {
    int cycle;
    long latency_ns;
    int missed;
} cycle_data_t;

cycle_data_t cycles[NUM_CYCLES];

// Time manipulation functions
void timespec_add_ns(struct timespec *ts, long ns) {
    ts->tv_nsec += ns;
    while (ts->tv_nsec >= NSEC_PER_SEC) {
        ts->tv_sec++;
        ts->tv_nsec -= NSEC_PER_SEC;
    }
}

long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * NSEC_PER_SEC) + diff_nsec;
}

// Simulate computational work
void do_computation(long duration_usec) {
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Busy-wait for specified duration
    volatile long sum = 0;
    do {
        for (int i = 0; i < 1000; i++) {
            sum += i;
        }
        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (timespec_diff_ns(&start, &now) < (duration_usec * NSEC_PER_USEC));
}

int main(int argc, char *argv[]) {
    int use_rt = 0;
    struct timespec next_activation, actual_activation, completion;
    long period_ns = PERIOD_MS * NSEC_PER_MSEC;
    int missed_deadlines = 0;
    long min_latency = LONG_MAX;
    long max_latency = 0;
    long total_latency = 0;

    // Parse command line arguments
    if (argc > 1 && strcmp(argv[1], "--rt") == 0) {
        use_rt = 1;
    }

    printf("=== Exercise 3: Periodic Task Simulation ===\n\n");

    // Set up RT scheduling if requested
    if (use_rt) {
        struct sched_param param;
        param.sched_priority = RT_PRIORITY;

        if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
            perror("Warning: sched_setscheduler failed");
            printf("Continuing with normal scheduling (may need --privileged)\n\n");
            use_rt = 0;
        } else {
            printf("Running with SCHED_FIFO priority %d\n\n", RT_PRIORITY);
        }
    } else {
        printf("Running with SCHED_OTHER (normal scheduling)\n\n");
    }

    // Display task configuration
    printf("Task Configuration:\n");
    printf("  Period: %d ms\n", PERIOD_MS);
    printf("  Computation time: %d µs\n", (int)COMPUTATION_USEC);
    printf("  Utilization: %.1f%%\n", (COMPUTATION_USEC * 100.0) / (PERIOD_MS * 1000.0));
    printf("  Duration: %d seconds\n", DURATION_SEC);
    printf("  Expected cycles: %d\n\n", NUM_CYCLES);

    // Get initial time for first activation
    clock_gettime(CLOCK_MONOTONIC, &next_activation);
    timespec_add_ns(&next_activation, period_ns);

    printf("Cycle | Latency (µs) | Status\n");
    printf("------|--------------|-------\n");

    // Main periodic loop
    for (int cycle = 0; cycle < NUM_CYCLES; cycle++) {
        // Sleep until next activation
        if (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL) != 0) {
            perror("clock_nanosleep");
            return 1;
        }

        // Record actual activation time
        clock_gettime(CLOCK_MONOTONIC, &actual_activation);

        // Calculate activation latency
        long latency_ns = timespec_diff_ns(&next_activation, &actual_activation);
        cycles[cycle].cycle = cycle;
        cycles[cycle].latency_ns = latency_ns;

        // Perform computation
        do_computation(COMPUTATION_USEC);

        // Check completion time
        clock_gettime(CLOCK_MONOTONIC, &completion);

        // Detect deadline miss
        struct timespec deadline = next_activation;
        timespec_add_ns(&deadline, period_ns);
        long time_to_deadline = timespec_diff_ns(&completion, &deadline);

        if (time_to_deadline < 0) {
            cycles[cycle].missed = 1;
            missed_deadlines++;
        } else {
            cycles[cycle].missed = 0;
        }

        // Update statistics
        if (latency_ns < min_latency) min_latency = latency_ns;
        if (latency_ns > max_latency) max_latency = latency_ns;
        total_latency += latency_ns;

        // Display progress (every 100 cycles)
        if (cycle % 100 == 0 || cycles[cycle].missed) {
            printf("%5d | %12.1f | %s\n",
                   cycle,
                   latency_ns / 1000.0,
                   cycles[cycle].missed ? "MISS" : "OK");
        }

        // Calculate next activation time
        timespec_add_ns(&next_activation, period_ns);
    }

    // Display results
    printf("\n=== Results ===\n");
    printf("Total cycles:      %d\n", NUM_CYCLES);
    printf("Missed deadlines:  %d (%.2f%%)\n",
           missed_deadlines,
           (missed_deadlines * 100.0) / NUM_CYCLES);

    printf("\nLatency Statistics:\n");
    printf("  Min:     %ld µs\n", min_latency / 1000);
    printf("  Max:     %ld µs\n", max_latency / 1000);
    printf("  Average: %ld µs\n", (total_latency / NUM_CYCLES) / 1000);
    printf("  Jitter:  %ld µs\n", (max_latency - min_latency) / 1000);

    // Analysis
    printf("\n=== Analysis ===\n");

    if (missed_deadlines == 0) {
        printf("✓ No deadline misses - Task is schedulable\n");
    } else {
        printf("✗ Deadline misses detected - Task overload or interference\n");
    }

    long jitter = max_latency - min_latency;
    if (use_rt) {
        if (jitter < 100000) {  // < 100 µs
            printf("✓ Low jitter with RT scheduling\n");
        } else {
            printf("⚠ Higher jitter (Docker/host kernel interference)\n");
        }
    } else {
        printf("⚠ Normal scheduling - expect higher jitter\n");
        printf("  Try: ./exercise3 --rt for better determinism\n");
    }

    // Utilization check
    double utilization = (COMPUTATION_USEC * 100.0) / (PERIOD_MS * 1000.0);
    if (utilization > 70.0) {
        printf("⚠ High utilization (%.1f%%) - limited safety margin\n", utilization);
    } else {
        printf("✓ Safe utilization (%.1f%%) - good safety margin\n", utilization);
    }

    printf("\n=== Key Takeaways ===\n");
    printf("• Periodic tasks need: period > WCET\n");
    printf("• RT scheduling reduces jitter\n");
    printf("• Deadline misses indicate overload\n");
    printf("• Utilization should be < 100%% (preferably < 70%%)\n");

    printf("\n=== Experiments to Try ===\n");
    printf("1. Compare RT vs normal: ./exercise3 vs ./exercise3 --rt\n");
    printf("2. Increase load: Edit COMPUTATION_USEC to 8000 and observe misses\n");
    printf("3. Add system load: stress-ng --cpu 4 & ./exercise3 --rt\n");
    printf("4. Find breaking point: gradually increase COMPUTATION_USEC\n");

    return 0;
}
