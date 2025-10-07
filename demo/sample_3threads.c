/*
 * Sample 3-Thread Real-Time Application
 *
 * This demo application showcases a multi-threaded real-time system with
 * three periodic tasks using Rate Monotonic Scheduling (RMS).
 *
 * Architecture:
 * - Thread 1 (Sensor):     5ms period, priority 90 - Reads sensor data
 * - Thread 2 (Controller): 10ms period, priority 80 - Processes control algorithm
 * - Thread 3 (Logger):     20ms period, priority 70 - Logs system state
 *
 * Concepts Demonstrated:
 * - Rate Monotonic Scheduling (shorter period = higher priority)
 * - Periodic real-time tasks with deadline detection
 * - Multi-threaded RT application design
 * - Shared data structures between threads
 * - RT scheduling with SCHED_FIFO
 * - Statistics collection and analysis
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
#include <signal.h>
#include <limits.h>
#include <math.h>

// Configuration
#define DURATION_SEC 5                 // Default run duration
#define NSEC_PER_SEC 1000000000L
#define NSEC_PER_MSEC 1000000L
#define NSEC_PER_USEC 1000L

// Thread configuration
typedef struct {
    int id;
    const char *name;
    int period_ms;
    int computation_usec;
    int priority;
    int num_cycles;
    int use_rt;
} thread_config_t;

// Statistics per cycle
typedef struct {
    long latency_ns;
    int missed_deadline;
} cycle_stat_t;

// Thread runtime data
typedef struct {
    thread_config_t config;
    pthread_t handle;
    cycle_stat_t *stats;
    int missed_count;
    long min_latency;
    long max_latency;
    long total_latency;
    int completed;
} thread_data_t;

// Shared data between threads (simulating sensor -> controller -> logger flow)
typedef struct {
    volatile int sensor_value;
    volatile int control_output;
    volatile int log_counter;
    struct timespec last_update;
} shared_data_t;

shared_data_t shared_data = {0};
volatile int keep_running = 1;

// Thread data
thread_data_t threads[3];

// Signal handler for graceful shutdown
void signal_handler(int sig) {
    (void)sig;
    keep_running = 0;
}

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

    volatile long sum = 0;
    do {
        for (int i = 0; i < 1000; i++) {
            sum += i;
        }
        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (timespec_diff_ns(&start, &now) < (duration_usec * NSEC_PER_USEC));
}

// Thread 1: High-priority sensor reader
void* sensor_thread(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    struct timespec next_activation, actual_activation, completion;
    long period_ns = data->config.period_ms * NSEC_PER_MSEC;

    // Get initial time
    clock_gettime(CLOCK_MONOTONIC, &next_activation);
    timespec_add_ns(&next_activation, period_ns);

    for (int cycle = 0; cycle < data->config.num_cycles && keep_running; cycle++) {
        // Wait for next period
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);
        clock_gettime(CLOCK_MONOTONIC, &actual_activation);

        // Calculate latency
        long latency_ns = timespec_diff_ns(&next_activation, &actual_activation);
        data->stats[cycle].latency_ns = latency_ns;

        // Simulate sensor reading
        do_computation(data->config.computation_usec);

        // Update shared data (simulated sensor value)
        shared_data.sensor_value = (shared_data.sensor_value + 1) % 1000;
        clock_gettime(CLOCK_MONOTONIC, &shared_data.last_update);

        // Check for deadline miss
        clock_gettime(CLOCK_MONOTONIC, &completion);
        struct timespec deadline = next_activation;
        timespec_add_ns(&deadline, period_ns);

        if (timespec_diff_ns(&completion, &deadline) < 0) {
            data->stats[cycle].missed_deadline = 1;
            data->missed_count++;
        }

        // Update statistics
        if (latency_ns < data->min_latency) data->min_latency = latency_ns;
        if (latency_ns > data->max_latency) data->max_latency = latency_ns;
        data->total_latency += latency_ns;

        // Next activation
        timespec_add_ns(&next_activation, period_ns);
    }

    data->completed = 1;
    return NULL;
}

// Thread 2: Medium-priority controller
void* controller_thread(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    struct timespec next_activation, actual_activation, completion;
    long period_ns = data->config.period_ms * NSEC_PER_MSEC;

    clock_gettime(CLOCK_MONOTONIC, &next_activation);
    timespec_add_ns(&next_activation, period_ns);

    for (int cycle = 0; cycle < data->config.num_cycles && keep_running; cycle++) {
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);
        clock_gettime(CLOCK_MONOTONIC, &actual_activation);

        long latency_ns = timespec_diff_ns(&next_activation, &actual_activation);
        data->stats[cycle].latency_ns = latency_ns;

        // Simulate control algorithm (read sensor, compute output)
        do_computation(data->config.computation_usec);

        int sensor_input = shared_data.sensor_value;
        shared_data.control_output = sensor_input * 2;  // Simple computation

        clock_gettime(CLOCK_MONOTONIC, &completion);
        struct timespec deadline = next_activation;
        timespec_add_ns(&deadline, period_ns);

        if (timespec_diff_ns(&completion, &deadline) < 0) {
            data->stats[cycle].missed_deadline = 1;
            data->missed_count++;
        }

        if (latency_ns < data->min_latency) data->min_latency = latency_ns;
        if (latency_ns > data->max_latency) data->max_latency = latency_ns;
        data->total_latency += latency_ns;

        timespec_add_ns(&next_activation, period_ns);
    }

    data->completed = 1;
    return NULL;
}

// Thread 3: Low-priority logger
void* logger_thread(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    struct timespec next_activation, actual_activation, completion;
    long period_ns = data->config.period_ms * NSEC_PER_MSEC;

    clock_gettime(CLOCK_MONOTONIC, &next_activation);
    timespec_add_ns(&next_activation, period_ns);

    for (int cycle = 0; cycle < data->config.num_cycles && keep_running; cycle++) {
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);
        clock_gettime(CLOCK_MONOTONIC, &actual_activation);

        long latency_ns = timespec_diff_ns(&next_activation, &actual_activation);
        data->stats[cycle].latency_ns = latency_ns;

        // Simulate logging (read shared data, perform I/O simulation)
        do_computation(data->config.computation_usec);

        shared_data.log_counter++;
        // In real application: write to file/network

        clock_gettime(CLOCK_MONOTONIC, &completion);
        struct timespec deadline = next_activation;
        timespec_add_ns(&deadline, period_ns);

        if (timespec_diff_ns(&completion, &deadline) < 0) {
            data->stats[cycle].missed_deadline = 1;
            data->missed_count++;
        }

        if (latency_ns < data->min_latency) data->min_latency = latency_ns;
        if (latency_ns > data->max_latency) data->max_latency = latency_ns;
        data->total_latency += latency_ns;

        timespec_add_ns(&next_activation, period_ns);
    }

    data->completed = 1;
    return NULL;
}

// Map thread ID to function
void* (*thread_functions[])(void*) = {
    sensor_thread,
    controller_thread,
    logger_thread
};

// Initialize thread configuration
void init_thread_configs(int duration_sec, int use_rt) {
    // Thread 1: Sensor - Highest priority (shortest period)
    threads[0].config = (thread_config_t){
        .id = 0,
        .name = "Sensor",
        .period_ms = 5,
        .computation_usec = 800,
        .priority = 90,
        .num_cycles = (duration_sec * 1000) / 5,
        .use_rt = use_rt
    };

    // Thread 2: Controller - Medium priority
    threads[1].config = (thread_config_t){
        .id = 1,
        .name = "Controller",
        .period_ms = 10,
        .computation_usec = 2000,
        .priority = 80,
        .num_cycles = (duration_sec * 1000) / 10,
        .use_rt = use_rt
    };

    // Thread 3: Logger - Lowest priority (longest period)
    threads[2].config = (thread_config_t){
        .id = 2,
        .name = "Logger",
        .period_ms = 20,
        .computation_usec = 3000,
        .priority = 70,
        .num_cycles = (duration_sec * 1000) / 20,
        .use_rt = use_rt
    };

    // Allocate statistics arrays
    for (int i = 0; i < 3; i++) {
        threads[i].stats = calloc(threads[i].config.num_cycles, sizeof(cycle_stat_t));
        threads[i].missed_count = 0;
        threads[i].min_latency = LONG_MAX;
        threads[i].max_latency = 0;
        threads[i].total_latency = 0;
        threads[i].completed = 0;
    }
}

// Display configuration
void display_config(void) {
    printf("\n=== Thread Configuration ===\n");
    printf("Thread | Name       | Period | Computation | Utilization | Priority\n");
    printf("-------|------------|--------|-------------|-------------|----------\n");

    double total_utilization = 0.0;
    for (int i = 0; i < 3; i++) {
        double utilization = (threads[i].config.computation_usec / 1000.0) / threads[i].config.period_ms * 100.0;
        total_utilization += utilization / 100.0;

        printf("  %d    | %-10s | %4d ms |    %4d µs  |    %5.1f%%   |    %2d\n",
               threads[i].config.id,
               threads[i].config.name,
               threads[i].config.period_ms,
               threads[i].config.computation_usec,
               utilization,
               threads[i].config.priority);
    }

    printf("\nTotal Utilization: %.1f%%\n", total_utilization * 100.0);

    // RMS schedulability bound for 3 tasks: U ≤ 3(2^(1/3) - 1) ≈ 0.78
    double rms_bound = 3 * (pow(2.0, 1.0/3.0) - 1.0);
    printf("RMS Schedulability Bound: %.1f%%\n", rms_bound * 100.0);

    if (total_utilization <= rms_bound) {
        printf("✓ System is schedulable under RMS\n");
    } else if (total_utilization <= 1.0) {
        printf("⚠ Utilization > RMS bound but < 100%% (may still be schedulable)\n");
    } else {
        printf("✗ System is NOT schedulable (utilization > 100%%)\n");
    }
}

// Display results
void display_results(void) {
    printf("\n=== Results Summary ===\n\n");

    printf("Thread | Name       | Cycles | Misses | Miss %% | Min Lat | Max Lat | Avg Lat | Jitter\n");
    printf("-------|------------|--------|--------|---------|---------|---------|---------|--------\n");

    for (int i = 0; i < 3; i++) {
        double miss_percent = (threads[i].missed_count * 100.0) / threads[i].config.num_cycles;
        long avg_latency = threads[i].total_latency / threads[i].config.num_cycles;
        long jitter = threads[i].max_latency - threads[i].min_latency;

        printf("  %d    | %-10s | %6d | %6d | %6.2f%% | %6ld µs| %6ld µs| %6ld µs| %5ld µs\n",
               threads[i].config.id,
               threads[i].config.name,
               threads[i].config.num_cycles,
               threads[i].missed_count,
               miss_percent,
               threads[i].min_latency / 1000,
               threads[i].max_latency / 1000,
               avg_latency / 1000,
               jitter / 1000);
    }

    printf("\n=== Analysis ===\n");

    int total_misses = 0;
    for (int i = 0; i < 3; i++) {
        total_misses += threads[i].missed_count;
    }

    if (total_misses == 0) {
        printf("✓ No deadline misses - All tasks met their deadlines\n");
    } else {
        printf("✗ Total deadline misses: %d\n", total_misses);
        printf("  Tasks with highest priority should miss least (RMS principle)\n");
    }

    printf("\n=== Shared Data ===\n");
    printf("Final sensor value: %d\n", shared_data.sensor_value);
    printf("Final control output: %d\n", shared_data.control_output);
    printf("Total log entries: %d\n", shared_data.log_counter);
}

int main(int argc, char *argv[]) {
    int duration_sec = DURATION_SEC;
    int use_rt = 0;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--rt") == 0) {
            use_rt = 1;
        } else if (strcmp(argv[i], "--duration") == 0 && i + 1 < argc) {
            duration_sec = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [OPTIONS]\n", argv[0]);
            printf("Options:\n");
            printf("  --rt              Use real-time scheduling (requires --privileged)\n");
            printf("  --duration SEC    Run duration in seconds (default: %d)\n", DURATION_SEC);
            printf("  --help            Show this help\n");
            return 0;
        }
    }

    printf("=== Sample 3-Thread Real-Time Application ===\n");
    printf("Duration: %d seconds\n", duration_sec);
    printf("Scheduling: %s\n", use_rt ? "SCHED_FIFO (Real-Time)" : "SCHED_OTHER (Normal)");

    // Setup signal handler
    signal(SIGINT, signal_handler);

    // Initialize configurations
    init_thread_configs(duration_sec, use_rt);
    display_config();

    printf("\n=== Starting Threads ===\n");

    // Create threads
    for (int i = 0; i < 3; i++) {
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        // Create thread
        if (pthread_create(&threads[i].handle, &attr, thread_functions[i], &threads[i]) != 0) {
            perror("pthread_create");
            return 1;
        }

        // Set RT scheduling if requested
        if (use_rt) {
            struct sched_param param;
            param.sched_priority = threads[i].config.priority;

            if (pthread_setschedparam(threads[i].handle, SCHED_FIFO, &param) != 0) {
                printf("Warning: Could not set RT priority for %s (need --privileged)\n",
                       threads[i].config.name);
                use_rt = 0;  // Fall back to normal scheduling
            } else {
                printf("✓ Thread %d (%s) started with priority %d\n",
                       i, threads[i].config.name, threads[i].config.priority);
            }
        } else {
            printf("✓ Thread %d (%s) started\n", i, threads[i].config.name);
        }

        pthread_attr_destroy(&attr);
    }

    printf("\nThreads running... (Press Ctrl+C to stop early)\n");

    // Wait for all threads to complete
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i].handle, NULL);
    }

    // Display results
    display_results();

    printf("\n=== Key Takeaways ===\n");
    printf("• Rate Monotonic Scheduling: Shorter period = Higher priority\n");
    printf("• Thread priorities: Sensor (90) > Controller (80) > Logger (70)\n");
    printf("• Higher priority threads should have lower latency and fewer misses\n");
    printf("• Total utilization must be ≤ RMS bound for guaranteed schedulability\n");

    printf("\n=== Experiments to Try ===\n");
    printf("1. Compare RT vs normal: ./sample_3threads vs ./sample_3threads --rt\n");
    printf("2. Add system load: stress-ng --cpu 4 & ./sample_3threads --rt\n");
    printf("3. Increase duration: ./sample_3threads --rt --duration 10\n");
    printf("4. Modify computation times in code and observe deadline misses\n");

    // Cleanup
    for (int i = 0; i < 3; i++) {
        free(threads[i].stats);
    }

    return 0;
}
