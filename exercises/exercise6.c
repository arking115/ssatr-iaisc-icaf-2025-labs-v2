/*
 * Exercise 6: Deadlock Demonstration & Detection
 *
 * This exercise demonstrates the classic deadlock problem where two tasks
 * create a circular wait condition by acquiring locks in opposite order.
 *
 * Concepts:
 * - Deadlock: circular wait for resources
 * - Four conditions for deadlock (Coffman conditions)
 * - Timeout-based deadlock detection
 * - Recovery strategies
 *
 * Scenario:
 * - Task A: locks mutex1, then tries mutex2
 * - Task B: locks mutex2, then tries mutex1
 * - Result: Circular wait → DEADLOCK!
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define NSEC_PER_SEC 1000000000L
#define NSEC_PER_MSEC 1000000L
#define DEADLOCK_TIMEOUT_MS 1000  // 1 second timeout

// Two mutexes that will cause deadlock
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

// Shared resources
int resource1 = 0;
int resource2 = 0;

// Deadlock detection flags
volatile int task_a_has_mutex1 = 0;
volatile int task_b_has_mutex2 = 0;
volatile int deadlock_detected = 0;
volatile int task_a_completed = 0;
volatile int task_b_completed = 0;

// Statistics
int task_a_attempts = 0;
int task_b_attempts = 0;

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

// Calculate absolute timeout
void calc_timeout(struct timespec *ts, int timeout_ms) {
    clock_gettime(CLOCK_REALTIME, ts);
    ts->tv_sec += timeout_ms / 1000;
    ts->tv_nsec += (timeout_ms % 1000) * NSEC_PER_MSEC;
    if (ts->tv_nsec >= NSEC_PER_SEC) {
        ts->tv_sec++;
        ts->tv_nsec -= NSEC_PER_SEC;
    }
}

// Task A: Acquires mutex1 first, then mutex2
void* task_a(void *arg) {
    (void)arg;
    struct timespec timeout;
    int result;

    log_event("TASK-A", "Started");

    while (!task_a_completed && task_a_attempts < 3) {
        task_a_attempts++;
        log_event("TASK-A", "Attempting to acquire resources...");

        // Acquire mutex1
        log_event("TASK-A", "Locking mutex1...");
        pthread_mutex_lock(&mutex1);
        task_a_has_mutex1 = 1;
        log_event("TASK-A", "✓ Acquired mutex1");

        // Small delay to increase chance of deadlock
        usleep(100000);  // 100ms

        // Try to acquire mutex2 with timeout
        log_event("TASK-A", "Trying to lock mutex2...");
        calc_timeout(&timeout, DEADLOCK_TIMEOUT_MS);
        result = pthread_mutex_timedlock(&mutex2, &timeout);

        if (result == ETIMEDOUT) {
            log_event("TASK-A", "✗ TIMEOUT waiting for mutex2!");
            log_event("TASK-A", "*** DEADLOCK DETECTED ***");
            deadlock_detected = 1;

            // Release mutex1 and retry
            pthread_mutex_unlock(&mutex1);
            task_a_has_mutex1 = 0;
            log_event("TASK-A", "Released mutex1, will retry...");

            sleep(1);  // Back off before retry
            continue;
        }

        // Successfully acquired both mutexes
        log_event("TASK-A", "✓ Acquired mutex2");

        // Critical section - access both resources
        log_event("TASK-A", "Accessing shared resources...");
        resource1 += 10;
        resource2 += 20;
        usleep(50000);  // Simulate work

        // Release mutexes
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
        task_a_has_mutex1 = 0;
        log_event("TASK-A", "Released both mutexes");

        task_a_completed = 1;
        log_event("TASK-A", "✓ Completed successfully");
        break;
    }

    if (!task_a_completed) {
        log_event("TASK-A", "✗ Failed after max retries");
    }

    return NULL;
}

// Task B: Acquires mutex2 first, then mutex1 (OPPOSITE ORDER!)
void* task_b(void *arg) {
    (void)arg;
    struct timespec timeout;
    int result;

    log_event("TASK-B", "Started");

    // Small delay to let Task A start
    usleep(50000);  // 50ms

    while (!task_b_completed && task_b_attempts < 3) {
        task_b_attempts++;
        log_event("TASK-B", "Attempting to acquire resources...");

        // Acquire mutex2 (OPPOSITE ORDER from Task A)
        log_event("TASK-B", "Locking mutex2...");
        pthread_mutex_lock(&mutex2);
        task_b_has_mutex2 = 1;
        log_event("TASK-B", "✓ Acquired mutex2");

        // Small delay to increase chance of deadlock
        usleep(100000);  // 100ms

        // Try to acquire mutex1 with timeout
        log_event("TASK-B", "Trying to lock mutex1...");
        calc_timeout(&timeout, DEADLOCK_TIMEOUT_MS);
        result = pthread_mutex_timedlock(&mutex1, &timeout);

        if (result == ETIMEDOUT) {
            log_event("TASK-B", "✗ TIMEOUT waiting for mutex1!");
            log_event("TASK-B", "*** DEADLOCK DETECTED ***");
            deadlock_detected = 1;

            // Release mutex2 and retry
            pthread_mutex_unlock(&mutex2);
            task_b_has_mutex2 = 0;
            log_event("TASK-B", "Released mutex2, will retry...");

            sleep(1);  // Back off before retry
            continue;
        }

        // Successfully acquired both mutexes
        log_event("TASK-B", "✓ Acquired mutex1");

        // Critical section - access both resources
        log_event("TASK-B", "Accessing shared resources...");
        resource1 += 100;
        resource2 += 200;
        usleep(50000);  // Simulate work

        // Release mutexes
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);
        task_b_has_mutex2 = 0;
        log_event("TASK-B", "Released both mutexes");

        task_b_completed = 1;
        log_event("TASK-B", "✓ Completed successfully");
        break;
    }

    if (!task_b_completed) {
        log_event("TASK-B", "✗ Failed after max retries");
    }

    return NULL;
}

int main(void) {
    pthread_t thread_a, thread_b;

    printf("=== Exercise 6: Deadlock Demonstration & Detection ===\n\n");
    printf("This exercise demonstrates deadlock with opposite lock ordering:\n");
    printf("  • Task A: locks mutex1 → tries mutex2\n");
    printf("  • Task B: locks mutex2 → tries mutex1\n");
    printf("  • Result: Circular wait → DEADLOCK!\n\n");
    printf("Detection method: timeout-based using pthread_mutex_timedlock()\n");
    printf("Timeout: %d ms\n\n", DEADLOCK_TIMEOUT_MS);

    // Initialize mutexes
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    printf("Starting tasks...\n\n");

    // Create both tasks
    pthread_create(&thread_a, NULL, task_a, NULL);
    pthread_create(&thread_b, NULL, task_b, NULL);

    // Wait for completion
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    // Analysis
    printf("\n=== Analysis ===\n");

    if (deadlock_detected) {
        printf("✗ DEADLOCK WAS DETECTED!\n\n");
        printf("Deadlock occurred because:\n");
        printf("  1. Task A held mutex1, waiting for mutex2\n");
        printf("  2. Task B held mutex2, waiting for mutex1\n");
        printf("  3. Circular wait condition created\n");
        printf("  4. Neither task could proceed\n\n");

        printf("Recovery actions taken:\n");
        printf("  • Timeout detected the deadlock\n");
        printf("  • Tasks released locks and retried\n");
        printf("  • Eventual completion after retry\n");
    } else {
        printf("⚠ No deadlock occurred (timing was lucky!)\n");
        printf("  Try running again - deadlock is timing-dependent\n");
    }

    printf("\nAttempt statistics:\n");
    printf("  Task A attempts: %d\n", task_a_attempts);
    printf("  Task B attempts: %d\n", task_b_attempts);
    printf("  Task A completed: %s\n", task_a_completed ? "YES" : "NO");
    printf("  Task B completed: %s\n", task_b_completed ? "YES" : "NO");

    printf("\nFinal resource values:\n");
    printf("  Resource 1: %d\n", resource1);
    printf("  Resource 2: %d\n", resource2);

    // Four Coffman conditions for deadlock
    printf("\n=== Four Coffman Conditions for Deadlock ===\n");
    printf("All four must be present for deadlock:\n\n");
    printf("1. Mutual Exclusion:\n");
    printf("   ✓ Mutexes provide exclusive access\n\n");
    printf("2. Hold and Wait:\n");
    printf("   ✓ Task holds one mutex while waiting for another\n\n");
    printf("3. No Preemption:\n");
    printf("   ✓ Mutexes cannot be forcibly taken away\n\n");
    printf("4. Circular Wait:\n");
    printf("   ✓ Task A → mutex1 → mutex2 (held by B)\n");
    printf("   ✓ Task B → mutex2 → mutex1 (held by A)\n");

    printf("\n=== Deadlock Detection Methods ===\n");
    printf("1. Timeout-based (this exercise):\n");
    printf("   • Use pthread_mutex_timedlock()\n");
    printf("   • Simple but may have false positives\n");
    printf("   • Good for recovery attempts\n\n");

    printf("2. Resource allocation graph:\n");
    printf("   • Track lock dependencies\n");
    printf("   • Detect cycles in wait-for graph\n");
    printf("   • More accurate but complex\n\n");

    printf("3. Watchdog timers:\n");
    printf("   • External monitoring\n");
    printf("   • System-level detection\n");
    printf("   • Can force recovery or restart\n");

    printf("\n=== Problems with Deadlock Detection ===\n");
    printf("✗ Overhead of timeout checking\n");
    printf("✗ Difficult to choose correct timeout value\n");
    printf("✗ Recovery is complex and error-prone\n");
    printf("✗ May not work in all scenarios\n");

    printf("\n💡 Better solution: PREVENT deadlock (see exercise7)\n");

    printf("\n=== Real-World Examples ===\n");
    printf("• Database deadlocks: Transaction A and B lock different rows\n");
    printf("• Network protocols: Two nodes wait for each other's response\n");
    printf("• Operating systems: Process resource allocation\n");

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
