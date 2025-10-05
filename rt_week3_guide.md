# Real-Time Systems - Week 3 Lab Guide

## Overview
Week 3 focuses on **deadlock in real-time systems**, understanding how circular wait conditions occur and how to prevent them through proper resource management.

## Learning Objectives
- Understand deadlock and the four Coffman conditions
- Recognize circular wait in multi-threaded systems
- Apply deadlock detection using timeouts
- Implement deadlock prevention through lock ordering
- Compare detection vs prevention strategies

## Background: Deadlock

### What is Deadlock?

**Deadlock** is a situation where two or more tasks are permanently blocked, each waiting for resources held by the others, creating a circular wait condition.

### Four Coffman Conditions

Deadlock occurs when **ALL FOUR** conditions are present simultaneously:

1. **Mutual Exclusion**: Resources cannot be shared (e.g., mutex locks)
2. **Hold and Wait**: Tasks hold resources while waiting for others
3. **No Preemption**: Resources cannot be forcibly taken away
4. **Circular Wait**: Task A waits for B, B waits for C, ..., C waits for A

**Breaking ANY ONE condition prevents deadlock!**

### Simple Deadlock Scenario

```
Time    Task A              Task B
----    ------              ------
  0     lock(mutex1) ✓
  1                         lock(mutex2) ✓
  2     wait for mutex2
  3                         wait for mutex1
  4     [BLOCKED]           [BLOCKED]

Result: DEADLOCK! Neither can proceed.
```

---

## Exercise 6: Deadlock Demonstration & Detection

### Concept
Demonstrate how opposite lock ordering creates deadlock and use timeouts for detection.

### Run the Exercise
```bash
make exercise6
./exercise6
```

### Expected Output

```
=== Exercise 6: Deadlock Demonstration & Detection ===

[  0.010] TASK-A: Started
[  0.010] TASK-A: Locking mutex1...
[  0.010] TASK-A: ✓ Acquired mutex1
[  0.060] TASK-B: Started
[  0.060] TASK-B: Locking mutex2...
[  0.060] TASK-B: ✓ Acquired mutex2
[  0.110] TASK-A: Trying to lock mutex2...
[  0.160] TASK-B: Trying to lock mutex1...
[  1.110] TASK-A: ✗ TIMEOUT waiting for mutex2!
[  1.110] TASK-A: *** DEADLOCK DETECTED ***
[  1.110] TASK-A: Released mutex1, will retry...
[  1.160] TASK-B: ✗ TIMEOUT waiting for mutex1!
[  1.160] TASK-B: *** DEADLOCK DETECTED ***
[  1.160] TASK-B: Released mutex2, will retry...
[  2.210] TASK-A: Locking mutex1...
[  2.210] TASK-A: ✓ Acquired mutex1
[  2.310] TASK-A: Trying to lock mutex2...
[  2.310] TASK-A: ✓ Acquired mutex2
[  2.310] TASK-A: Accessing shared resources...
[  2.360] TASK-A: Released both mutexes
[  2.360] TASK-A: ✓ Completed successfully
[  3.260] TASK-B: Locking mutex2...
[  3.260] TASK-B: ✓ Acquired mutex2
[  3.360] TASK-B: Trying to lock mutex1...
[  3.360] TASK-B: ✓ Acquired mutex1
[  3.360] TASK-B: Accessing shared resources...
[  3.410] TASK-B: Released both mutexes
[  3.410] TASK-B: ✓ Completed successfully

=== Analysis ===
✗ DEADLOCK WAS DETECTED!

Deadlock occurred because:
  1. Task A held mutex1, waiting for mutex2
  2. Task B held mutex2, waiting for mutex1
  3. Circular wait condition created
  4. Neither task could proceed
```

### What's Happening?

**Initial State:**
- Task A locks mutex1
- Task B locks mutex2

**Deadlock Formation:**
- Task A tries mutex2 → blocked (B holds it)
- Task B tries mutex1 → blocked (A holds it)
- **Circular wait:** A→B→A

**Detection:**
- `pthread_mutex_timedlock()` with 1 second timeout
- Timeout expires → deadlock detected
- Tasks release locks and retry

**Recovery:**
- After timeout, release held lock
- Back off (sleep 1 second)
- Retry acquisition
- Eventually one succeeds

### Code Analysis

Key sections in `exercise6.c`:

```c
// Task A: mutex1 → mutex2
pthread_mutex_lock(&mutex1);          // Get mutex1
pthread_mutex_timedlock(&mutex2, ...); // Try mutex2 (timeout!)

// Task B: mutex2 → mutex1 (OPPOSITE!)
pthread_mutex_lock(&mutex2);          // Get mutex2
pthread_mutex_timedlock(&mutex1, ...); // Try mutex1 (timeout!)
```

**Detection mechanism:**
```c
struct timespec timeout;
calc_timeout(&timeout, 1000);  // 1 second from now

result = pthread_mutex_timedlock(&mutex, &timeout);
if (result == ETIMEDOUT) {
    // Deadlock detected!
    pthread_mutex_unlock(&held_mutex);
    // Retry
}
```

### Problems with Timeout Detection

**Disadvantages:**
1. ✗ False positives (slow != deadlocked)
2. ✗ Difficult to choose correct timeout
3. ✗ Overhead of timeout management
4. ✗ Complex recovery logic
5. ✗ May not work in all scenarios
6. ✗ Wastes time waiting for timeout

**When to use:**
- Last resort when prevention not possible
- Non-critical systems where retry is acceptable
- Debugging and testing

### Experiments to Try

1. **Change timeout value:**
   ```c
   #define DEADLOCK_TIMEOUT_MS 100  // Shorter timeout
   ```
   - Faster detection but more false positives

2. **Remove timeout (deadlock forever):**
   ```c
   pthread_mutex_lock(&mutex2);  // Instead of timedlock
   ```
   - Tasks hang indefinitely

3. **Add more tasks:**
   - Create Task C that also needs both mutexes
   - Increases deadlock frequency

4. **Vary delays:**
   - Change `usleep()` values
   - Affects deadlock probability

### Questions to Answer

1. How often does deadlock occur? (Run multiple times)
2. What's the minimum timeout that detects deadlock reliably?
3. How many retries are needed before success?
4. What happens if timeout is too short?

---

## Exercise 7: Deadlock Prevention with Lock Ordering

### Concept
Prevent deadlock by enforcing a global lock ordering discipline.

### Run the Exercise
```bash
make exercise7
./exercise7
```

### Expected Output

```
=== Exercise 7: Deadlock Prevention with Lock Ordering ===

[  0.010] TASK-A: Started
[  0.010] TASK-A: Locking mutex1 (order: 1)...
[  0.010] TASK-A: ✓ Acquired mutex1
[  0.060] TASK-B: Started
[  0.110] TASK-A: Locking mutex2 (order: 2)...
[  0.110] TASK-A: ✓ Acquired mutex2
[  0.110] TASK-A: Accessing shared resources...
[  0.110] TASK-B: Locking mutex1 (order: 1)...
[  0.160] TASK-A: Work completed, releasing locks...
[  0.160] TASK-A: Released mutex2
[  0.160] TASK-A: Released mutex1
[  0.160] TASK-A: ✓ Completed successfully
[  0.160] TASK-B: ✓ Acquired mutex1
[  0.260] TASK-B: Locking mutex2 (order: 2)...
[  0.260] TASK-B: ✓ Acquired mutex2
[  0.260] TASK-B: Accessing shared resources...
[  0.310] TASK-B: Work completed, releasing locks...
[  0.310] TASK-B: Released mutex2
[  0.310] TASK-B: Released mutex1
[  0.310] TASK-B: ✓ Completed successfully

=== Analysis ===
✓ BOTH TASKS COMPLETED SUCCESSFULLY!
✓ NO DEADLOCK occurred

Execution times:
  Task A: 150.2 ms
  Task B: 250.5 ms
  Total:  300.8 ms

Why no deadlock?
  • Both tasks acquire locks in SAME order
  • Task that gets mutex1 first will complete fully
  • Other task waits, then proceeds when locks free
  • No circular wait condition possible
```

### What's Happening?

**Lock Ordering Rule:**
- Define global order: mutex1 (order 1) < mutex2 (order 2)
- **ALL** tasks must lock in same order: 1 → 2

**Execution Flow:**
1. Task A locks mutex1 (order 1) ✓
2. Task B tries mutex1 (order 1) → blocks on A
3. Task A locks mutex2 (order 2) ✓
4. Task A completes and releases both
5. Task B acquires mutex1 ✓
6. Task B acquires mutex2 ✓
7. Task B completes ✓

**No Circular Wait:**
- Both need mutex1 first
- Whichever gets it continues to mutex2
- Other waits until both are free
- Linear wait chain, not circular!

### Code Analysis

Key sections in `exercise7.c`:

```c
// Task A: mutex1 → mutex2 (order 1→2)
pthread_mutex_lock(&mutex1);   // Order 1
pthread_mutex_lock(&mutex2);   // Order 2

// Task B: mutex1 → mutex2 (SAME order 1→2)
pthread_mutex_lock(&mutex1);   // Order 1
pthread_mutex_lock(&mutex2);   // Order 2
```

**No timeouts needed** - deadlock is impossible!

### Lock Ordering Discipline

**Rule:** Assign a unique order number to each lock

```
mutex1 = order 1
mutex2 = order 2
mutex3 = order 3
...
```

**Enforcement:** Always acquire in increasing order

```c
// ✓ Correct patterns:
lock(1) → lock(2) → lock(3)
lock(1) → lock(3)           // Skip 2 is OK
lock(2) → lock(3)

// ✗ Wrong patterns:
lock(2) → lock(1)           // Violates order!
lock(3) → lock(2) → lock(1) // Reverse order
lock(1) → lock(3) → lock(2) // Out of order
```

**Breaking Circular Wait:**
- Circular wait needs: A waits for B, B waits for A
- With ordering: Both lock same resource first
- One succeeds, other waits (no cycle!)

### Comparison: Detection vs Prevention

| Aspect | Exercise 6 (Detection) | Exercise 7 (Prevention) |
|--------|------------------------|-------------------------|
| Lock order | Opposite (A≠B) | Same (A=B) |
| Deadlock | YES | NO |
| Timeout needed | YES | NO |
| Retries | Multiple | None |
| Complexity | High | Low |
| Execution time | Unpredictable | Predictable |
| Overhead | High | Minimal |
| Reliability | Fair | Excellent |

**Conclusion: Prevention >> Detection**

### Experiments to Try

1. **Violate lock ordering:**
   ```c
   // In Task B, reverse order:
   pthread_mutex_lock(&mutex2);  // Wrong order!
   pthread_mutex_lock(&mutex1);
   ```
   - Deadlock will occur

2. **Add third mutex:**
   ```c
   pthread_mutex_t mutex3;
   // Order: 1 → 2 → 3
   ```
   - Verify ordering still prevents deadlock

3. **Measure timing:**
   - Compare execution time with Exercise 6
   - Prevention is faster and more consistent

4. **Stress test:**
   - Create 10 tasks all needing same locks
   - Ordering ensures no deadlock

### Questions to Answer

1. Why is Task B's execution time longer?
2. Could deadlock occur with same ordering? Why not?
3. How would you handle dynamic lock acquisition?
4. What if lock order can't be predetermined?

---

## Advanced Topics

### 1. Resource Allocation Graph

Visual tool for deadlock analysis:

**Nodes:**
- Circles = Tasks
- Squares = Resources

**Edges:**
- Task → Resource = Request
- Resource → Task = Allocation

**Deadlock Detection:**
- Cycle in graph = Deadlock!

**Example:**
```
Task A → [mutex1] → Task A  (holds)
       ↓
     [mutex2]
       ↑
Task B → [mutex2] → Task B  (holds)
       ↓
     [mutex1]  ← Cycle! Deadlock!
```

### 2. Banker's Algorithm

Prevention through safe state checking:

1. Before allocating resource, check if system remains in safe state
2. Safe state = execution order exists where all tasks can complete
3. Unsafe state ≠ deadlock, but deadlock is possible
4. Refuse allocation that leads to unsafe state

**Used in:** Operating systems, database transactions

### 3. Hierarchical Locking

Assign locks to hierarchy levels:

```
Level 3: Application locks
Level 2: Subsystem locks
Level 1: Low-level locks
```

**Rule:** Lock higher levels before lower levels
- Always lock from top down
- Prevents circular dependencies

### 4. Two-Phase Locking (Databases)

**Growing Phase:**
- Acquire all needed locks
- No releases allowed

**Shrinking Phase:**
- Release locks
- No acquisitions allowed

**Prevents:** Deadlock through strict protocol

### 5. Wait-Die / Wound-Wait

Timestamp-based prevention:

**Wait-Die (non-preemptive):**
- Older task waits for younger
- Younger task dies (restarts) if conflicts with older

**Wound-Wait (preemptive):**
- Older task wounds (preempts) younger
- Younger task waits for older

**Ensures:** No circular wait through age ordering

---

## Best Practices

### 1. Lock Ordering Checklist

✓ Document all locks with order numbers
✓ Use naming conventions (e.g., `lock_L1_xxx`)
✓ Validate order at runtime with assertions
✓ Code review for ordering violations
✓ Use static analysis tools

### 2. Minimize Lock Scope

```c
// ✓ Good: Short critical section
pthread_mutex_lock(&mutex);
shared_var++;
pthread_mutex_unlock(&mutex);

// ✗ Bad: Long critical section
pthread_mutex_lock(&mutex);
do_complex_work();          // Takes time
shared_var++;
pthread_mutex_unlock(&mutex);
```

### 3. Avoid Nested Locks

```c
// ✓ Best: Single lock
pthread_mutex_lock(&single_mutex);
access_resources();
pthread_mutex_unlock(&single_mutex);

// ⚠ Risky: Nested locks (needs ordering!)
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);
// Must ensure correct order!
pthread_mutex_unlock(&mutex2);
pthread_mutex_unlock(&mutex1);
```

### 4. Use Try-Lock for Flexibility

```c
if (pthread_mutex_trylock(&mutex2) != 0) {
    // Couldn't get second lock
    pthread_mutex_unlock(&mutex1);
    // Back off and retry
}
```

### 5. Static Analysis

Tools to detect potential deadlock:
- ThreadSanitizer (GCC/Clang)
- Helgrind (Valgrind)
- Lockdep (Linux kernel)

---

## Real-World Examples

### 1. Database Systems

**Problem:** Transactions lock rows in different order
**Solution:**
- Lock rows by primary key order
- Two-phase locking protocol
- Deadlock detection with rollback

### 2. Operating Systems

**Problem:** Processes competing for memory and I/O
**Solution:**
- Banker's algorithm for safe allocation
- Resource hierarchy (CPU > memory > I/O)
- Deadlock detection daemon

### 3. Distributed Systems

**Problem:** Nodes waiting for network responses
**Solution:**
- Timeout-based detection
- Distributed deadlock detection algorithms
- Chandy-Lamport snapshot algorithm

### 4. Aerospace Systems

**Problem:** Critical avionics tasks sharing resources
**Solution:**
- Static resource allocation
- Strict priority and ordering
- Formal verification of deadlock-freedom

---

## Summary

### Week 3 Key Takeaways

✓ **Deadlock Basics:**
- Four Coffman conditions must all be present
- Circular wait is the key condition to break
- Detection vs prevention trade-offs

✓ **Detection (Exercise 6):**
- Timeout-based detection
- Recovery through retry
- Last resort, not ideal

✓ **Prevention (Exercise 7):**
- Lock ordering eliminates circular wait
- More efficient than detection
- Preferred approach for RT systems

✓ **Best Practices:**
- Always use lock ordering
- Minimize lock scope
- Avoid nested locks when possible
- Document and enforce lock hierarchy

### Commands Reference

```bash
# Build Week 3 exercises
make week3

# Run individual exercises
make run6    # Deadlock demonstration
make run7    # Deadlock prevention

# Run both
make run-week3

# Build all weeks
make all
make run-all
```

---

## Assignment Ideas

### Part 1: Analysis
1. Run Exercise 6 multiple times, calculate deadlock frequency
2. Compare execution times: detection vs prevention
3. Graph retry attempts vs timeout value

### Part 2: Implementation
1. Implement 3-mutex scenario with ordering
2. Create resource allocation graph visualizer
3. Add dynamic lock ordering validation

### Part 3: Advanced
1. Implement Banker's algorithm for deadlock avoidance
2. Create wait-die protocol implementation
3. Design lock-free alternative to mutex-based solution

---

## Next Steps

**Week 4 Preview:** Lock-Free Programming
- Atomic operations and memory ordering
- Lock-free data structures (stack, queue)
- Compare-and-swap (CAS) operations
- ABA problem and solutions
- Performance analysis: locks vs lock-free

## Resources

- Coffman et al. (1971): "System Deadlocks"
- Dijkstra's Banker's Algorithm
- Linux kernel lockdep documentation
- "The Art of Multiprocessor Programming" - Herlihy & Shavit
- ThreadSanitizer: https://github.com/google/sanitizers
