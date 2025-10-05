/*
 * Exercise 4: Priority Inversion Demonstration
 *
 * This exercise demonstrates the classic priority inversion problem where
 * a high-priority task is indirectly blocked by a medium-priority task.
 *
 * Concepts:
 * - Priority inversion: High blocked by Low, preempted by Medium
 * - Unbounded blocking: Medium can run indefinitely
 * - Impact on real-time guarantees
 * - The need for priority inheritance
 *
 * Scenario:
 * 1. Low-priority task acquires mutex
 * 2. High-priority task blocks on mutex (wants resource)
 * 3. Medium-priority task preempts Low
 * 4. Result: High is blocked while Medium runs (priority inversion!)
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <limits.h>

#define NSEC_PER_SEC 1000000000L
#define NSEC_PER_MSEC 1000000L

// Task priorities
#define PRIO_HIGH 90
#define PRIO_MEDIUM 50
#define PRIO_LOW 10

// Timing configuration
#define LOW_WORK_MS 50      // Low task holds mutex for 50ms
#define MEDIUM_WORK_MS 100  // Medium task runs for 100ms
#define HIGH_WORK_MS 10     // High task work is quick

// Shared resource and mutex
pthread_mutex_t resource_mutex;
int shared_resource = 0;

// Synchronization flags
volatile int low_has_mutex = 0;
volatile int high_blocked = 0;
volatile int medium_started = 0;

// Statistics
struct timespec high_block_start, high_block_end;
long high_blocking_time_ns = 0;

// Helper function to get thread ID
pid_t gettid(void) {
    return syscall(SYS_gettid);
}

// Calculate time difference in nanoseconds
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * NSEC_PER_SEC) + diff_nsec;
}

// Sleep for milliseconds
void sleep_ms(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * NSEC_PER_MSEC;
    nanosleep(&ts, NULL);
}

// Print with timestamp
void log_event(const char *task, const char *event) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    printf("[%3ld.%03ld] %s: %s\n",
           now.tv_sec % 1000,
           now.tv_nsec / NSEC_PER_MSEC,
           task, event);
    fflush(stdout);
}

// Set thread priority
int set_thread_priority(int policy, int priority) {
    struct sched_param param;
    param.sched_priority = priority;

    if (sched_setscheduler(0, policy, &param) != 0) {
        perror("sched_setscheduler");
        return -1;
    }
    return 0;
}

// Low-priority task
void* low_priority_task(void *arg) {
    (void)arg;

    set_thread_priority(SCHED_FIFO, PRIO_LOW);
    log_event("LOW", "Started (priority 10)");

    sleep_ms(10);  // Let high task get ready

    log_event("LOW", "Acquiring mutex...");
    pthread_mutex_lock(&resource_mutex);
    low_has_mutex = 1;
    log_event("LOW", "✓ Acquired mutex");

    // Simulate work with the resource
    log_event("LOW", "Working with shared resource...");
    for (int i = 0; i < 5; i++) {
        shared_resource++;
        sleep_ms(LOW_WORK_MS / 5);

        // Check if we're being preempted by medium
        if (medium_started && i < 4) {
            log_event("LOW", "⚠ Preempted by medium-priority task!");
        }
    }

    log_event("LOW", "Releasing mutex");
    pthread_mutex_unlock(&resource_mutex);
    low_has_mutex = 0;

    log_event("LOW", "Completed");
    return NULL;
}

// Medium-priority task
void* medium_priority_task(void *arg) {
    (void)arg;

    set_thread_priority(SCHED_FIFO, PRIO_MEDIUM);
    log_event("MEDIUM", "Started (priority 50)");

    // Wait for priority inversion to begin
    while (!high_blocked) {
        usleep(1000);
    }

    sleep_ms(5);  // Small delay to ensure high is blocked

    log_event("MEDIUM", "*** STARTING WORK (causing priority inversion) ***");
    medium_started = 1;

    // Do work - this blocks the high-priority task indirectly
    volatile long sum = 0;
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    do {
        for (int i = 0; i < 1000000; i++) {
            sum += i;
        }
        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (timespec_diff_ns(&start, &now) < (MEDIUM_WORK_MS * NSEC_PER_MSEC));

    log_event("MEDIUM", "Completed (high task was blocked this entire time!)");
    return NULL;
}

// High-priority task
void* high_priority_task(void *arg) {
    (void)arg;

    set_thread_priority(SCHED_FIFO, PRIO_HIGH);
    log_event("HIGH", "Started (priority 90)");

    // Wait for low to acquire mutex
    while (!low_has_mutex) {
        usleep(1000);
    }

    sleep_ms(20);  // Ensure low is working

    log_event("HIGH", "Need shared resource - trying to acquire mutex...");
    clock_gettime(CLOCK_MONOTONIC, &high_block_start);
    high_blocked = 1;

    pthread_mutex_lock(&resource_mutex);
    clock_gettime(CLOCK_MONOTONIC, &high_block_end);

    high_blocking_time_ns = timespec_diff_ns(&high_block_start, &high_block_end);

    log_event("HIGH", "✓ Acquired mutex after blocking");
    printf("       └─> Blocked for: %.1f ms\n", high_blocking_time_ns / 1000000.0);

    // Quick work
    shared_resource += 100;
    sleep_ms(HIGH_WORK_MS);

    pthread_mutex_unlock(&resource_mutex);
    log_event("HIGH", "Released mutex and completed");

    return NULL;
}

int main(void) {
    pthread_t low_thread, medium_thread, high_thread;

    printf("=== Exercise 4: Priority Inversion Demonstration ===\n\n");
    printf("This exercise shows unbounded priority inversion:\n");
    printf("  • Low task (P=10) acquires mutex\n");
    printf("  • High task (P=90) blocks on mutex\n");
    printf("  • Medium task (P=50) preempts Low\n");
    printf("  • Result: High blocked while Medium runs!\n\n");

    // Initialize mutex (regular, no priority inheritance)
    pthread_mutex_init(&resource_mutex, NULL);

    printf("Using REGULAR mutex (no priority inheritance)\n");
    printf("Starting tasks...\n\n");

    // Create threads in order: Low, High, Medium
    pthread_create(&low_thread, NULL, low_priority_task, NULL);
    sleep_ms(5);
    pthread_create(&high_thread, NULL, high_priority_task, NULL);
    sleep_ms(5);
    pthread_create(&medium_thread, NULL, medium_priority_task, NULL);

    // Wait for completion
    pthread_join(low_thread, NULL);
    pthread_join(medium_thread, NULL);
    pthread_join(high_thread, NULL);

    // Analysis
    printf("\n=== Analysis ===\n");
    printf("High-priority task blocking time: %.1f ms\n",
           high_blocking_time_ns / 1000000.0);

    if (high_blocking_time_ns > (LOW_WORK_MS * NSEC_PER_MSEC)) {
        printf("\n✗ UNBOUNDED PRIORITY INVERSION DETECTED!\n");
        printf("  High task blocked for longer than Low's critical section\n");
        printf("  This is because Medium preempted Low while Low held the mutex\n");
        printf("\n  Expected blocking: ~%d ms (Low's work time)\n", LOW_WORK_MS);
        printf("  Actual blocking:   %.1f ms (includes Medium's interference)\n",
               high_blocking_time_ns / 1000000.0);
    }

    printf("\n=== Problem Summary ===\n");
    printf("Priority inversion causes:\n");
    printf("  ✗ Unbounded blocking of high-priority tasks\n");
    printf("  ✗ Deadline misses and unpredictable behavior\n");
    printf("  ✗ Lower-priority work delays critical tasks\n");
    printf("\n💡 Solution: Use Priority Inheritance Protocol (see exercise5)\n");

    printf("\n=== Famous Example ===\n");
    printf("Mars Pathfinder (1997): Priority inversion caused system resets\n");
    printf("  • High-priority bus task blocked by low-priority meteorological task\n");
    printf("  • Medium-priority communication task caused inversion\n");
    printf("  • Fixed by enabling priority inheritance in VxWorks\n");

    pthread_mutex_destroy(&resource_mutex);
    return 0;
}
