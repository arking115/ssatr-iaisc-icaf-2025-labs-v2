/*
 * Exercise 0F: Statistics and Array Manipulation
 *
 * This exercise teaches statistical analysis and array manipulation,
 * essential for analyzing real-time system performance.
 *
 * Learning Objectives:
 * 1. Calculate min, max, average from arrays
 * 2. Compute jitter (variation in timing)
 * 3. Implement simple sorting algorithms
 * 4. Format output in tables
 * 5. Calculate percentiles and distributions
 *
 * Key Concepts:
 * - Statistical measures: min, max, mean, jitter
 * - Jitter = max - min (measure of variability)
 * - Bubble sort for ordering data
 * - Percentiles (50th = median, 95th, 99th)
 * - Formatted output with printf
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define NUM_SAMPLES 100

// Generate sample latency measurements (simulated)
void generate_samples(long *samples, int count) {
    srand(time(NULL));

    for (int i = 0; i < count; i++) {
        // Simulate latencies: base 50µs + random 0-200µs
        samples[i] = 50000 + (rand() % 200000);

        // Add occasional spikes (10% chance of 2x latency)
        if (rand() % 10 == 0) {
            samples[i] *= 2;
        }
    }
}

// Calculate minimum value in array
long find_min(long *array, int count) {
    long min = LONG_MAX;
    for (int i = 0; i < count; i++) {
        if (array[i] < min) {
            min = array[i];
        }
    }
    return min;
}

// Calculate maximum value in array
long find_max(long *array, int count) {
    long max = 0;
    for (int i = 0; i < count; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

// Calculate average (mean)
long calculate_average(long *array, int count) {
    long sum = 0;
    for (int i = 0; i < count; i++) {
        sum += array[i];
    }
    return sum / count;
}

// Bubble sort (simple, good for small arrays)
void bubble_sort(long *array, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Swap
                long temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

// Calculate percentile from sorted array
long percentile(long *sorted_array, int count, int percent) {
    int index = (count - 1) * percent / 100;
    return sorted_array[index];
}

// Count values in a range
int count_in_range(long *array, int count, long min, long max) {
    int cnt = 0;
    for (int i = 0; i < count; i++) {
        if (array[i] >= min && array[i] <= max) {
            cnt++;
        }
    }
    return cnt;
}

int main(void) {
    long samples[NUM_SAMPLES];
    long sorted[NUM_SAMPLES];

    printf("=== Exercise 0F: Statistics and Array Manipulation ===\n\n");

    /* Step 1: Generate sample data */
    printf("Step 1: Generating %d simulated latency measurements\n\n", NUM_SAMPLES);
    generate_samples(samples, NUM_SAMPLES);

    printf("First 10 samples (nanoseconds):\n");
    for (int i = 0; i < 10; i++) {
        printf("  samples[%2d] = %6ld ns (%.1f µs)\n",
               i, samples[i], samples[i] / 1000.0);
    }

    /* Step 2: Calculate basic statistics */
    printf("\n\nStep 2: Calculating basic statistics\n");

    long min = find_min(samples, NUM_SAMPLES);
    long max = find_max(samples, NUM_SAMPLES);
    long avg = calculate_average(samples, NUM_SAMPLES);
    long jitter = max - min;

    printf("\n  Minimum:     %6ld ns (%.1f µs)\n", min, min / 1000.0);
    printf("  Maximum:     %6ld ns (%.1f µs)\n", max, max / 1000.0);
    printf("  Average:     %6ld ns (%.1f µs)\n", avg, avg / 1000.0);
    printf("  Jitter:      %6ld ns (%.1f µs)\n", jitter, jitter / 1000.0);

    /* Step 3: Sort and calculate percentiles */
    printf("\n\nStep 3: Sorting data and calculating percentiles\n");

    // Copy to sorted array
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sorted[i] = samples[i];
    }

    bubble_sort(sorted, NUM_SAMPLES);

    printf("\n  Sorted samples (first 5 and last 5):\n");
    for (int i = 0; i < 5; i++) {
        printf("    [%2d] %6ld ns (%.1f µs)\n",
               i, sorted[i], sorted[i] / 1000.0);
    }
    printf("    ...\n");
    for (int i = NUM_SAMPLES - 5; i < NUM_SAMPLES; i++) {
        printf("    [%2d] %6ld ns (%.1f µs)\n",
               i, sorted[i], sorted[i] / 1000.0);
    }

    printf("\n  Percentiles:\n");
    printf("    50th (median): %6ld ns (%.1f µs)\n",
           percentile(sorted, NUM_SAMPLES, 50),
           percentile(sorted, NUM_SAMPLES, 50) / 1000.0);
    printf("    90th:          %6ld ns (%.1f µs)\n",
           percentile(sorted, NUM_SAMPLES, 90),
           percentile(sorted, NUM_SAMPLES, 90) / 1000.0);
    printf("    95th:          %6ld ns (%.1f µs)\n",
           percentile(sorted, NUM_SAMPLES, 95),
           percentile(sorted, NUM_SAMPLES, 95) / 1000.0);
    printf("    99th:          %6ld ns (%.1f µs)\n",
           percentile(sorted, NUM_SAMPLES, 99),
           percentile(sorted, NUM_SAMPLES, 99) / 1000.0);

    /* Step 4: Distribution analysis */
    printf("\n\nStep 4: Analyzing distribution\n\n");

    printf("Latency Range      | Count | Percentage\n");
    printf("-------------------|-------|------------\n");

    struct {
        long min;
        long max;
        const char *label;
    } ranges[] = {
        {0, 100000, "0-100 µs"},
        {100000, 200000, "100-200 µs"},
        {200000, 300000, "200-300 µs"},
        {300000, 400000, "300-400 µs"},
        {400000, LONG_MAX, "> 400 µs"}
    };

    for (int i = 0; i < 5; i++) {
        int count = count_in_range(samples, NUM_SAMPLES, ranges[i].min, ranges[i].max);
        double percent = (count * 100.0) / NUM_SAMPLES;

        printf("%-18s | %5d | %5.1f%%\n", ranges[i].label, count, percent);
    }

    /* Step 5: Formatted table output */
    printf("\n\nStep 5: Example formatted output table\n\n");

    printf("Sample | Value (ns) | Value (µs) | Deviation from Avg | Status\n");
    printf("-------|------------|------------|-------------------|--------\n");

    for (int i = 0; i < 10; i++) {
        long deviation = samples[i] - avg;
        const char *status;

        if (samples[i] <= avg) {
            status = "GOOD";
        } else if (samples[i] <= avg * 1.5) {
            status = "OK";
        } else {
            status = "HIGH";
        }

        printf("  %3d  |  %8ld  |  %7.1f   |      %+8ld      | %s\n",
               i, samples[i], samples[i] / 1000.0, deviation, status);
    }

    /* Summary */
    printf("\n\n=== Key Takeaways ===\n");
    printf("✓ Basic statistics: min, max, average\n");
    printf("✓ Jitter = max - min (critical for RT systems)\n");
    printf("✓ Bubble sort: simple O(n²) sorting algorithm\n");
    printf("   - for (i) for (j) if (a[j] > a[j+1]) swap\n");
    printf("✓ Percentiles require sorted data\n");
    printf("   - 50th percentile = median\n");
    printf("   - 95th/99th show tail latencies (important for RT!)\n");
    printf("✓ Distribution analysis: count samples in ranges\n");
    printf("✓ Formatted output: use printf width specifiers\n");
    printf("   - %%6ld = 6 chars wide, left-padded\n");
    printf("   - %%.1f = 1 decimal place\n");
    printf("   - %%+8ld = show sign, 8 chars wide\n");

    printf("\n=== Why This Matters for RT ===\n");
    printf("Real-time systems care about:\n");
    printf("  • Worst-case latency (max, 99th percentile) - NOT average!\n");
    printf("  • Jitter - variation must be bounded\n");
    printf("  • Distribution - understanding outliers\n");
    printf("\n");
    printf("A system with avg=50µs but max=10ms is NOT suitable for hard RT,\n");
    printf("even though the average looks good!\n");

    printf("\n=== Usage in RT Exercises ===\n");
    printf("Exercise 1: Calculates min/max/avg/jitter for latency measurements\n");
    printf("Exercise 2: Sorts threads by completion time using bubble sort\n");
    printf("Exercise 3: Tracks min/max latency to detect RT performance\n");

    printf("\n=== Next Steps ===\n");
    printf("You now have all the foundational skills for Exercise 1!\n");
    printf("Exercise 1 combines: time measurement + statistics + file I/O\n");

    return 0;
}
