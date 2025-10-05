/*
 * Exercise 0D: Command-Line Arguments and Busy-Wait Loops
 *
 * This exercise teaches command-line parsing and CPU-bound computation,
 * both essential for real-time task configuration and execution measurement.
 *
 * Learning Objectives:
 * 1. Parse command-line arguments (argc/argv)
 * 2. Implement busy-wait loops for controlled CPU usage
 * 3. Measure actual computation time
 * 4. Understand volatile keyword to prevent compiler optimization
 *
 * Key Concepts:
 * - argc: argument count (includes program name)
 * - argv[]: array of string arguments
 * - strcmp(): string comparison for flags
 * - atoi()/atof(): string to number conversion
 * - volatile: prevents compiler from optimizing away code
 * - Busy-wait: active waiting (CPU-bound) vs sleep (blocking)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NSEC_PER_SEC 1000000000L
#define DEFAULT_DURATION_MS 100

// Calculate time difference in nanoseconds
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * NSEC_PER_SEC) + diff_nsec;
}

/*
 * Busy-wait function: actively consumes CPU for specified duration
 *
 * Important: Uses 'volatile' to prevent compiler optimization
 * Without volatile, the compiler might optimize away the loop
 */
void busy_wait_ms(int duration_ms) {
    struct timespec start, now;
    long target_ns = duration_ms * 1000000L;

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Volatile prevents the compiler from optimizing this loop away
    volatile long sum = 0;

    do {
        // Do some computation
        for (int i = 0; i < 10000; i++) {
            sum += i;
        }
        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (timespec_diff_ns(&start, &now) < target_ns);
}

/*
 * Display usage information
 */
void print_usage(const char *program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("\nOptions:\n");
    printf("  --duration <ms>    Set computation duration in milliseconds (default: %d)\n", DEFAULT_DURATION_MS);
    printf("  --iterations <n>   Number of iterations to run (default: 3)\n");
    printf("  --verbose          Show detailed timing information\n");
    printf("  --help             Display this help message\n");
    printf("\nExamples:\n");
    printf("  %s                              # Run with defaults\n", program_name);
    printf("  %s --duration 50                # Run 50ms computation\n", program_name);
    printf("  %s --duration 200 --iterations 5 --verbose\n", program_name);
}

int main(int argc, char *argv[]) {
    int duration_ms = DEFAULT_DURATION_MS;
    int iterations = 3;
    int verbose = 0;

    printf("=== Exercise 0D: Command-Line Arguments and Busy-Wait ===\n\n");

    /* Step 1: Parse command-line arguments */
    printf("Step 1: Parsing command-line arguments\n");
    printf("  argc = %d\n", argc);
    printf("  Arguments:\n");

    for (int i = 0; i < argc; i++) {
        printf("    argv[%d] = \"%s\"\n", i, argv[i]);
    }
    printf("\n");

    // Process arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        }
        else if (strcmp(argv[i], "--duration") == 0) {
            if (i + 1 < argc) {
                duration_ms = atoi(argv[i + 1]);
                i++;  // Skip next argument (already processed)
            } else {
                fprintf(stderr, "Error: --duration requires a value\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "--iterations") == 0) {
            if (i + 1 < argc) {
                iterations = atoi(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "Error: --iterations requires a value\n");
                return 1;
            }
        }
        else {
            fprintf(stderr, "Error: Unknown argument '%s'\n", argv[i]);
            fprintf(stderr, "Run '%s --help' for usage information\n", argv[0]);
            return 1;
        }
    }

    printf("Configuration:\n");
    printf("  Duration:   %d ms\n", duration_ms);
    printf("  Iterations: %d\n", iterations);
    printf("  Verbose:    %s\n\n", verbose ? "Yes" : "No");

    /* Step 2: Run busy-wait computations */
    printf("Step 2: Running busy-wait computations\n\n");

    printf("Iter | Target (ms) | Actual (ms) | Error (µs) | Error (%%)\n");
    printf("-----|-------------|-------------|------------|----------\n");

    long total_error_ns = 0;

    for (int iter = 0; iter < iterations; iter++) {
        struct timespec start, end;

        // Measure actual computation time
        clock_gettime(CLOCK_MONOTONIC, &start);
        busy_wait_ms(duration_ms);
        clock_gettime(CLOCK_MONOTONIC, &end);

        long actual_ns = timespec_diff_ns(&start, &end);
        long target_ns = duration_ms * 1000000L;
        long error_ns = actual_ns - target_ns;
        double error_percent = (error_ns * 100.0) / target_ns;

        total_error_ns += (error_ns > 0 ? error_ns : -error_ns);

        printf(" %2d  |    %6d   |   %8.3f  |  %+7.1f   | %+6.2f%%\n",
               iter,
               duration_ms,
               actual_ns / 1000000.0,
               error_ns / 1000.0,
               error_percent);

        if (verbose) {
            printf("      └─ Start: %ld.%09ld, End: %ld.%09ld\n",
                   start.tv_sec, start.tv_nsec,
                   end.tv_sec, end.tv_nsec);
        }
    }

    long avg_error_ns = total_error_ns / iterations;
    printf("\nAverage absolute error: %.1f µs\n", avg_error_ns / 1000.0);

    /* Step 3: Demonstrate volatile importance */
    printf("\n\nStep 3: Why 'volatile' matters\n");
    printf("The busy_wait_ms() function uses 'volatile long sum'\n");
    printf("Without volatile, the compiler might optimize away the computation loop,\n");
    printf("making timing measurements inaccurate.\n");

    printf("\nVolatile tells the compiler: 'This variable may change unexpectedly,\n");
    printf("don't optimize code that uses it.'\n");

    /* Summary */
    printf("\n=== Key Takeaways ===\n");
    printf("✓ argc = number of arguments (including program name)\n");
    printf("✓ argv[0] = program name, argv[1]... = user arguments\n");
    printf("✓ Use strcmp() to compare string arguments\n");
    printf("✓ Use atoi()/atof() to convert strings to numbers\n");
    printf("✓ Busy-wait loops actively consume CPU (unlike sleep)\n");
    printf("✓ volatile prevents compiler optimization of timing-critical code\n");
    printf("✓ Always validate argument parsing (check argc before accessing argv)\n");

    printf("\n=== Usage in RT Exercises ===\n");
    printf("Exercise 3 uses '--rt' flag to enable real-time scheduling\n");
    printf("Exercise 2 uses busy-wait (do_work) to simulate CPU-bound tasks\n");
    printf("Exercise 3 uses do_computation() with adjustable COMPUTATION_USEC\n");

    printf("\n=== Try These Commands ===\n");
    printf("  %s --help\n", argv[0]);
    printf("  %s --duration 50\n", argv[0]);
    printf("  %s --duration 200 --iterations 10 --verbose\n", argv[0]);

    printf("\n=== Next Steps ===\n");
    printf("Continue to Exercise 0E to learn about scheduling policies.\n");

    return 0;
}
