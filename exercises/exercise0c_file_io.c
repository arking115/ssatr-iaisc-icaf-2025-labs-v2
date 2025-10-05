/*
 * Exercise 0C: Structs and File I/O
 *
 * This exercise demonstrates how to organize measurement data using structs
 * and save results to files for later analysis.
 *
 * Learning Objectives:
 * 1. Define and use custom structs for data organization
 * 2. Store measurements in arrays
 * 3. Write data to files using fprintf()
 * 4. Proper file error handling
 *
 * Key Concepts:
 * - typedef struct: Creating custom data types
 * - Arrays of structs: Storing multiple measurements
 * - File operations: fopen(), fprintf(), fclose()
 * - Error checking: NULL pointer checks
 * - CSV-style output for easy plotting
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_MEASUREMENTS 10
#define NSEC_PER_SEC 1000000000L

/*
 * Structure to hold a single measurement
 * This pattern is used throughout RT exercises to store timing data
 */
typedef struct {
    int iteration;           // Measurement number
    long timestamp_ns;       // When the measurement was taken
    long value_ns;          // The measured value (e.g., latency)
    int is_valid;           // Status flag
} measurement_t;

// Calculate time difference in nanoseconds
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * NSEC_PER_SEC) + diff_nsec;
}

int main(void) {
    measurement_t measurements[NUM_MEASUREMENTS];
    struct timespec program_start, measurement_time, prev_time;
    FILE *output_file;

    printf("=== Exercise 0C: Structs and File I/O ===\n\n");

    /* Step 1: Collect measurements into struct array */
    printf("Step 1: Collecting %d measurements into array\n", NUM_MEASUREMENTS);

    clock_gettime(CLOCK_MONOTONIC, &program_start);
    prev_time = program_start;

    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        // Sleep for a random duration (10-100 ms)
        int sleep_ms = 10 + (rand() % 90);
        usleep(sleep_ms * 1000);

        // Take measurement
        clock_gettime(CLOCK_MONOTONIC, &measurement_time);

        // Calculate interval since last measurement
        long interval_ns = timespec_diff_ns(&prev_time, &measurement_time);

        // Store in struct
        measurements[i].iteration = i;
        measurements[i].timestamp_ns = timespec_diff_ns(&program_start, &measurement_time);
        measurements[i].value_ns = interval_ns;
        measurements[i].is_valid = 1;

        printf("  Measurement %2d: interval = %6.2f ms\n",
               i, interval_ns / 1000000.0);

        prev_time = measurement_time;
    }

    /* Step 2: Write data to file */
    printf("\nStep 2: Writing measurements to file\n");

    // Open file for writing
    output_file = fopen("results/exercise0c_data.txt", "w");
    if (output_file == NULL) {
        perror("Error opening file");
        printf("\nTrying to create results directory...\n");

        // Try to create directory (may fail, which is OK if it exists)
        system("mkdir -p results");

        // Try again
        output_file = fopen("results/exercise0c_data.txt", "w");
        if (output_file == NULL) {
            perror("Error: Still cannot open file");
            printf("Falling back to stdout output only.\n\n");
        }
    }

    if (output_file != NULL) {
        // Write header comments
        fprintf(output_file, "# Exercise 0C Measurement Data\n");
        fprintf(output_file, "# Format: iteration timestamp_ms interval_ms valid\n");
        fprintf(output_file, "#\n");

        // Write data rows
        for (int i = 0; i < NUM_MEASUREMENTS; i++) {
            fprintf(output_file, "%d %.3f %.3f %d\n",
                    measurements[i].iteration,
                    measurements[i].timestamp_ns / 1000000.0,
                    measurements[i].value_ns / 1000000.0,
                    measurements[i].is_valid);
        }

        fclose(output_file);
        printf("  ✓ Data written to: results/exercise0c_data.txt\n");
    }

    /* Step 3: Display summary statistics from array */
    printf("\nStep 3: Computing statistics from stored data\n");

    long total = 0;
    long min = measurements[0].value_ns;
    long max = measurements[0].value_ns;

    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        total += measurements[i].value_ns;
        if (measurements[i].value_ns < min) min = measurements[i].value_ns;
        if (measurements[i].value_ns > max) max = measurements[i].value_ns;
    }

    long average = total / NUM_MEASUREMENTS;

    printf("\n  Interval Statistics:\n");
    printf("    Min:     %6.2f ms\n", min / 1000000.0);
    printf("    Max:     %6.2f ms\n", max / 1000000.0);
    printf("    Average: %6.2f ms\n", average / 1000000.0);
    printf("    Range:   %6.2f ms\n", (max - min) / 1000000.0);

    /* Step 4: Demonstrate struct initialization patterns */
    printf("\n\nBonus: Different Ways to Initialize Structs\n");

    // Method 1: Direct assignment
    measurement_t m1;
    m1.iteration = 0;
    m1.timestamp_ns = 1000000;
    m1.value_ns = 500000;
    m1.is_valid = 1;

    // Method 2: Designated initializers (preferred for clarity)
    measurement_t m2 = {
        .iteration = 1,
        .timestamp_ns = 2000000,
        .value_ns = 600000,
        .is_valid = 1
    };

    // Method 3: Partial initialization (unset fields = 0)
    measurement_t m3 = {
        .iteration = 2,
        .value_ns = 700000
        // timestamp_ns and is_valid will be 0
    };

    printf("  m1: iter=%d, timestamp=%ld, value=%ld, valid=%d\n",
           m1.iteration, m1.timestamp_ns, m1.value_ns, m1.is_valid);
    printf("  m2: iter=%d, timestamp=%ld, value=%ld, valid=%d\n",
           m2.iteration, m2.timestamp_ns, m2.value_ns, m2.is_valid);
    printf("  m3: iter=%d, timestamp=%ld, value=%ld, valid=%d (partial init)\n",
           m3.iteration, m3.timestamp_ns, m3.value_ns, m3.is_valid);

    /* Summary */
    printf("\n=== Key Takeaways ===\n");
    printf("✓ Structs organize related data (better than separate arrays)\n");
    printf("✓ Arrays of structs store multiple measurements efficiently\n");
    printf("✓ Use designated initializers: .field = value for clarity\n");
    printf("✓ File I/O pattern:\n");
    printf("    1. fopen() - check for NULL\n");
    printf("    2. fprintf() - write formatted data\n");
    printf("    3. fclose() - always close files\n");
    printf("✓ Add header comments to files for documentation\n");
    printf("✓ CSV/space-separated format enables easy plotting with Python/gnuplot\n");

    printf("\n=== Usage in RT Exercises ===\n");
    printf("Exercise 1 uses this pattern to save latency data to results/latency_raw.txt\n");
    printf("Exercise 3 stores cycle_data_t structs to track deadline misses\n");

    printf("\n=== Next Steps ===\n");
    printf("Continue to Exercise 0D to learn command-line arguments and busy-wait loops.\n");

    return 0;
}
