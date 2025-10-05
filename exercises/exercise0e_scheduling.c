/*
 * Exercise 0E: Introduction to Scheduling Policies
 *
 * This exercise introduces Linux scheduling policies and priority management,
 * which are fundamental to real-time systems.
 *
 * Learning Objectives:
 * 1. Understand different scheduling policies (SCHED_OTHER, SCHED_FIFO, SCHED_RR)
 * 2. Check scheduling capabilities with sched_get_priority_max/min()
 * 3. Set scheduling policy and priority with sched_setscheduler()
 * 4. Handle permission errors gracefully
 * 5. Query current scheduling settings
 *
 * Key Concepts:
 * - SCHED_OTHER: Default time-sharing (normal processes)
 * - SCHED_FIFO: Real-time FIFO (runs until yields or blocks)
 * - SCHED_RR: Real-time Round-Robin (time-sliced RT)
 * - Priority ranges: 0 for SCHED_OTHER, 1-99 for RT policies
 * - struct sched_param: Contains priority field
 * - Requires CAP_SYS_NICE capability (root or --privileged container)
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/*
 * Convert scheduling policy number to human-readable name
 */
const char* policy_name(int policy) {
    switch(policy) {
        case SCHED_OTHER: return "SCHED_OTHER (normal)";
        case SCHED_FIFO:  return "SCHED_FIFO (RT)";
        case SCHED_RR:    return "SCHED_RR (RT)";
        case SCHED_BATCH: return "SCHED_BATCH";
        case SCHED_IDLE:  return "SCHED_IDLE";
        default:          return "UNKNOWN";
    }
}

/*
 * Display current scheduling policy and priority
 */
void show_current_scheduling(void) {
    int policy = sched_getscheduler(0);  // 0 = current process
    struct sched_param param;

    if (policy == -1) {
        perror("sched_getscheduler");
        return;
    }

    if (sched_getparam(0, &param) == -1) {
        perror("sched_getparam");
        return;
    }

    printf("  Policy:   %s\n", policy_name(policy));
    printf("  Priority: %d\n", param.sched_priority);
}

int main(void) {
    int policy;
    struct sched_param param;

    printf("=== Exercise 0E: Introduction to Scheduling Policies ===\n\n");

    /* Step 1: Check system capabilities */
    printf("Step 1: Checking system scheduling capabilities\n\n");

    printf("Scheduling Policy | Min Priority | Max Priority\n");
    printf("------------------|--------------|-------------\n");

    int policies[] = {SCHED_OTHER, SCHED_FIFO, SCHED_RR};
    const char *policy_names[] = {"SCHED_OTHER", "SCHED_FIFO", "SCHED_RR"};

    for (int i = 0; i < 3; i++) {
        int min_prio = sched_get_priority_min(policies[i]);
        int max_prio = sched_get_priority_max(policies[i]);

        if (min_prio == -1 || max_prio == -1) {
            printf("%-17s |      N/A     |     N/A\n", policy_names[i]);
        } else {
            printf("%-17s |      %2d      |     %2d\n",
                   policy_names[i], min_prio, max_prio);
        }
    }

    /* Step 2: Display current scheduling settings */
    printf("\n\nStep 2: Current scheduling settings\n");
    show_current_scheduling();

    /* Step 3: Attempt to set RT scheduling */
    printf("\n\nStep 3: Attempting to set SCHED_FIFO with priority 50\n");

    param.sched_priority = 50;
    policy = SCHED_FIFO;

    if (sched_setscheduler(0, policy, &param) == 0) {
        printf("  ✓ Successfully set SCHED_FIFO with priority 50\n");
        printf("\nNew scheduling settings:\n");
        show_current_scheduling();

        // Restore to normal scheduling
        printf("\n\nStep 4: Restoring normal scheduling (SCHED_OTHER)\n");
        param.sched_priority = 0;
        if (sched_setscheduler(0, SCHED_OTHER, &param) == 0) {
            printf("  ✓ Restored to SCHED_OTHER\n");
            printf("\nFinal scheduling settings:\n");
            show_current_scheduling();
        }
    } else {
        printf("  ✗ Failed to set SCHED_FIFO: %s\n", strerror(errno));

        if (errno == EPERM) {
            printf("\n  This is expected! RT scheduling requires elevated privileges.\n");
            printf("\n  To enable RT scheduling:\n");
            printf("    - In Docker: docker-compose run --rm --privileged rt-lab\n");
            printf("    - On Linux:  sudo ./exercise0e_scheduling\n");
            printf("               or: sudo setcap cap_sys_nice=ep ./exercise0e_scheduling\n");
        }

        printf("\n\nContinuing with normal scheduling:\n");
        show_current_scheduling();
    }

    /* Step 4: Demonstrate policy comparison */
    printf("\n\n=== Scheduling Policy Comparison ===\n\n");

    printf("SCHED_OTHER (Default, Time-Sharing):\n");
    printf("  • Normal processes (99%% of programs use this)\n");
    printf("  • Priority: always 0 (niceness controls weighting)\n");
    printf("  • Preemptible by other processes\n");
    printf("  • Fair sharing of CPU time\n");
    printf("  • CFS (Completely Fair Scheduler) in Linux\n\n");

    printf("SCHED_FIFO (Real-Time FIFO):\n");
    printf("  • Real-time policy\n");
    printf("  • Priority: 1-99 (higher = more important)\n");
    printf("  • Runs until it yields, blocks, or is preempted by higher priority\n");
    printf("  • No time slicing (can monopolize CPU!)\n");
    printf("  • Use for hard real-time tasks\n\n");

    printf("SCHED_RR (Real-Time Round-Robin):\n");
    printf("  • Real-time policy with time slicing\n");
    printf("  • Priority: 1-99\n");
    printf("  • Like SCHED_FIFO but with time quantum (typically 100ms)\n");
    printf("  • Multiple tasks at same priority share CPU\n");
    printf("  • Use when multiple RT tasks need fair scheduling\n\n");

    /* Step 5: Code examples */
    printf("=== Code Pattern for RT Scheduling ===\n\n");

    printf("// Set SCHED_FIFO with priority 80\n");
    printf("struct sched_param param;\n");
    printf("param.sched_priority = 80;\n");
    printf("\n");
    printf("if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {\n");
    printf("    perror(\"sched_setscheduler\");\n");
    printf("    // Handle error (maybe continue with SCHED_OTHER)\n");
    printf("}\n\n");

    /* Summary */
    printf("=== Key Takeaways ===\n");
    printf("✓ Three main policies: SCHED_OTHER (normal), SCHED_FIFO, SCHED_RR (RT)\n");
    printf("✓ RT policies require elevated privileges (CAP_SYS_NICE)\n");
    printf("✓ Priority range: 0 for normal, 1-99 for RT (99 = highest)\n");
    printf("✓ SCHED_FIFO runs until done; SCHED_RR adds time slicing\n");
    printf("✓ Always check return values and handle EPERM errors\n");
    printf("✓ Use sched_getscheduler() and sched_getparam() to query settings\n");

    printf("\n=== Safety Warning ===\n");
    printf("⚠ SCHED_FIFO at high priority can lock up your system!\n");
    printf("  • Always include sleep/yield in RT threads\n");
    printf("  • Keep priority < 99 (kernel uses 99)\n");
    printf("  • Test with lower priorities first (e.g., 50)\n");

    printf("\n=== Usage in RT Exercises ===\n");
    printf("Exercise 2: Compares SCHED_OTHER vs SCHED_FIFO vs SCHED_RR\n");
    printf("Exercise 3: Uses '--rt' flag to enable SCHED_FIFO for periodic task\n");
    printf("All exercises check capabilities with sched_get_priority_max()\n");

    printf("\n=== Next Steps ===\n");
    printf("Continue to Exercise 0F to learn statistical analysis of measurements.\n");

    return 0;
}
