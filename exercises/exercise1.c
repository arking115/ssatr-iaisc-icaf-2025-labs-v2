/*
 * Exercise 1: Basic Latency Measurement
 *
 * This exercise measures the precision of sleep() calls to understand
 * latency and jitter in the system.
 *
 * Concepts:
 * - Latency: deviation from target sleep time
 * - Jitter: variation in latency (max - min)
 * - CLOCK_MONOTONIC for precise timing
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#define ITERATIONS 1000
#define TARGET_INTERVAL_NS 1000000  // 1ms = 1,000,000 ns
#define NSEC_PER_SEC 1000000000L

// Helper function to calculate time difference in nanoseconds
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * NSEC_PER_SEC) + diff_nsec;
}

int main(void) {
    struct timespec start, end, sleep_time;
    long deviations[ITERATIONS];
    long min_deviation = LONG_MAX;
    long max_deviation = 0;
    long total_deviation = 0;
    FILE *outfile;

    printf("=== Exercise 1: Basic Latency Measurement ===\n");
    printf("Target sleep interval: %d ns (%.3f ms)\n", TARGET_INTERVAL_NS,
           TARGET_INTERVAL_NS / 1000000.0);
    printf("Running %d iterations...\n\n", ITERATIONS);

    // Configure sleep time
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = TARGET_INTERVAL_NS;

    // Run measurements
    for (int i = 0; i < ITERATIONS; i++) {
        // Get start time
        if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
            perror("clock_gettime start");
            return 1;
        }

        // Sleep for target interval
        if (clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep_time, NULL) != 0) {
            perror("clock_nanosleep");
            return 1;
        }

        // Get end time
        if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
            perror("clock_gettime end");
            return 1;
        }

        // Calculate actual sleep duration
        long actual_ns = timespec_diff_ns(&start, &end);

        // Calculate deviation from target
        long deviation = actual_ns - TARGET_INTERVAL_NS;
        if (deviation < 0) deviation = -deviation;  // Absolute value

        deviations[i] = deviation;
        total_deviation += deviation;

        // Track min/max
        if (deviation < min_deviation) min_deviation = deviation;
        if (deviation > max_deviation) max_deviation = deviation;

        // Progress indicator
        if ((i + 1) % 100 == 0) {
            printf("Progress: %d/%d iterations completed\r", i + 1, ITERATIONS);
            fflush(stdout);
        }
    }

    printf("\n\n=== Results ===\n");

    // Calculate average
    long avg_deviation = total_deviation / ITERATIONS;
    long jitter = max_deviation - min_deviation;

    printf("Average latency: %ld ns (%.3f µs)\n", avg_deviation, avg_deviation / 1000.0);
    printf("Min deviation:   %ld ns (%.3f µs)\n", min_deviation, min_deviation / 1000.0);
    printf("Max deviation:   %ld ns (%.3f µs)\n", max_deviation, max_deviation / 1000.0);
    printf("Jitter:          %ld ns (%.3f µs)\n", jitter, jitter / 1000.0);

    // Save raw data for visualization
    outfile = fopen("results/latency_raw.txt", "w");
    if (outfile == NULL) {
        perror("Failed to open results/latency_raw.txt");
        printf("\nWarning: Could not save results to file\n");
    } else {
        fprintf(outfile, "# Latency measurements (nanoseconds)\n");
        fprintf(outfile, "# Iteration Deviation_ns\n");
        for (int i = 0; i < ITERATIONS; i++) {
            fprintf(outfile, "%d %ld\n", i, deviations[i]);
        }
        fclose(outfile);
        printf("\nResults saved to: results/latency_raw.txt\n");
        printf("Visualize with: python3 scripts/plot_latency.py results/latency_raw.txt\n");
    }

    // Interpretation
    printf("\n=== Interpretation ===\n");
    if (jitter < 100000) {  // < 100 µs
        printf("✓ Low jitter - Good for soft real-time\n");
    } else if (jitter < 500000) {  // < 500 µs
        printf("⚠ Moderate jitter - Acceptable for some RT tasks\n");
    } else {
        printf("✗ High jitter - Not suitable for hard real-time\n");
    }

    printf("\nNote: Results in Docker show relative performance.\n");
    printf("      For true RT guarantees, use PREEMPT_RT kernel on host.\n");

    return 0;
}
