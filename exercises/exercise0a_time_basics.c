/*
 * Exercise 0A: Time Basics
 *
 * This introductory exercise teaches fundamental time manipulation in C,
 * which is essential for real-time programming.
 *
 * Learning Objectives:
 * 1. Use clock_gettime() to read high-precision timestamps
 * 2. Understand struct timespec (seconds + nanoseconds)
 * 3. Perform time arithmetic (difference calculation)
 * 4. Convert between time units (nanoseconds, microseconds, milliseconds)
 *
 * Key Concepts:
 * - CLOCK_MONOTONIC: A clock that cannot go backwards (unlike wall-clock time)
 * - struct timespec: { time_t tv_sec; long tv_nsec; }
 * - Nanosecond precision: 1 second = 1,000,000,000 nanoseconds
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NSEC_PER_SEC 1000000000L

/*
 * Function: timespec_to_ns
 * Converts a timespec structure to total nanoseconds
 */
long long timespec_to_ns(struct timespec *ts) {
    return (long long)ts->tv_sec * NSEC_PER_SEC + ts->tv_nsec;
}

/*
 * Function: timespec_diff_ns
 * Calculates the difference between two timespec structures in nanoseconds
 */
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * NSEC_PER_SEC) + diff_nsec;
}

int main(void) {
    struct timespec start, end;

    printf("=== Exercise 0A: Time Basics ===\n\n");

    /* Step 1: Read current time */
    printf("Step 1: Reading current time with clock_gettime()\n");

    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime failed");
        return 1;
    }

    printf("  Current time: %ld seconds, %ld nanoseconds\n",
           start.tv_sec, start.tv_nsec);
    printf("  Total nanoseconds: %lld\n\n", timespec_to_ns(&start));

    /* Step 2: Perform some work (sleep) */
    printf("Step 2: Sleeping for 100 milliseconds...\n");
    usleep(100000);  // Sleep 100,000 microseconds = 100 ms

    /* Step 3: Read time again */
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        perror("clock_gettime failed");
        return 1;
    }

    printf("  After sleep: %ld seconds, %ld nanoseconds\n\n",
           end.tv_sec, end.tv_nsec);

    /* Step 4: Calculate elapsed time */
    printf("Step 3: Calculating elapsed time\n");

    long elapsed_ns = timespec_diff_ns(&start, &end);

    printf("  Elapsed time (nanoseconds):  %ld ns\n", elapsed_ns);
    printf("  Elapsed time (microseconds): %.1f µs\n", elapsed_ns / 1000.0);
    printf("  Elapsed time (milliseconds): %.3f ms\n", elapsed_ns / 1000000.0);
    printf("  Elapsed time (seconds):      %.6f s\n\n", elapsed_ns / 1000000000.0);

    /* Step 5: Practice - Multiple measurements */
    printf("Step 4: Practice - Measuring different sleep durations\n\n");

    int sleep_durations_us[] = {1000, 5000, 10000, 50000};  // microseconds

    printf("Target (ms) | Actual (ms) | Error (µs)\n");
    printf("------------|-------------|----------\n");

    for (int i = 0; i < 4; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        usleep(sleep_durations_us[i]);
        clock_gettime(CLOCK_MONOTONIC, &end);

        long measured_ns = timespec_diff_ns(&start, &end);
        long target_ns = sleep_durations_us[i] * 1000L;
        long error_ns = measured_ns - target_ns;

        printf("   %6.1f   |   %7.3f   |  %+7.1f\n",
               sleep_durations_us[i] / 1000.0,
               measured_ns / 1000000.0,
               error_ns / 1000.0);
    }

    /* Summary */
    printf("\n=== Key Takeaways ===\n");
    printf("✓ clock_gettime(CLOCK_MONOTONIC, ...) gives high-precision time\n");
    printf("✓ struct timespec has two fields: tv_sec (seconds) and tv_nsec (nanoseconds)\n");
    printf("✓ Time arithmetic requires careful handling of seconds and nanoseconds\n");
    printf("✓ Unit conversions:\n");
    printf("    1 second (s)      = 1,000 milliseconds (ms)\n");
    printf("    1 millisecond     = 1,000 microseconds (µs)\n");
    printf("    1 microsecond     = 1,000 nanoseconds (ns)\n");
    printf("✓ Sleep functions are not perfectly accurate (observe the errors above)\n");

    printf("\n=== Next Steps ===\n");
    printf("This time measurement foundation is used in Exercise 1 to measure latency.\n");
    printf("Continue to Exercise 0B to learn about threading.\n");

    return 0;
}
