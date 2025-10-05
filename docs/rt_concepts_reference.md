# Real-Time Systems - Concepts Quick Reference

## Core Timing Concepts

### Latency
Time between when a task should execute and when it actually starts executing.
- Measured in microseconds (µs) or nanoseconds (ns)
- Low latency = responsive system
- Example: 45 µs average deviation from target sleep

### Jitter
Variation in latency over time; unpredictability in timing.
- Formula: `jitter = max_latency - min_latency`
- Low jitter = predictable, deterministic system
- More critical than average latency for RT systems
- Example: Prefer consistent 10ms over variable 1-100ms

### Determinism
Ability to predict system behavior with bounded worst-case execution.
- RT systems prioritize determinism over raw performance
- Requires predictable worst-case behavior
- Essential for safety-critical systems

### WCET (Worst Case Execution Time)
Maximum time a task can take to execute under any conditions.
- Required for schedulability analysis
- Must account for cache misses, interrupts, contention
- Critical for meeting hard real-time deadlines

---

## Real-Time System Classifications

### Hard Real-Time
Missing deadline = system failure (catastrophic).
- **Zero tolerance** for deadline misses
- Examples: airbag deployment, pacemaker, flight control
- Requires formal WCET analysis and schedulability proof
- Must guarantee all deadlines are met

### Soft Real-Time
Missing deadline = degraded performance (acceptable occasionally).
- Best-effort with graceful degradation
- Examples: video streaming, audio playback, UI responsiveness
- Occasional misses tolerable but undesirable
- Focus on average-case performance

---

## Linux Scheduling Policies

### SCHED_OTHER (CFS)
Default time-sharing scheduler (Completely Fair Scheduler).
- Priority: Nice values (-20 to +19)
- Preemption: Time-sliced, fair sharing
- Use case: General-purpose applications
- Not suitable for real-time guarantees

### SCHED_FIFO
Real-time First-In-First-Out scheduling.
- Priority: 1-99 (higher = more important)
- Preemption: Only by higher priority tasks
- Runs until: yields, blocks, or preempted by higher priority
- Use case: Single high-priority RT task

### SCHED_RR
Real-time Round-Robin scheduling.
- Priority: 1-99 (higher = more important)
- Preemption: Time-sliced among same-priority tasks
- Use case: Multiple RT tasks at same priority level
- Similar to FIFO but fair within priority level

### SCHED_DEADLINE
Earliest Deadline First (EDF) scheduling.
- Priority: Automatic based on deadlines
- Preemption: Deadline-based preemption
- Use case: Complex RT systems with varying deadlines
- Optimal dynamic priority scheduler

---

## Synchronization Problems

### Priority Inversion
High-priority task indirectly blocked by lower-priority task through resource contention.

**Classic Scenario:**
1. Low-priority task (L) acquires mutex
2. High-priority task (H) blocks on same mutex
3. Medium-priority task (M) preempts L
4. **Result:** H waits for L, but L can't run (blocked by M)

**Types:**
- **Unbounded:** H blocks indefinitely (depends on M's execution)
- **Bounded:** H blocks only for L's critical section duration

**Real Example:** Mars Pathfinder (1997) - watchdog resets due to priority inversion

### Priority Inheritance Protocol (PIP)
Solution to bounded priority inversion.

**Mechanism:**
1. When H blocks on mutex held by L
2. L temporarily inherits H's priority
3. M cannot preempt L (L now runs at H's priority)
4. L completes critical section and releases mutex
5. L returns to original priority
6. H acquires mutex and proceeds

**Benefits:**
- Bounds blocking time to critical section length
- Automatic kernel-managed priority boost
- Predictable worst-case blocking

**Implementation:**
```c
pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
```

---

## Deadlock

### Definition
Two or more tasks permanently blocked, each waiting for resources held by others (circular wait).

### Four Coffman Conditions
Deadlock occurs when **ALL FOUR** are present:
1. **Mutual Exclusion:** Resources cannot be shared
2. **Hold and Wait:** Tasks hold resources while waiting for others
3. **No Preemption:** Resources cannot be forcibly taken
4. **Circular Wait:** A waits for B, B waits for C, C waits for A

**Breaking ANY ONE condition prevents deadlock!**

### Detection Strategies

**Timeout-Based:**
- Use `pthread_mutex_timedlock()` with timeout
- If timeout expires, assume deadlock
- Release held locks and retry
- **Drawbacks:** False positives, wasted time, complex recovery

**Resource Allocation Graph:**
- Visual model: Tasks (circles) and Resources (squares)
- Edges show requests and allocations
- **Cycle in graph = Deadlock**

### Prevention Strategies

**Lock Ordering (Preferred):**
- Assign global order to all locks (e.g., mutex1=1, mutex2=2)
- **Rule:** Always acquire in increasing order
- Breaks circular wait condition
- No timeouts needed, zero overhead

**Banker's Algorithm:**
- Check if allocation leaves system in safe state
- Refuse allocations leading to unsafe states
- Used in OS resource allocation

**Hierarchical Locking:**
- Organize locks into hierarchy levels
- Always lock from top to bottom
- Prevents circular dependencies

---

## Periodic Task Concepts

### Period (T)
Time interval between task activations.
- Example: Control loop every 10ms → T = 10ms

### Execution Time (C)
Time required to complete one task instance.
- Should use WCET for RT analysis
- Example: Sensor processing takes 2ms → C = 2ms

### Deadline (D)
Time by which task must complete.
- Often equals period: D = T
- **Deadline miss:** Task completes after deadline

### Utilization (U)
Fraction of CPU time used by a task.
- Formula: `U = C / T`
- Example: C=2ms, T=10ms → U = 0.2 (20%)

### Schedulability
System is schedulable if all tasks can meet deadlines.
- **Rate Monotonic (RM):** Priority inversely proportional to period
  - Schedulability bound: `U_total ≤ n(2^(1/n) - 1)` ≈ 0.69 for many tasks
- **EDF:** Optimal dynamic scheduler
  - Schedulability: `U_total ≤ 1.0`

---

## Measurement Techniques

### High-Precision Timing
```c
clock_gettime(CLOCK_MONOTONIC, &ts);
```
- Monotonic clock: never jumps backward
- Nanosecond precision
- Not affected by system time adjustments

### High-Precision Sleep
```c
clock_nanosleep(CLOCK_MONOTONIC, 0, &interval, NULL);
```
- More accurate than `usleep()` or `nanosleep()`
- Uses monotonic clock
- Absolute or relative timing

### Setting RT Priority
```c
struct sched_param param;
param.sched_priority = 80;
sched_setscheduler(0, SCHED_FIFO, &param);
```
- Requires `CAP_SYS_NICE` capability
- Priority range: 1-99 for RT policies
- Typical RT priority: 80

### CPU Affinity
```c
pthread_setaffinity_np(thread, sizeof(cpu_set), &cpu_set);
```
- Pin task to specific CPU core
- Reduces cache misses and migration overhead
- Improves timing predictability

---

## System Load Impact

### CPU Stress
Tools: `stress-ng --cpu 4`
- Increases scheduling contention
- Higher jitter in normal scheduling
- RT scheduling shows resilience

### I/O Stress
Tools: `stress-ng --io 2`
- Kernel interrupt handling overhead
- Can affect even RT tasks
- Demonstrates system interference

### Memory Pressure
Tools: `stress-ng --vm 2 --vm-bytes 512M`
- Page faults increase latency
- Cache eviction affects WCET
- Memory locking (`mlockall`) helps RT tasks

---

## Best Practices

### Critical Section Management
- **Keep short:** Minimize time holding locks
- **Avoid blocking:** No I/O or long computations in critical sections
- **Use PI mutexes:** Always in RT systems

### Lock Hygiene
- **Document ordering:** Assign and document lock hierarchy
- **Validate at runtime:** Use assertions to check ordering
- **Static analysis:** Use ThreadSanitizer, Helgrind
- **Code reviews:** Check for ordering violations

### RT Task Design
- **Bounded execution:** Ensure WCET is known and bounded
- **Avoid dynamic allocation:** Memory allocation is unpredictable
- **Lock memory:** Use `mlockall(MCL_CURRENT | MCL_FUTURE)`
- **Disable paging:** Prevent page faults

### Schedulability Analysis
- **Calculate utilization:** Ensure U_total within bounds
- **Account for blocking:** Include mutex blocking time
- **Test under load:** Verify with stress testing
- **Formal verification:** Prove schedulability for critical systems

---

## Tools and Commands

### Timing Measurement
- `cyclictest`: Industry-standard latency measurement
- `clock_gettime()`: High-precision timestamp
- `perf`: Performance profiling

### Tracing and Analysis
- `ftrace`: Kernel function tracing
- `trace-cmd`: Trace recording and analysis
- `perf sched`: Scheduling event analysis

### Debugging
- `htop`: Visual task priority monitoring
- `chrt`: View/set scheduling policy and priority
- `ps -eo pid,class,rtprio,cmd`: Show task scheduling info

### Static Analysis
- ThreadSanitizer: Race and deadlock detection
- Helgrind (Valgrind): Synchronization error detection
- Lockdep (Linux kernel): Runtime lock ordering validation

---

## Important Formulas

### Jitter
```
jitter = max_latency - min_latency
```

### Utilization
```
U = C / T
(where C = execution time, T = period)
```

### Rate Monotonic Schedulability
```
U_total ≤ n(2^(1/n) - 1)
(for n tasks, limit ≈ 0.69 as n → ∞)
```

### EDF Schedulability
```
U_total ≤ 1.0
(optimal for periodic tasks)
```

### Blocking Time (with Priority Inheritance)
```
B_i = max(C_k) for all lower-priority tasks k that share resources
(bounded by longest critical section)
```

---

## Docker/Container Limitations

### Shared Kernel
- Container uses host kernel (not isolated)
- RT guarantees require PREEMPT_RT on host
- Results show relative differences, not absolute RT performance

### Required Capabilities
- `--privileged` or `--cap-add=SYS_NICE` for RT scheduling
- Needed for `sched_setscheduler()` to work

### Focus
- **Conceptual understanding** over production-grade RT
- **Relative comparisons:** RT vs normal, with/without load
- **Educational:** Learn patterns and principles

---

## Lock-Free Programming (Week 4)

### Progress Guarantees

**Wait-Free (Strongest):**
- Every thread makes progress in finite steps
- No thread can be delayed indefinitely
- Hardest to implement, best theoretical guarantee

**Lock-Free:**
- At least one thread makes progress in finite steps
- Other threads may retry (CAS failures)
- No locks, no blocking, no priority inversion

**Obstruction-Free:**
- Progress guaranteed only if thread runs in isolation
- Weaker than lock-free

**Blocking (Weakest):**
- Threads can wait indefinitely for locks
- Subject to priority inversion and deadlock

### Compare-And-Swap (CAS)

Fundamental atomic operation for lock-free algorithms:

```c
bool CAS(T* ptr, T expected, T new_value) {
    atomic {
        if (*ptr == expected) {
            *ptr = new_value;
            return true;  // Success
        }
        return false;     // Failed - value changed
    }
}
```

**GCC/Clang Builtin:**
```c
__atomic_compare_exchange_n(
    &var,           // Pointer to variable
    &expected,      // Expected value (updated on failure!)
    new_val,        // New value to set
    false,          // Weak (allow spurious failures)
    __ATOMIC_SEQ_CST,   // Success memory order
    __ATOMIC_SEQ_CST    // Failure memory order
);
```

**Usage Pattern:**
```c
do {
    old_value = atomic_load(&var);
    new_value = compute(old_value);
} while (!CAS(&var, old_value, new_value));
```

### ABA Problem

**Scenario:**
1. Thread 1 reads pointer A
2. Thread 2: changes A→B→A (pops, pops, pushes)
3. Thread 1's CAS succeeds (pointer is A)
4. **Problem:** But it's a different A! Data structure corrupted

**Example:**
```
Stack: A → B → C
T1: Read head (A), prepare to CAS to B
[T1 preempted]
T2: Pop A (head = B)
T2: Pop B (head = C)
T2: Push A (head = A again)
[T1 resumes]
T1: CAS succeeds! (head was A, now B)
    But B is dangling/freed!
```

**Solutions:**
- **Tagged Pointers:** Include version counter with pointer
- **Hazard Pointers:** Mark nodes as "in use" before accessing
- **Epoch-Based Reclamation:** Track epochs, defer freeing
- **Garbage Collection:** Automatic memory management
- **Double-Width CAS:** CAS on pointer+counter atomically

### Memory Ordering

**Sequential Consistency (`__ATOMIC_SEQ_CST`):**
- All threads see operations in same total order
- Strongest guarantee, slowest performance
- Use for correctness first

**Acquire (`__ATOMIC_ACQUIRE`):**
- Loads: Subsequent reads/writes can't move before this
- For acquiring locks/data

**Release (`__ATOMIC_RELEASE`):**
- Stores: Previous reads/writes can't move after this
- For releasing locks/publishing data

**Relaxed (`__ATOMIC_RELAXED`):**
- No ordering constraints
- Fastest, most dangerous
- Only for independent counters

**For RT:** Start with SEQ_CST, optimize later with profiling

### Lock-Free Stack Algorithm

**Structure:**
```c
typedef struct Node {
    int data;
    struct Node* next;
} Node;

_Atomic(Node*) head;
```

**Push (Lock-Free):**
```c
void push(int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = value;

    Node* old_head;
    do {
        old_head = atomic_load(&head);
        new_node->next = old_head;
    } while (!atomic_compare_exchange(&head, &old_head, new_node));
}
```

**Pop (Lock-Free):**
```c
bool pop(int* value) {
    Node* old_head, *new_head;
    do {
        old_head = atomic_load(&head);
        if (old_head == NULL) return false;
        new_head = old_head->next;
    } while (!atomic_compare_exchange(&head, &old_head, new_head));

    *value = old_head->data;
    free(old_head);  // WARNING: ABA problem!
    return true;
}
```

**Key Points:**
- No locks needed
- Retry on CAS failure
- Memory reclamation is tricky (ABA)

### Lock-Free vs Locks Trade-offs

**Lock-Free Advantages:**
- No priority inversion
- No deadlock possible
- Better scalability under high contention
- Predictable worst-case (no blocking)
- Thread failure doesn't block system

**Lock-Free Disadvantages:**
- Complex to implement correctly
- ABA problem requires careful handling
- Memory ordering is subtle
- CAS retries waste CPU
- Harder to reason about correctness
- Not all data structures have efficient lock-free versions

**When to Use Lock-Free:**
- High contention scenarios
- RT systems (blocking unacceptable)
- Scalability across many cores critical
- Simple structures (stack, queue, counter)

**When to Use Locks:**
- Low contention
- Complex data structures
- Simplicity valued
- Team lacks lock-free expertise

### Memory Reclamation Strategies

**Problem:** When can we safely free a node popped from lock-free structure?

**Hazard Pointers:**
```c
// Thread announces: "I'm using this pointer"
hazard_ptr[thread_id] = node;
memory_fence();
// Use node safely...
hazard_ptr[thread_id] = NULL;

// Defer freeing nodes in hazard_ptr array
```

**Epoch-Based Reclamation:**
```c
global_epoch = 0;
thread_epoch[tid] = 0;

// Periodically:
global_epoch++;
// Free nodes from epochs < min(thread_epoch)
```

**Reference Counting:**
- Increment refcount on access
- Decrement on done
- Free when refcount → 0
- Beware of ABA on refcount itself!

### Other Atomic Operations

**Atomic Load:**
```c
int value = __atomic_load_n(&var, __ATOMIC_SEQ_CST);
```

**Atomic Store:**
```c
__atomic_store_n(&var, new_value, __ATOMIC_SEQ_CST);
```

**Atomic Fetch-And-Add:**
```c
int old = __atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST);
```

**Atomic Exchange:**
```c
int old = __atomic_exchange_n(&var, new_value, __ATOMIC_SEQ_CST);
```

---

## Week Progression

### Week 1: RT Fundamentals
- Latency and jitter measurement
- Scheduling policy comparison
- Periodic task simulation
- Impact of system load

### Week 2: Synchronization
- Priority inversion problem
- Priority Inheritance Protocol
- Bounded vs unbounded blocking

### Week 3: Deadlock
- Four Coffman conditions
- Detection with timeouts
- Prevention with lock ordering
- Resource allocation graphs

### Week 4: Lock-Free Programming
- Progress guarantees (wait-free, lock-free, blocking)
- Compare-And-Swap (CAS) operations
- Lock-free stack implementation
- ABA problem and solutions
- Memory ordering and barriers
- Performance comparison: locks vs lock-free

---

## Key Metrics to Track

### Latency Metrics
- Minimum latency
- Maximum latency
- Average latency
- Jitter (max - min)
- 95th/99th percentile

### Task Metrics
- Deadline misses (count and percentage)
- Response time (time from release to completion)
- Blocking time (time waiting for resources)
- Execution time distribution

### System Metrics
- CPU utilization per core
- Context switches per second
- Interrupt overhead
- Cache miss rate (if measurable)

---

## Common Pitfalls

### Timing
- ❌ Using `sleep()` instead of `clock_nanosleep()`
- ❌ Using `CLOCK_REALTIME` (can jump) instead of `CLOCK_MONOTONIC`
- ❌ Ignoring timer resolution and overhead

### Synchronization
- ❌ Using regular mutexes in RT systems (no PI)
- ❌ Long critical sections
- ❌ Nested locks without ordering discipline

### Scheduling
- ❌ Not accounting for blocking time in schedulability
- ❌ Assuming 100% utilization is safe (need margin)
- ❌ Ignoring priority inversion

### System
- ❌ Not locking memory (paging causes unpredictability)
- ❌ Dynamic memory allocation in RT tasks
- ❌ Not isolating RT tasks from system interference

---

**This reference covers fundamental RT concepts from Weeks 1-3. Details and code examples are in the respective week guides.**
