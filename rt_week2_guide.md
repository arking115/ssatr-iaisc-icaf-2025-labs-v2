# Real-Time Systems - Week 2 Lab Guide

## Overview
Week 2 focuses on **synchronization problems in real-time systems**, specifically priority inversion and its solution through priority inheritance protocol.

## Learning Objectives
- Understand the priority inversion problem
- Recognize unbounded vs bounded blocking
- Apply Priority Inheritance Protocol (PIP)
- Compare regular mutexes vs PI mutexes
- Analyze the impact on real-time guarantees

## Background: Priority Inversion

### What is Priority Inversion?

**Priority inversion** occurs when a high-priority task is indirectly blocked by a lower-priority task through resource contention.

**Classic Scenario:**
1. Low-priority task (L) acquires a mutex
2. High-priority task (H) tries to acquire the same mutex → blocks
3. Medium-priority task (M) preempts L
4. Result: H waits for L, but L can't run because M is running
5. **Priority inversion**: H is effectively blocked by M!

### Unbounded vs Bounded

**Unbounded Priority Inversion** (Exercise 4):
- H blocks for an indefinite time
- Duration depends on M's execution time
- Unpredictable and dangerous for RT systems
- Can cause deadline misses

**Bounded Priority Inversion** (Exercise 5):
- H blocks for at most the duration of L's critical section
- Predictable and analyzable
- Achieved through Priority Inheritance Protocol

## Exercise 4: Priority Inversion Demonstration

### Concept
Demonstrate the unbounded priority inversion problem using a regular mutex.

### Run the Exercise
```bash
make exercise4
./exercise4
```

### Expected Output
```
=== Exercise 4: Priority Inversion Demonstration ===

[  0.010] LOW: Started (priority 10)
[  0.020] LOW: Acquiring mutex...
[  0.020] LOW: ✓ Acquired mutex
[  0.020] LOW: Working with shared resource...
[  0.030] HIGH: Started (priority 90)
[  0.050] HIGH: Need shared resource - trying to acquire mutex...
[  0.050] HIGH: (This should block Low but...)
[  0.055] MEDIUM: Started (priority 50)
[  0.060] LOW: ⚠ Preempted by medium-priority task!
[  0.060] MEDIUM: *** STARTING WORK (causing priority inversion) ***
[  0.160] MEDIUM: Completed (high task was blocked this entire time!)
[  0.170] LOW: Releasing mutex
[  0.170] HIGH: ✓ Acquired mutex after blocking
         └─> Blocked for: 120.5 ms

=== Analysis ===
✗ UNBOUNDED PRIORITY INVERSION DETECTED!
  Expected blocking: ~50 ms (Low's work time)
  Actual blocking:   120.5 ms (includes Medium's interference)
```

### What's Happening?

1. **Low (P=10)** acquires regular mutex, starts 50ms of work
2. **High (P=90)** tries to acquire mutex → blocks on Low
3. **Medium (P=50)** becomes runnable → preempts Low (50 > 10)
4. **Medium runs for 100ms** while High is blocked
5. **Low finally completes** after Medium finishes
6. **High unblocks** after ~150ms total delay

**Problem:** High-priority task blocked for duration of Medium's execution!

### Code Analysis

Key sections in `exercise4.c`:

```c
// Regular mutex (NO priority inheritance)
pthread_mutex_init(&resource_mutex, NULL);

// Low priority task holds mutex
pthread_mutex_lock(&resource_mutex);  // Low acquires
// ... doing work ...

// High priority task blocks
pthread_mutex_lock(&resource_mutex);  // High blocks on Low

// Medium priority preempts Low
// Medium runs, delaying Low from releasing mutex
// High is indirectly blocked by Medium!
```

### Experiments to Try

1. **Vary Medium's execution time:**
   - Edit `MEDIUM_WORK_MS` in exercise4.c
   - Longer Medium work = longer High blocking
   - Shows unbounded nature

2. **Remove Medium task:**
   - Comment out Medium thread creation
   - High will only block for Low's critical section
   - Shows the difference Medium makes

3. **Add more Medium tasks:**
   - Create multiple Medium-priority threads
   - Even worse blocking for High

### Questions to Answer
1. How long was High blocked? Compare to Low's critical section time.
2. What would happen if Medium ran indefinitely?
3. Can you predict High's worst-case blocking time? Why not?
4. What are the implications for meeting deadlines?

---

## Exercise 5: Priority Inheritance Solution

### Concept
Solve priority inversion using Priority Inheritance Protocol (PIP).

### Run the Exercise
```bash
make exercise5
./exercise5
```

### Expected Output
```
=== Exercise 5: Priority Inheritance Solution ===

[  0.010] LOW: Started (priority 10)
[  0.020] LOW: Acquiring mutex...
[  0.020] LOW: ✓ Acquired mutex
[  0.030] HIGH: Started (priority 90)
[  0.050] HIGH: Need shared resource - trying to acquire mutex...
[  0.050] HIGH: (This will boost Low's priority to 90)
[  0.055] MEDIUM: Started (priority 50)
[  0.060] LOW: *** Priority inherited from HIGH (now running at P=90) ***
[  0.060] LOW: Working with shared resource (protected from Medium)...
[  0.065] MEDIUM: Attempting to run...
[  0.110] LOW: Releasing mutex (priority returns to 10)
[  0.110] HIGH: ✓ Acquired mutex after blocking
         └─> Blocked for: 60.2 ms (bounded!)
[  0.165] MEDIUM: ✓ Could NOT preempt Low (Low had inherited priority!)
[  0.265] MEDIUM: Completed after High finished

=== Analysis ===
✓ BOUNDED PRIORITY INVERSION - Success!
  Expected blocking: ~50 ms (Low's work time)
  Actual blocking:   60.2 ms

✓ Medium task did NOT preempt Low during critical section
```

### What's Happening?

1. **Low (P=10)** acquires PI mutex, starts work
2. **High (P=90)** blocks on mutex
3. **Kernel boosts Low to P=90** (priority inheritance!)
4. **Medium (P=50)** tries to run → CANNOT preempt Low (90 > 50)
5. **Low completes** at inherited priority, releases mutex
6. **Low returns to P=10**, High unblocks
7. **Medium finally runs** after High completes

**Solution:** High only blocks for Low's critical section duration!

### Priority Inheritance Protocol

**Rules:**
1. When H blocks on mutex held by L:
   - L inherits H's priority (temporarily)
2. L runs at inherited priority:
   - Cannot be preempted by medium-priority tasks
3. When L releases mutex:
   - L returns to original priority
   - H acquires mutex and runs

**Key Property:** Blocking time is bounded by critical section length

### Code Analysis

Key sections in `exercise5.c`:

```c
// Initialize mutex with priority inheritance
pthread_mutexattr_t mutex_attr;
pthread_mutexattr_init(&mutex_attr);
pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
pthread_mutex_init(&resource_mutex, &mutex_attr);

// When High blocks on this mutex:
// - Low automatically inherits High's priority
// - Kernel handles priority changes
// - No manual intervention needed
```

### Comparison: Exercise 4 vs Exercise 5

| Aspect | Exercise 4 (Regular) | Exercise 5 (PI) |
|--------|---------------------|-----------------|
| Mutex type | Regular | Priority Inheritance |
| High blocking | ~150ms (unbounded) | ~60ms (bounded) |
| Medium interference | YES - preempts Low | NO - cannot preempt |
| Predictability | Unpredictable | Predictable |
| RT suitability | ✗ Dangerous | ✓ Safe |

### Experiments to Try

1. **Increase Low's critical section:**
   - Edit `LOW_WORK_MS` to 200
   - High's blocking increases, but still bounded
   - Still better than unbounded inversion

2. **Compare with Exercise 4:**
   ```bash
   ./exercise4 > ex4.log
   ./exercise5 > ex5.log
   grep "Blocked for" ex4.log ex5.log
   ```

3. **Multiple blocking tasks:**
   - Create multiple high-priority tasks
   - All will boost Low when blocking
   - Observe transitive inheritance

### Questions to Answer
1. How does PI guarantee bounded blocking?
2. What happens if Low holds multiple PI mutexes?
3. Can you calculate the maximum blocking time?
4. When would PI NOT be sufficient?

---

## Advanced Topics

### Transitive Inheritance

What if there's a chain of blocking?

```
H blocks on mutex held by M
M blocks on mutex held by L
```

**PI solution:** L inherits H's priority (transitively through M)

### Priority Ceiling Protocol

Alternative to PI:
- Each mutex has a ceiling priority
- Task acquiring mutex runs at ceiling priority
- Prevents priority inversion entirely
- May cause more blocking than necessary

### Deadlock Prevention

PI doesn't prevent deadlock:
```c
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);  // Risk of deadlock
```

**Solutions:**
- Lock ordering discipline
- Try-lock with timeout
- Deadlock detection

---

## Real-World Examples

### Mars Pathfinder (1997)

**Problem:**
- Information bus task (high priority) blocked by meteorological task (low)
- Communication task (medium) caused priority inversion
- System watchdog reset after long blocking

**Solution:**
- Enabled priority inheritance in VxWorks
- Changed mutex initialization flag
- System ran successfully after patch

### Common RT Systems Using PI

- **Linux PREEMPT_RT:** futex with PI support
- **VxWorks:** PI mutexes standard
- **QNX:** Adaptive partitioning with PI
- **FreeRTOS:** Mutex with priority inheritance
- **RTEMS:** Binary semaphores with PI

---

## Measurement and Analysis

### Timing Measurements

Both exercises measure:
```c
// Record blocking start
clock_gettime(CLOCK_MONOTONIC, &high_block_start);

// Block on mutex
pthread_mutex_lock(&resource_mutex);

// Record blocking end
clock_gettime(CLOCK_MONOTONIC, &high_block_end);

// Calculate blocking time
blocking_time = timespec_diff_ns(&start, &end);
```

### Expected Results

**Exercise 4 (Regular Mutex):**
- High blocking: 100-200ms
- Unbounded - varies with Medium's work
- Medium preempts Low: YES

**Exercise 5 (PI Mutex):**
- High blocking: 50-80ms
- Bounded by Low's critical section
- Medium preempts Low: NO

---

## Summary

### Week 2 Key Takeaways

✓ **Priority Inversion Problem:**
- Occurs with shared resources
- Can be unbounded and unpredictable
- Dangerous for real-time systems

✓ **Priority Inheritance Solution:**
- Temporary priority boost
- Bounds blocking time
- Automatic in kernel

✓ **Best Practices:**
- Always use PI mutexes in RT code
- Keep critical sections short
- Minimize shared resources
- Use lock ordering to prevent deadlock

### Commands Reference

```bash
# Build Week 2 exercises
make week2

# Run individual exercises
make run4    # Priority inversion demo
make run5    # Priority inheritance solution

# Run both
make run-week2

# Build all weeks
make all
make run-all
```

---

## Assignment Ideas

### Part 1: Analysis
1. Compare blocking times between exercise4 and exercise5
2. Calculate the speedup factor
3. Graph blocking time vs critical section length

### Part 2: Modification
1. Add a fourth task with different priority
2. Implement transitive inheritance scenario (3 mutexes, 4 tasks)
3. Create a deadlock scenario and detect it

### Part 3: Critical Thinking
1. When would PI alone not be sufficient?
2. Design a system with 5 tasks and 3 shared resources
3. Prove schedulability with blocking time analysis

---

## Next Steps

**Week 3 Preview:** Advanced Synchronization
- Deadlock detection and prevention
- Lock-free programming
- Read-Copy-Update (RCU)
- Synchronization overhead analysis

## Resources

- Linux futex documentation: `man 2 futex`
- Priority inheritance paper: Sha, Rajkumar, Lehoczky (1990)
- Mars Pathfinder analysis: https://research.microsoft.com/en-us/um/people/mbj/Mars_Pathfinder/
- RTEMS PI implementation: https://docs.rtems.org/
