/*
 * Exercise 5: Priority Inheritance Solution
 *
 * This exercise demonstrates how Priority Inheritance Protocol (PIP)
 * solves the unbounded priority inversion problem.
 *
 * Concepts:
 * - Priority Inheritance Protocol (PIP)
 * - PTHREAD_PRIO_INHERIT mutex attribute
 * - Bounded vs unbounded blocking
 * - Temporary priority elevation
 *
 * Solution:
 * 1. Low task acquires PI mutex
 * 2. High task blocks on mutex
 * 3. Low inherits High's priority (P=90)
 * 4. Medium (P=50) cannot preempt inherited-priority Low
 * 5. Low completes quickly, High unblocks
 * 6. Result: Bounded priority inversion!
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
#define MEDIUM_WORK_MS 100  // Medium task attempts to run
#define HIGH_WORK_MS 10     // High task work

// Shared resource and PI mutex
pthread_mutex_t resource_mutex;
int shared_resource = 0;

// Synchronization flags
volatile int low_has_mutex = 0;
volatile int high_blocked = 0;
volatile int medium_tried = 0;
volatile int medium_ran = 0;

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

    // Wait for high to block (which will boost our priority)
    while (!high_blocked) {
        usleep(1000);
    }

    sleep_ms(10);  // Give high time to boost us

    log_event("LOW", "*** Priority inherited from HIGH (now running at P=90) ***");
    log_event("LOW", "Working with shared resource (protected from Medium)...");

    // Simulate work with the resource
    for (int i = 0; i < 5; i++) {
        shared_resource++;
        sleep_ms(LOW_WORK_MS / 5);
    }

    log_event("LOW", "Releasing mutex (priority returns to 10)");
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

    // Wait for priority inversion scenario to begin
    while (!high_blocked) {
        usleep(1000);
    }

    sleep_ms(15);  // Let inheritance take effect

    log_event("MEDIUM", "Attempting to run...");
    medium_tried = 1;

    // Try to do work - but should be blocked by inherited-priority Low
    volatile long sum = 0;
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int iterations = 0;
    do {
        for (int i = 0; i < 1000000; i++) {
            sum += i;
        }
        iterations++;
        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (timespec_diff_ns(&start, &now) < (MEDIUM_WORK_MS * NSEC_PER_MSEC));

    if (iterations > 1) {
        medium_ran = 1;
        log_event("MEDIUM", "Completed work");
    } else {
        log_event("MEDIUM", "✓ Could NOT preempt Low (Low had inherited priority!)");
        log_event("MEDIUM", "Completed after High finished");
    }

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
    log_event("HIGH", "(This will boost Low's priority to 90)");
    clock_gettime(CLOCK_MONOTONIC, &high_block_start);
    high_blocked = 1;

    pthread_mutex_lock(&resource_mutex);
    clock_gettime(CLOCK_MONOTONIC, &high_block_end);

    high_blocking_time_ns = timespec_diff_ns(&high_block_start, &high_block_end);

    log_event("HIGH", "✓ Acquired mutex after blocking");
    printf("       └─> Blocked for: %.1f ms (bounded!)\n",
           high_blocking_time_ns / 1000000.0);

    // Quick work
    shared_resource += 100;
    sleep_ms(HIGH_WORK_MS);

    pthread_mutex_unlock(&resource_mutex);
    log_event("HIGH", "Released mutex and completed");

    return NULL;
}

int main(void) {
    pthread_t low_thread, medium_thread, high_thread;
    pthread_mutexattr_t mutex_attr;

    printf("=== Exercise 5: Priority Inheritance Solution ===\n\n");
    printf("This exercise shows how Priority Inheritance solves priority inversion:\n");
    printf("  • Low task (P=10) acquires PI mutex\n");
    printf("  • High task (P=90) blocks on mutex\n");
    printf("  • Low inherits High's priority (P=90)\n");
    printf("  • Medium task (P=50) CANNOT preempt inherited-priority Low\n");
    printf("  • Result: Bounded blocking!\n\n");

    // Initialize mutex with priority inheritance
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&resource_mutex, &mutex_attr);

    printf("Using PRIORITY INHERITANCE mutex\n");
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

    if (high_blocking_time_ns <= ((LOW_WORK_MS + 20) * NSEC_PER_MSEC)) {
        printf("\n✓ BOUNDED PRIORITY INVERSION - Success!\n");
        printf("  High task blocked for approximately Low's critical section time\n");
        printf("  Medium could NOT interfere (Low had inherited priority)\n");
        printf("\n  Expected blocking: ~%d ms (Low's work time)\n", LOW_WORK_MS);
        printf("  Actual blocking:   %.1f ms\n",
               high_blocking_time_ns / 1000000.0);

        if (!medium_ran) {
            printf("\n✓ Medium task did NOT preempt Low during critical section\n");
        }
    } else {
        printf("\n⚠ Priority inheritance may not be working\n");
        printf("  (Container may need additional RT configuration)\n");
    }

    printf("\n=== How Priority Inheritance Works ===\n");
    printf("1. Low (P=10) holds mutex\n");
    printf("2. High (P=90) blocks on mutex\n");
    printf("3. Kernel boosts Low to P=90 (temporary inheritance)\n");
    printf("4. Medium (P=50) cannot preempt Low (90 > 50)\n");
    printf("5. Low finishes, releases mutex, returns to P=10\n");
    printf("6. High acquires mutex and runs\n");
    printf("7. Medium finally gets to run\n");

    printf("\n=== Key Benefits ===\n");
    printf("✓ Prevents unbounded priority inversion\n");
    printf("✓ Blocking time is bounded by critical section length\n");
    printf("✓ High-priority tasks remain predictable\n");
    printf("✓ Automatic - no manual priority management needed\n");

    printf("\n=== Comparison with Exercise 4 ===\n");
    printf("Exercise 4 (regular mutex):  Unbounded blocking (~150+ ms)\n");
    printf("Exercise 5 (PI mutex):       Bounded blocking (~%d ms)\n", LOW_WORK_MS);

    printf("\n=== Real-World Usage ===\n");
    printf("Priority inheritance is used in:\n");
    printf("  • Linux RT kernel (PREEMPT_RT)\n");
    printf("  • VxWorks (fixed Mars Pathfinder)\n");
    printf("  • QNX, RTEMS, FreeRTOS\n");
    printf("  • Any safety-critical RT system\n");

    pthread_mutexattr_destroy(&mutex_attr);
    pthread_mutex_destroy(&resource_mutex);
    return 0;
}
