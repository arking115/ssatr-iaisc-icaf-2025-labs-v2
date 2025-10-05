/*
 * Exercise 7: Deadlock Prevention with Lock Ordering
 *
 * This exercise demonstrates how to prevent deadlocks by enforcing
 * a strict global lock ordering discipline.
 *
 * Concepts:
 * - Lock ordering: always acquire locks in same order
 * - Breaking circular wait condition
 * - Prevention vs detection
 * - Resource hierarchy
 *
 * Solution:
 * - Task A: locks mutex1 → mutex2 (same order)
 * - Task B: locks mutex1 → mutex2 (SAME ORDER!)
 * - Result: No circular wait possible → NO DEADLOCK!
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NSEC_PER_SEC 1000000000L
#define NSEC_PER_MSEC 1000000L

// Two mutexes with defined ordering
pthread_mutex_t mutex1;  // Lock order: 1
pthread_mutex_t mutex2;  // Lock order: 2

// Shared resources
int resource1 = 0;
int resource2 = 0;

// Statistics
struct timespec start_time;
struct timespec task_a_start, task_a_end;
struct timespec task_b_start, task_b_end;
volatile int task_a_completed = 0;
volatile int task_b_completed = 0;

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

// Calculate time difference in ms
double timespec_diff_ms(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * 1000.0) + (diff_nsec / 1000000.0);
}

// Task A: Acquires locks in ORDER (mutex1 → mutex2)
void* task_a(void *arg) {
    (void)arg;

    log_event("TASK-A", "Started");
    clock_gettime(CLOCK_MONOTONIC, &task_a_start);

    // Lock ordering: ALWAYS acquire mutex1 first, then mutex2
    log_event("TASK-A", "Locking mutex1 (order: 1)...");
    pthread_mutex_lock(&mutex1);
    log_event("TASK-A", "✓ Acquired mutex1");

    // Small delay to allow Task B to run
    usleep(100000);  // 100ms

    log_event("TASK-A", "Locking mutex2 (order: 2)...");
    pthread_mutex_lock(&mutex2);
    log_event("TASK-A", "✓ Acquired mutex2");

    // Critical section - access both resources
    log_event("TASK-A", "Accessing shared resources...");
    resource1 += 10;
    resource2 += 20;
    usleep(50000);  // Simulate work

    log_event("TASK-A", "Work completed, releasing locks...");

    // Release in reverse order (good practice)
    pthread_mutex_unlock(&mutex2);
    log_event("TASK-A", "Released mutex2");

    pthread_mutex_unlock(&mutex1);
    log_event("TASK-A", "Released mutex1");

    clock_gettime(CLOCK_MONOTONIC, &task_a_end);
    task_a_completed = 1;
    log_event("TASK-A", "✓ Completed successfully");

    return NULL;
}

// Task B: Acquires locks in SAME ORDER (mutex1 → mutex2)
void* task_b(void *arg) {
    (void)arg;

    log_event("TASK-B", "Started");
    clock_gettime(CLOCK_MONOTONIC, &task_b_start);

    // Small delay to create potential conflict
    usleep(50000);  // 50ms

    // Lock ordering: SAME as Task A (mutex1 first, then mutex2)
    log_event("TASK-B", "Locking mutex1 (order: 1)...");
    pthread_mutex_lock(&mutex1);
    log_event("TASK-B", "✓ Acquired mutex1");

    usleep(100000);  // 100ms

    log_event("TASK-B", "Locking mutex2 (order: 2)...");
    pthread_mutex_lock(&mutex2);
    log_event("TASK-B", "✓ Acquired mutex2");

    // Critical section - access both resources
    log_event("TASK-B", "Accessing shared resources...");
    resource1 += 100;
    resource2 += 200;
    usleep(50000);  // Simulate work

    log_event("TASK-B", "Work completed, releasing locks...");

    // Release in reverse order
    pthread_mutex_unlock(&mutex2);
    log_event("TASK-B", "Released mutex2");

    pthread_mutex_unlock(&mutex1);
    log_event("TASK-B", "Released mutex1");

    clock_gettime(CLOCK_MONOTONIC, &task_b_end);
    task_b_completed = 1;
    log_event("TASK-B", "✓ Completed successfully");

    return NULL;
}

int main(void) {
    pthread_t thread_a, thread_b;
    struct timespec program_start, program_end;

    printf("=== Exercise 7: Deadlock Prevention with Lock Ordering ===\n\n");
    printf("This exercise prevents deadlock using strict lock ordering:\n");
    printf("  • Task A: locks mutex1 → mutex2 (order 1→2)\n");
    printf("  • Task B: locks mutex1 → mutex2 (SAME order 1→2)\n");
    printf("  • Result: No circular wait → NO DEADLOCK!\n\n");
    printf("Lock Ordering Rule: Always acquire locks in increasing order\n");
    printf("  mutex1 (order 1) must be locked before mutex2 (order 2)\n\n");

    // Initialize mutexes
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    printf("Starting tasks...\n\n");
    clock_gettime(CLOCK_MONOTONIC, &program_start);

    // Create both tasks
    pthread_create(&thread_a, NULL, task_a, NULL);
    pthread_create(&thread_b, NULL, task_b, NULL);

    // Wait for completion
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    clock_gettime(CLOCK_MONOTONIC, &program_end);

    // Analysis
    printf("\n=== Analysis ===\n");

    if (task_a_completed && task_b_completed) {
        printf("✓ BOTH TASKS COMPLETED SUCCESSFULLY!\n");
        printf("✓ NO DEADLOCK occurred\n\n");

        double task_a_time = timespec_diff_ms(&task_a_start, &task_a_end);
        double task_b_time = timespec_diff_ms(&task_b_start, &task_b_end);
        double total_time = timespec_diff_ms(&program_start, &program_end);

        printf("Execution times:\n");
        printf("  Task A: %.1f ms\n", task_a_time);
        printf("  Task B: %.1f ms\n", task_b_time);
        printf("  Total:  %.1f ms\n", total_time);

        printf("\nWhy no deadlock?\n");
        printf("  • Both tasks acquire locks in SAME order\n");
        printf("  • Task that gets mutex1 first will complete fully\n");
        printf("  • Other task waits, then proceeds when locks free\n");
        printf("  • No circular wait condition possible\n");
    } else {
        printf("✗ Unexpected: Task didn't complete\n");
    }

    printf("\nFinal resource values:\n");
    printf("  Resource 1: %d (expected: 110)\n", resource1);
    printf("  Resource 2: %d (expected: 220)\n", resource2);

    // Lock ordering explanation
    printf("\n=== Lock Ordering Discipline ===\n");
    printf("Rule: Define a global order for all locks\n");
    printf("  mutex1 = order 1\n");
    printf("  mutex2 = order 2\n");
    printf("  mutex3 = order 3 (if exists)\n");
    printf("  ...\n\n");

    printf("Enforcement: Always acquire in increasing order:\n");
    printf("  ✓ Correct: lock(1) → lock(2) → lock(3)\n");
    printf("  ✓ Correct: lock(1) → lock(3) (skip 2 is OK)\n");
    printf("  ✗ Wrong:   lock(2) → lock(1) (violates order!)\n");
    printf("  ✗ Wrong:   lock(3) → lock(2) → lock(1)\n\n");

    printf("Breaking circular wait:\n");
    printf("  • Circular wait needs: A waits for B, B waits for A\n");
    printf("  • With lock ordering: A and B both lock in same order\n");
    printf("  • One gets first lock, other waits (no cycle!)\n");

    // Comparison with Exercise 6
    printf("\n=== Comparison: Exercise 6 vs Exercise 7 ===\n");
    printf("\n");
    printf("Exercise 6 (Detection):\n");
    printf("  • Opposite lock ordering\n");
    printf("  • Deadlock occurred\n");
    printf("  • Timeout detection needed\n");
    printf("  • Recovery with retry\n");
    printf("  • Multiple attempts possible\n");
    printf("  • Unpredictable execution time\n\n");

    printf("Exercise 7 (Prevention):\n");
    printf("  • Same lock ordering\n");
    printf("  • No deadlock possible\n");
    printf("  • No timeout needed\n");
    printf("  • No recovery needed\n");
    printf("  • Completes in one attempt\n");
    printf("  • Predictable execution time\n\n");

    printf("✓ Prevention is BETTER than detection!\n");

    // Best practices
    printf("\n=== Best Practices for Deadlock-Free Code ===\n");
    printf("1. Lock Ordering:\n");
    printf("   • Define global order for all locks\n");
    printf("   • Document the order clearly\n");
    printf("   • Enforce with code reviews\n\n");

    printf("2. Lock Hierarchy:\n");
    printf("   • Assign numeric levels to locks\n");
    printf("   • Always lock lower level before higher\n");
    printf("   • Use assertions to verify order\n\n");

    printf("3. Minimize Lock Scope:\n");
    printf("   • Hold locks for minimum time\n");
    printf("   • Release as soon as possible\n");
    printf("   • Reduce contention probability\n\n");

    printf("4. Avoid Nested Locks:\n");
    printf("   • Try to use only one lock at a time\n");
    printf("   • Redesign to eliminate multi-lock needs\n");
    printf("   • Use lock-free algorithms where possible\n\n");

    printf("5. Use Try-Lock:\n");
    printf("   • pthread_mutex_trylock() for non-blocking\n");
    printf("   • Release and retry if can't get all locks\n");
    printf("   • Fallback when ordering not possible\n");

    // Real-world patterns
    printf("\n=== Real-World Patterns ===\n");
    printf("• Database transactions: Lock rows in primary key order\n");
    printf("• Network stacks: Protocol layers have defined order\n");
    printf("• File systems: Inode locks ordered by inode number\n");
    printf("• Memory allocators: Address-ordered locking\n");

    printf("\n=== Advanced Topics ===\n");
    printf("• Banker's algorithm: Resource allocation safety\n");
    printf("• Wait-die / Wound-wait: Timestamp-based prevention\n");
    printf("• Lock-free data structures: Avoid locks entirely\n");
    printf("• Two-phase locking: Database deadlock prevention\n");

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
