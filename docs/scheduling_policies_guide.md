# Linux Scheduling Policies Guide

## Table of Contents
- [Overview](#overview)
- [Scheduling Policies](#scheduling-policies)
  - [SCHED_OTHER (CFS)](#sched_other-cfs)
  - [SCHED_FIFO](#sched_fifo)
  - [SCHED_RR](#sched_rr)
  - [SCHED_BATCH](#sched_batch)
  - [SCHED_IDLE](#sched_idle)
- [Priority Ranges](#priority-ranges)
- [Comparison Table](#comparison-table)
- [API Usage](#api-usage)
- [Practical Examples from Exercises](#practical-examples-from-exercises)
- [Performance Characteristics](#performance-characteristics)
- [Requirements & Permissions](#requirements--permissions)
- [Common Pitfalls & Best Practices](#common-pitfalls--best-practices)

---

## Overview

Linux provides multiple scheduling policies to handle different types of workloads. Understanding these policies is crucial for building real-time systems where predictable task execution is required.

**Two Main Categories:**
1. **Normal Scheduling** - Time-sharing for regular processes (SCHED_OTHER, SCHED_BATCH, SCHED_IDLE)
2. **Real-Time Scheduling** - Priority-based preemptive scheduling (SCHED_FIFO, SCHED_RR)

```
┌─────────────────────────────────────────────────────────┐
│           Linux Scheduler Architecture                   │
├─────────────────────────────────────────────────────────┤
│                                                           │
│  Real-Time Policies (Priority 1-99)                     │
│  ┌───────────────────────────────────────────────┐     │
│  │ SCHED_FIFO: Run until yield/block             │     │
│  │ SCHED_RR:   Run until time quantum expires    │     │
│  └───────────────────────────────────────────────┘     │
│                     ▲                                    │
│                     │ Higher priority always preempts   │
│                     │                                    │
│  Normal Policies (Priority 0)                           │
│  ┌───────────────────────────────────────────────┐     │
│  │ SCHED_OTHER: CFS fair scheduling              │     │
│  │ SCHED_BATCH: Non-interactive batch jobs       │     │
│  │ SCHED_IDLE:  Background tasks only            │     │
│  └───────────────────────────────────────────────┘     │
└─────────────────────────────────────────────────────────┘
```

---

## Scheduling Policies

### SCHED_OTHER (CFS)

**Type:** Normal, time-sharing
**Priority:** Always 0 (niceness controls weight: -20 to +19)
**Algorithm:** Completely Fair Scheduler (CFS)

#### How It Works
- Default policy for 99% of processes
- Uses virtual runtime tracking to ensure fairness
- No static priorities - niceness affects time slice weight
- Preemptible by any real-time task
- Optimized for interactive and throughput workloads

#### Characteristics
```
Time allocation: Fair sharing based on nice value
Preemption:      By RT tasks, or after time slice expires
Latency:         Variable (20-200 µs typical, can spike under load)
Use case:        Desktop apps, servers, batch processing
```

#### When to Use
- General-purpose applications
- Web servers, databases
- User-interactive programs
- Any non-real-time workload

**Code Example:**
```c
// SCHED_OTHER is the default - no special setup needed
// To explicitly set it:
struct sched_param param;
param.sched_priority = 0;  // Must be 0 for SCHED_OTHER
sched_setscheduler(0, SCHED_OTHER, &param);
```

---

### SCHED_FIFO

**Type:** Real-time, FIFO (First-In-First-Out)
**Priority:** 1-99 (higher number = higher priority)
**Algorithm:** Strict priority preemptive scheduling

#### How It Works
- Task runs until it:
  - Voluntarily yields (sched_yield())
  - Blocks on I/O or synchronization
  - Is preempted by higher-priority RT task
- **No time slicing** - can monopolize CPU indefinitely!
- Higher priority tasks always preempt lower priority
- Same priority tasks run in FIFO order

#### Characteristics
```
Time allocation: Unlimited until yield/block
Preemption:      Only by higher-priority RT tasks
Latency:         Very low (< 100 µs), deterministic
Use case:        Hard real-time tasks, control loops
```

#### Task Behavior Diagram
```
Priority 90: [████████████████] (runs continuously)
Priority 50: [waiting...]
Priority 10: [waiting...]
             └─> Lower priorities blocked until P90 yields
```

#### When to Use
- Hard real-time control loops
- Sensor data acquisition
- Safety-critical tasks with deadlines
- Tasks requiring guaranteed response time

**Code Example:**
```c
struct sched_param param;
param.sched_priority = 80;  // 1-99, typically use 50-90

if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
    perror("sched_setscheduler");
    // Requires CAP_SYS_NICE or root
}
```

**Safety Warning:**
```
⚠️  DANGER: SCHED_FIFO without yield can lock up your system!
    Always include sleep/yield in FIFO tasks
    Keep priority < 99 (kernel uses 99)
    Test with lower priorities first
```

---

### SCHED_RR

**Type:** Real-time, Round-Robin
**Priority:** 1-99 (higher number = higher priority)
**Algorithm:** Priority preemptive with time slicing

#### How It Works
- Like SCHED_FIFO, but adds **time quantum** (typically 100ms)
- After time quantum expires, task moves to end of queue at its priority
- Multiple tasks at same priority share CPU fairly
- Higher priority tasks still preempt lower priority

#### Characteristics
```
Time allocation: Time quantum (typically 100ms) per turn
Preemption:      By higher-priority RT tasks, or quantum expiry
Latency:         Very low (< 100 µs), deterministic
Use case:        Multiple RT tasks at same priority
```

#### Round-Robin Behavior
```
Priority 80 tasks with 100ms quantum:

Task A: [███████] yields to Task B
Task B: [███████] yields to Task C  } Same priority, time-shared
Task C: [███████] yields to Task A

Priority 90 task arrives → preempts immediately regardless of quantum
```

#### When to Use
- Multiple real-time tasks with equal importance
- Want RT guarantees but fair sharing among peers
- Preventing single task from monopolizing CPU
- Control loops with multiple sensors

**Code Example:**
```c
struct sched_param param;
param.sched_priority = 80;  // 1-99

if (sched_setscheduler(0, SCHED_RR, &param) != 0) {
    perror("sched_setscheduler");
}

// Check time quantum
struct timespec quantum;
sched_rr_get_interval(0, &quantum);
printf("RR quantum: %ld.%ld sec\n", quantum.tv_sec, quantum.tv_nsec);
```

#### SCHED_FIFO vs SCHED_RR
```
Scenario: Two tasks at priority 80

SCHED_FIFO:
Task 1: [████████████████████████████] (runs until yield)
Task 2:                              [████████████]

SCHED_RR:
Task 1: [██████] [██████] [██████]
Task 2:       [██████] [██████] [██████]
        └─> Alternates every 100ms
```

---

### SCHED_BATCH

**Type:** Normal, batch processing
**Priority:** Always 0 (niceness controls weight)
**Algorithm:** CFS with slight penalty for interactivity

#### How It Works
- Like SCHED_OTHER but optimized for non-interactive tasks
- Scheduler assumes task is CPU-bound
- Slightly lower priority than interactive SCHED_OTHER tasks
- Better throughput for batch jobs

#### When to Use
- Background compilation
- Data processing pipelines
- Scientific computations
- Video encoding

**Code Example:**
```c
struct sched_param param;
param.sched_priority = 0;
sched_setscheduler(0, SCHED_BATCH, &param);
```

---

### SCHED_IDLE

**Type:** Normal, extremely low priority
**Priority:** Always 0
**Algorithm:** Runs only when no other task is runnable

#### How It Works
- Lowest priority in the system
- Runs only when CPU would otherwise be idle
- Even nice +19 SCHED_OTHER tasks have higher priority

#### When to Use
- Indexing services (e.g., updatedb)
- Low-priority background tasks
- Tasks that should never interfere with anything else

**Code Example:**
```c
struct sched_param param;
param.sched_priority = 0;
sched_setscheduler(0, SCHED_IDLE, &param);
```

---

## Priority Ranges

| Policy       | Priority Range | Notes                                    |
|--------------|----------------|------------------------------------------|
| SCHED_OTHER  | 0              | Nice value (-20 to +19) controls weight |
| SCHED_BATCH  | 0              | Nice value (-20 to +19) controls weight |
| SCHED_IDLE   | 0              | Nice value ignored                       |
| SCHED_FIFO   | 1-99           | 99 = highest, reserved for kernel       |
| SCHED_RR     | 1-99           | 99 = highest, reserved for kernel       |

**Priority Hierarchy:**
```
  99 ← Kernel threads (migration, watchdog)
  90 ← High-priority RT user tasks
  80 ← Typical RT task priority (used in exercises)
  50 ← Medium-priority RT tasks
  10 ← Low-priority RT tasks
   1 ← Minimum RT priority
───────────────────────────────────────────
   0 ← SCHED_OTHER (all normal tasks)
   0 ← SCHED_BATCH
   0 ← SCHED_IDLE (runs last)
```

**Checking System Limits:**
```c
int min_fifo = sched_get_priority_min(SCHED_FIFO);  // Returns 1
int max_fifo = sched_get_priority_max(SCHED_FIFO);  // Returns 99
```

---

## Comparison Table

| Feature                | SCHED_OTHER | SCHED_FIFO | SCHED_RR | SCHED_BATCH | SCHED_IDLE |
|------------------------|-------------|------------|----------|-------------|------------|
| **Type**               | Normal      | Real-time  | Real-time| Normal      | Normal     |
| **Priority Range**     | 0           | 1-99       | 1-99     | 0           | 0          |
| **Time Slicing**       | Yes (CFS)   | No         | Yes      | Yes (CFS)   | Yes        |
| **Preemptible by RT**  | Yes         | By higher  | By higher| Yes         | Yes        |
| **Determinism**        | Low         | High       | High     | Low         | Low        |
| **Latency**            | 20-200 µs   | < 100 µs   | < 100 µs | Variable    | Variable   |
| **Use Case**           | Desktop     | Hard RT    | Soft RT  | Batch       | Background |
| **Requires Privileges**| No          | Yes        | Yes      | No          | No         |
| **Can Starve System**  | No          | **YES**    | **YES**  | No          | No         |

---

## API Usage

### Setting Scheduling Policy

```c
#include <sched.h>

int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
```

**Parameters:**
- `pid`: 0 for calling thread, or specific PID
- `policy`: SCHED_OTHER, SCHED_FIFO, SCHED_RR, etc.
- `param`: struct containing `sched_priority`

**Return:** 0 on success, -1 on error (check errno)

### Querying Current Settings

```c
// Get current policy
int policy = sched_getscheduler(0);  // 0 = current process

// Get current priority
struct sched_param param;
sched_getparam(0, &param);
printf("Priority: %d\n", param.sched_priority);

// Convert policy to name
const char* policy_name(int policy) {
    switch(policy) {
        case SCHED_OTHER: return "SCHED_OTHER";
        case SCHED_FIFO:  return "SCHED_FIFO";
        case SCHED_RR:    return "SCHED_RR";
        case SCHED_BATCH: return "SCHED_BATCH";
        case SCHED_IDLE:  return "SCHED_IDLE";
        default:          return "UNKNOWN";
    }
}
```

### Complete Example

```c
#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <string.h>

int main(void) {
    struct sched_param param;

    // Set to SCHED_FIFO with priority 80
    param.sched_priority = 80;

    if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
        if (errno == EPERM) {
            fprintf(stderr, "Error: Insufficient privileges\n");
            fprintf(stderr, "Run with: sudo ./program\n");
            fprintf(stderr, "Or in Docker: docker run --privileged ...\n");
        } else {
            perror("sched_setscheduler");
        }
        return 1;
    }

    printf("Successfully set to SCHED_FIFO priority 80\n");

    // Do real-time work...

    // Restore to normal scheduling before exit
    param.sched_priority = 0;
    sched_setscheduler(0, SCHED_OTHER, &param);

    return 0;
}
```

### Using with pthreads

```c
#include <pthread.h>
#include <sched.h>

void* thread_function(void *arg) {
    // Set scheduling for current thread
    struct sched_param param;
    param.sched_priority = 70;

    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) {
        perror("pthread_setschedparam");
    }

    // Thread work...
    return NULL;
}

int main(void) {
    pthread_t thread;
    pthread_attr_t attr;

    // Alternative: Set policy before thread creation
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    struct sched_param param;
    param.sched_priority = 70;
    pthread_attr_setschedparam(&attr, &param);

    pthread_create(&thread, &attr, thread_function, NULL);
    pthread_join(thread, NULL);

    pthread_attr_destroy(&attr);
    return 0;
}
```

---

## Practical Examples from Exercises

### Exercise 2: Policy Comparison

Demonstrates three policies running identical workload:

```c
// Thread 0: Normal scheduling
threads[0] = (thread_info_t){
    .policy = SCHED_OTHER,
    .priority = 0
};

// Thread 1: Real-time FIFO
threads[1] = (thread_info_t){
    .policy = SCHED_FIFO,
    .priority = 80
};

// Thread 2: Real-time Round-Robin
threads[2] = (thread_info_t){
    .policy = SCHED_RR,
    .priority = 80
};
```

**Typical Results:**
```
Execution order (by completion time):
Rank | Thread | Policy       | Priority | Duration (ms)
-----|--------|--------------|----------|-------------
  1  |   1    | SCHED_FIFO   |    80    |    245.123
  2  |   2    | SCHED_RR     |    80    |    246.891
  3  |   0    | SCHED_OTHER  |     0    |    512.456

RT threads completed 108% faster
```

**Key Insight:** RT policies (FIFO/RR) get CPU time first, completing much faster than SCHED_OTHER.

---

### Exercise 3: Periodic Task with RT Scheduling

Simulates a control loop running every 10ms:

```c
// Run with normal scheduling
$ ./exercise3
Running with SCHED_OTHER (normal scheduling)
Average latency: 156 µs
Jitter: 523 µs

// Run with RT scheduling
$ ./exercise3 --rt
Running with SCHED_FIFO priority 80
Average latency: 23 µs
Jitter: 47 µs
```

**Code Pattern:**
```c
if (use_rt) {
    struct sched_param param;
    param.sched_priority = RT_PRIORITY;

    if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
        perror("sched_setscheduler");
        use_rt = 0;  // Fall back to normal
    }
}

// Periodic loop
for (int cycle = 0; cycle < NUM_CYCLES; cycle++) {
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);

    // Do work
    do_computation(COMPUTATION_USEC);

    // Check for deadline miss...
}
```

**Key Insight:** SCHED_FIFO dramatically reduces jitter and improves deadline predictability.

---

### Exercise 4 & 5: Priority Inversion

Shows how scheduling policies interact with synchronization:

**Exercise 4 (Regular Mutex):**
```c
pthread_mutex_t mutex;  // No priority inheritance

// Low (P=10) holds mutex
// High (P=90) blocks on mutex
// Medium (P=50) preempts Low
// Result: High blocked for 150+ ms (unbounded inversion!)
```

**Exercise 5 (Priority Inheritance Mutex):**
```c
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
pthread_mutex_init(&mutex, &attr);

// Low (P=10) holds mutex
// High (P=90) blocks on mutex
// Low inherits P=90 (temporarily boosted!)
// Medium (P=50) CANNOT preempt Low
// Result: High blocked for ~50ms (bounded to critical section)
```

**Key Insight:** Priority inheritance prevents medium-priority tasks from indirectly blocking high-priority tasks.

---

## Performance Characteristics

### Latency Comparison

```
Typical latency (time from wakeup signal to actual execution):

SCHED_FIFO/RR:   < 100 µs   ████
SCHED_OTHER:     100-500 µs ████████████████
SCHED_BATCH:     500+ µs    ████████████████████████
SCHED_IDLE:      Variable   ████████████████████████████████

Under system load (stress-ng --cpu 4):

SCHED_FIFO/RR:   < 200 µs   ████████
SCHED_OTHER:     1-10 ms    ████████████████████████████████████████
```

### Jitter Comparison

**Jitter** = variation in latency (max - min)

```
Normal kernel (no PREEMPT_RT):
  SCHED_FIFO:  50-200 µs
  SCHED_OTHER: 500-5000 µs

RT-patched kernel (PREEMPT_RT):
  SCHED_FIFO:  < 50 µs
  SCHED_OTHER: 100-500 µs
```

### Context Switch Overhead

```
Policy Switch Cost:
  SCHED_OTHER → SCHED_OTHER:  1-2 µs
  SCHED_FIFO  → SCHED_FIFO:   1-2 µs
  SCHED_OTHER → SCHED_FIFO:   2-5 µs (cache effects)
```

---

## Requirements & Permissions

### CAP_SYS_NICE Capability

Real-time scheduling requires the `CAP_SYS_NICE` capability:

**Methods to obtain:**

1. **Run as root:**
   ```bash
   sudo ./my_rt_program
   ```

2. **Grant capability to executable:**
   ```bash
   sudo setcap cap_sys_nice=ep ./my_rt_program
   ./my_rt_program  # Now works without sudo
   ```

3. **Docker privileged mode:**
   ```bash
   docker run --privileged ...
   ```

4. **Docker with specific capability:**
   ```bash
   docker run --cap-add=SYS_NICE ...
   ```

5. **docker-compose.yml:**
   ```yaml
   services:
     rt-lab:
       privileged: true  # Grants all capabilities including CAP_SYS_NICE
   ```

### Checking Capabilities

```bash
# Check if current process can use RT scheduling
$ chrt -m
SCHED_FIFO min/max priority    : 1/99

# If you get an error, you lack CAP_SYS_NICE
```

### Error Handling

```c
if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
    if (errno == EPERM) {
        fprintf(stderr, "Error: Operation not permitted\n");
        fprintf(stderr, "Real-time scheduling requires privileges:\n");
        fprintf(stderr, "  - Run with sudo\n");
        fprintf(stderr, "  - Grant CAP_SYS_NICE capability\n");
        fprintf(stderr, "  - Use --privileged in Docker\n");

        // Fall back to normal scheduling
        param.sched_priority = 0;
        sched_setscheduler(0, SCHED_OTHER, &param);
    } else {
        perror("sched_setscheduler");
    }
}
```

---

## Common Pitfalls & Best Practices

### 1. SCHED_FIFO Runaway

**Problem:**
```c
// DANGER: This will lock up your system!
struct sched_param param = {.sched_priority = 99};
sched_setscheduler(0, SCHED_FIFO, &param);

while (1) {
    // Infinite loop with no yield
    compute();  // System is now frozen!
}
```

**Solution:**
```c
// SAFE: Always include yield points
struct sched_param param = {.sched_priority = 80};  // Not 99!
sched_setscheduler(0, SCHED_FIFO, &param);

while (running) {
    compute();

    // Yield CPU periodically
    usleep(1000);  // Sleep 1ms
    // OR
    sched_yield();  // Yield to same-priority tasks
    // OR
    clock_nanosleep(...);  // Sleep until next period
}
```

### 2. Priority Inversion

**Problem:**
```c
pthread_mutex_t mutex;  // Regular mutex
pthread_mutex_init(&mutex, NULL);

// High-priority task blocks on mutex held by low-priority task
// Medium-priority tasks can preempt low, causing unbounded blocking
```

**Solution:**
```c
pthread_mutex_t mutex;
pthread_mutexattr_t attr;

pthread_mutexattr_init(&attr);
pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);  // Enable PI
pthread_mutex_init(&mutex, &attr);

// Now low-priority holder inherits high-priority blocker's priority
```

### 3. Forgetting to Check Return Values

**Problem:**
```c
sched_setscheduler(0, SCHED_FIFO, &param);  // Ignores failures
// Program thinks it's RT but actually running as SCHED_OTHER
```

**Solution:**
```c
if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
    perror("sched_setscheduler");
    // Handle error appropriately
}

// Verify it worked
int actual_policy = sched_getscheduler(0);
if (actual_policy != SCHED_FIFO) {
    fprintf(stderr, "Warning: Failed to set RT scheduling\n");
}
```

### 4. Wrong Priority for SCHED_OTHER

**Problem:**
```c
// INVALID: SCHED_OTHER priority must be 0
struct sched_param param = {.sched_priority = 50};
sched_setscheduler(0, SCHED_OTHER, &param);  // Fails with EINVAL
```

**Solution:**
```c
// Correct: Use priority 0 for normal policies
struct sched_param param = {.sched_priority = 0};
sched_setscheduler(0, SCHED_OTHER, &param);

// Control scheduling with nice value instead
nice(10);  // Lower priority (+10)
```

### 5. Not Restoring Scheduling on Exit

**Problem:**
```c
int main(void) {
    struct sched_param param = {.sched_priority = 90};
    sched_setscheduler(0, SCHED_FIFO, &param);

    do_work();

    return 0;  // Exits with SCHED_FIFO still set!
}
```

**Solution:**
```c
int main(void) {
    struct sched_param param = {.sched_priority = 90};
    sched_setscheduler(0, SCHED_FIFO, &param);

    do_work();

    // Restore normal scheduling before exit
    param.sched_priority = 0;
    sched_setscheduler(0, SCHED_OTHER, &param);

    return 0;
}
```

### 6. Using Priority 99

**Problem:**
```c
// DON'T: Priority 99 is reserved for kernel threads
struct sched_param param = {.sched_priority = 99};
sched_setscheduler(0, SCHED_FIFO, &param);
// Can interfere with kernel watchdogs and migration threads
```

**Solution:**
```c
// DO: Use priorities 1-90 for user tasks
struct sched_param param = {.sched_priority = 80};  // Safe range
sched_setscheduler(0, SCHED_FIFO, &param);
```

### 7. CPU Utilization > 100%

**Problem:**
```c
// Task 1: 10ms period, 8ms execution → 80% CPU
// Task 2: 10ms period, 8ms execution → 80% CPU
// Total: 160% CPU → Impossible to schedule!
```

**Solution:**
```c
// Keep total utilization under 70-80% for safety margin
// U = Σ(execution_time / period) ≤ 0.7

// Task 1: 10ms period, 3ms execution → 30%
// Task 2: 10ms period, 3ms execution → 30%
// Total: 60% CPU → Safe
```

---

## Best Practices Summary

✅ **DO:**
- Always check return values from scheduling functions
- Use priority inheritance mutexes for RT tasks
- Include yield/sleep in SCHED_FIFO loops
- Keep RT priorities in range 1-90 (not 99)
- Calculate and verify total CPU utilization < 70%
- Test with lower priorities first
- Restore SCHED_OTHER on program exit
- Use clock_nanosleep() for precise timing

❌ **DON'T:**
- Use priority 99 for user tasks
- Create infinite loops without yield in SCHED_FIFO
- Forget to handle EPERM errors
- Use regular mutexes with RT tasks (use PI mutexes)
- Set SCHED_OTHER priority to non-zero
- Assume RT scheduling works without checking
- Ignore deadline misses or high jitter
- Run untested RT code on production systems

---

## Additional Resources

### Checking System Status

```bash
# View scheduling policies of running processes
ps -eLo pid,tid,class,rtprio,comm | head -20

# Check RT scheduling limits
chrt -m

# Monitor RT tasks
htop  # Press F2 → Columns → Add PRIORITY, NICE

# View kernel scheduler statistics
cat /proc/sched_debug
```

### Testing Tools

```bash
# Stress test system
stress-ng --cpu 4 --timeout 60s

# Measure latency under load
./exercise1 & stress-ng --cpu 4 --timeout 30s

# Compare RT vs normal
./exercise3       # Normal scheduling
./exercise3 --rt  # RT scheduling
```

### Related Man Pages

```bash
man 7 sched          # Overview of scheduling
man 2 sched_setscheduler
man 2 sched_getscheduler
man 2 sched_yield
man 2 sched_get_priority_max
man 3 pthread_setschedparam
man 1 chrt           # Command-line scheduling tool
```

---

d
