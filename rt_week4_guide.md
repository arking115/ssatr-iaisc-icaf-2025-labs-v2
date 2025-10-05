# Real-Time Systems - Week 4 Lab Guide

## Overview
Week 4 focuses on **lock-free programming** - an alternative approach to synchronization that avoids traditional mutexes and their problems (priority inversion, deadlock, blocking).

## Learning Objectives
- Understand lock-free vs wait-free vs blocking algorithms
- Master atomic operations and Compare-And-Swap (CAS)
- Implement lock-free data structures (stack)
- Recognize the ABA problem and its solutions
- Analyze performance: locks vs lock-free approaches
- Understand memory ordering and barriers

## Background: Lock-Free Programming

### What is Lock-Free?

**Lock-Free:** At least one thread makes progress in a finite number of steps, even if others are delayed.
- No locks/mutexes required
- Based on atomic hardware instructions
- Thread delays don't block others
- **Not** the same as "lock-less" (without locks)

**Wait-Free:** Every thread makes progress in a finite number of steps.
- Stronger guarantee than lock-free
- Harder to implement
- Best theoretical guarantee

**Blocking (Traditional):** Threads can block indefinitely waiting for locks.
- Can cause priority inversion
- Can cause deadlock
- Simpler to implement

### Progress Guarantees Hierarchy

```
Wait-Free (strongest)
    ↓
Lock-Free
    ↓
Obstruction-Free
    ↓
Blocking (weakest)
```

### Why Lock-Free?

**Advantages:**
- ✓ No priority inversion
- ✓ No deadlock possible
- ✓ Better scalability (no lock contention)
- ✓ Thread failure doesn't block system
- ✓ Often better performance under high contention

**Disadvantages:**
- ✗ Complex to implement correctly
- ✗ ABA problem
- ✗ Memory ordering challenges
- ✗ May waste CPU on retries
- ✗ Harder to reason about correctness

### When to Use Lock-Free?

**Good candidates:**
- High contention scenarios
- RT systems where blocking is unacceptable
- Systems where priority inversion is problematic
- Scalable multi-core performance needed

**Poor candidates:**
- Complex data structures (trees, graphs)
- When simplicity is paramount
- Low contention scenarios
- When locks perform well enough

---

## Atomic Operations

### Compare-And-Swap (CAS)

The fundamental lock-free primitive:

```c
bool CAS(T* ptr, T expected, T new_value) {
    atomic {
        if (*ptr == expected) {
            *ptr = new_value;
            return true;  // Success
        }
        return false;  // Failed - value changed
    }
}
```

**GCC/Clang Builtin:**
```c
bool __atomic_compare_exchange_n(
    T* ptr,
    T* expected,      // Updated on failure!
    T desired,
    bool weak,        // Allow spurious failures
    int success_memorder,
    int failure_memorder
);
```

**Example Usage:**
```c
int old_value = 10;
int new_value = 20;
if (__atomic_compare_exchange_n(&shared_var, &old_value, new_value,
                                  false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
    // Success: shared_var was 10, now 20
} else {
    // Failure: shared_var wasn't 10
    // old_value now contains actual value
}
```

### Other Atomic Operations

**Atomic Load:**
```c
int value = __atomic_load_n(&var, __ATOMIC_SEQ_CST);
```

**Atomic Store:**
```c
__atomic_store_n(&var, new_value, __ATOMIC_SEQ_CST);
```

**Atomic Fetch-and-Add:**
```c
int old = __atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST);
```

**Atomic Exchange:**
```c
int old = __atomic_exchange_n(&var, new_value, __ATOMIC_SEQ_CST);
```

### Memory Ordering

**Memory Order Modes:**
- `__ATOMIC_SEQ_CST` - Sequential consistency (strictest, slowest)
- `__ATOMIC_ACQUIRE` - Acquire semantics (loads)
- `__ATOMIC_RELEASE` - Release semantics (stores)
- `__ATOMIC_ACQ_REL` - Both acquire and release
- `__ATOMIC_RELAXED` - No ordering constraints (fastest)

**For RT systems:** Use `__ATOMIC_SEQ_CST` initially for correctness. Optimize later if needed.

---

## Exercise 8: Lock-Free Stack Implementation

### Concept
Implement a lock-free stack using CAS operations and compare with mutex-based implementation.

### Run the Exercise
```bash
make exercise8
./exercise8
```

### Expected Output

```
=== Exercise 8: Lock-Free Stack Implementation ===

Part 1: Correctness Testing
----------------------------
Testing mutex-based stack...
  Pushing 1000 items from 4 threads...
  Popping all items...
  ✓ All 4000 items accounted for
  ✓ No duplicates found
  ✓ Mutex stack: CORRECT

Testing lock-free stack...
  Pushing 1000 items from 4 threads...
  Popping all items...
  ✓ All 4000 items accounted for
  ✓ No duplicates found
  ✓ Lock-free stack: CORRECT

Part 2: Performance Testing
----------------------------
Scenario: 4 threads, 100000 operations each

Mutex-based stack:
  Total operations: 400000
  Time: 1.245 seconds
  Throughput: 321285 ops/sec
  Avg latency: 3.11 µs
  Lock contention detected: 45623 times

Lock-free stack:
  Total operations: 400000
  Time: 0.856 seconds
  Throughput: 467290 ops/sec
  Avg latency: 2.14 µs
  CAS retries: 12456

Performance Improvement:
  ✓ 45.4% faster
  ✓ 31.2% lower latency
  ✓ No blocking or priority inversion

=== Analysis ===
Lock-free stack advantages:
  • No priority inversion risk
  • No deadlock possible
  • Better scalability (no lock contention)
  • Consistent performance under load

Trade-offs:
  • More complex implementation
  • CAS retries consume CPU
  • Memory ordering requirements
```

### Algorithm Explanation

**Lock-Free Stack Structure:**
```c
typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;  // Atomic pointer
} LockFreeStack;
```

**Push Operation:**
```c
void push(LockFreeStack* stack, int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = value;

    Node* old_head;
    do {
        old_head = __atomic_load_n(&stack->head, __ATOMIC_SEQ_CST);
        new_node->next = old_head;
    } while (!__atomic_compare_exchange_n(
        &stack->head,
        &old_head,     // Expected value
        new_node,      // New value
        false,         // Strong CAS
        __ATOMIC_SEQ_CST,
        __ATOMIC_SEQ_CST
    ));
}
```

**How it works:**
1. Create new node
2. Read current head atomically
3. Set new node's next to current head
4. Try to CAS head from old_head to new_node
5. If CAS fails (head changed), retry from step 2
6. If CAS succeeds, push complete

**Pop Operation:**
```c
bool pop(LockFreeStack* stack, int* value) {
    Node* old_head;
    Node* new_head;

    do {
        old_head = __atomic_load_n(&stack->head, __ATOMIC_SEQ_CST);
        if (old_head == NULL) {
            return false;  // Stack empty
        }
        new_head = old_head->next;
    } while (!__atomic_compare_exchange_n(
        &stack->head,
        &old_head,
        new_head,
        false,
        __ATOMIC_SEQ_CST,
        __ATOMIC_SEQ_CST
    ));

    *value = old_head->data;
    free(old_head);  // WARNING: ABA problem!
    return true;
}
```

**How it works:**
1. Read current head atomically
2. If head is NULL, stack is empty (return false)
3. Read next node (new head after pop)
4. Try to CAS head from old_head to new_head
5. If CAS fails (head changed), retry from step 1
6. If CAS succeeds, extract value and free node

### The ABA Problem

**Scenario:**
```
Initial state: head → A → B → C

Thread 1:                    Thread 2:
1. Read head (A)
2. Read A->next (B)
3. [PREEMPTED]
                             4. Pop A (head = B)
                             5. Pop B (head = C)
                             6. Push A (head = A)
                                Now: head → A → C
7. CAS(head, A, B)
   ✓ SUCCESS! (head was A)
   Now: head → B
   BUT B->next is garbage!
```

**Problem:** Head looks the same (A), but it's not the *same* A!

**Solutions:**

1. **Tagged Pointers / Version Numbers:**
```c
typedef struct {
    Node* ptr;
    uint64_t counter;
} TaggedPointer;

// CAS includes counter check
```

2. **Hazard Pointers:**
- Mark nodes as "in use" before dereferencing
- Delay deallocation until safe

3. **Epoch-Based Reclamation:**
- Track epochs/generations
- Defer freeing until all threads advance

4. **Garbage Collection:**
- Automatic memory management
- No manual free()

**For Exercise 8:** We use a simplified approach suitable for demonstration. Production code needs proper ABA protection!

### Code Structure

**File: exercises/exercise8.c**

Main sections:
1. Mutex-based stack (baseline)
2. Lock-free stack (CAS-based)
3. Correctness testing
4. Performance benchmarking
5. Statistical analysis

### Experiments to Try

1. **Vary thread count:**
   ```c
   #define NUM_THREADS 8  // Increase contention
   ```
   - Lock-free advantage increases with more threads

2. **Vary operation count:**
   ```c
   #define OPS_PER_THREAD 1000000
   ```
   - Test scalability

3. **Add delays (simulate work):**
   ```c
   usleep(10);  // After each operation
   ```
   - Changes contention patterns

4. **Measure CAS retry rate:**
   - Count CAS failures
   - Indicates contention level

5. **Test under RT priority:**
   ```bash
   chrt -f 80 ./exercise8
   ```
   - See priority inversion effect with mutex version

### Questions to Answer

1. When does lock-free outperform mutex-based?
2. What is the CAS retry rate under high contention?
3. How does performance scale with thread count?
4. What are the memory ordering requirements?
5. How would you detect the ABA problem in practice?

---

## Exercise 9: Comprehensive Performance Analysis

### Concept
Systematic comparison of synchronization approaches under various workloads and priorities.

### Run the Exercise
```bash
make exercise9
./exercise9
```

### Expected Output

```
=== Exercise 9: Synchronization Performance Analysis ===

Test Configuration:
  Threads: 4
  Operations per thread: 100000
  Total operations: 400000

Test 1: Low Contention (90% read, 10% write)
--------------------------------------------
Mutex (PI):
  Time: 0.523 sec
  Throughput: 765027 ops/sec
  Avg latency: 1.31 µs
  Priority inversions: 0 (PI mutex)

Lock-Free:
  Time: 0.412 sec
  Throughput: 970874 ops/sec
  Avg latency: 1.03 µs
  CAS retries: 3245 (3.2%)

Winner: Lock-free (26.9% faster)

Test 2: High Contention (50% read, 50% write)
---------------------------------------------
Mutex (PI):
  Time: 1.834 sec
  Throughput: 218132 ops/sec
  Avg latency: 4.58 µs
  Lock hold time: avg 3.2 µs, max 125 µs

Lock-Free:
  Time: 1.245 sec
  Throughput: 321285 ops/sec
  Avg latency: 3.11 µs
  CAS retries: 45623 (11.4%)

Winner: Lock-free (47.3% faster)

Test 3: Mixed Priority Tasks
-----------------------------
Scenario: High-priority reader + Low-priority writers

Mutex (Regular):
  High-priority blocked: 34 times
  Max blocking: 234 µs
  ✗ Priority inversion detected!

Mutex (PI):
  High-priority blocked: 34 times
  Max blocking: 45 µs
  ✓ Bounded blocking (PI working)

Lock-Free:
  High-priority blocked: 0 times
  Max CAS retries: 12
  ✓ No blocking ever!

Winner: Lock-free (zero priority inversion)

Test 4: Deadline Sensitivity
-----------------------------
Scenario: Periodic tasks with 10ms deadlines

Mutex approach:
  Deadline misses: 23 (5.75%)
  Worst-case latency: 12.3 ms
  ✗ Deadlines missed due to blocking

Lock-free approach:
  Deadline misses: 0 (0%)
  Worst-case latency: 8.7 ms
  ✓ All deadlines met

Winner: Lock-free (RT guarantees)

=== Summary ===

Lock-Free Advantages:
  ✓ 26-47% better throughput
  ✓ Zero priority inversion
  ✓ Better worst-case latency
  ✓ Scalable with threads
  ✓ No deadlock risk

Mutex Advantages:
  ✓ Simpler to implement
  ✓ Lower CPU usage (no retries)
  ✓ Easier to reason about
  ✓ PI bounds blocking time

Recommendation:
  • High contention + RT: Use lock-free
  • Low contention + simplicity: Use mutex with PI
  • Critical RT paths: Use lock-free
  • Complex structures: Use mutex with careful design
```

### Test Scenarios

**Test 1: Read-Heavy Workload**
- 90% reads, 10% writes
- Simulates caches, lookup tables
- Low contention on writes

**Test 2: Write-Heavy Workload**
- 50% reads, 50% writes
- High contention scenario
- Stresses synchronization

**Test 3: Priority Inversion Test**
- High-priority reader task
- Low-priority writer tasks
- Shows PI effects

**Test 4: Real-Time Deadline Test**
- Periodic tasks with deadlines
- Measures deadline miss rate
- Shows predictability

### Code Structure

**File: exercises/exercise9.c**

Implements:
1. Counter with mutex protection
2. Counter with lock-free CAS
3. Multiple test workloads
4. Statistical collection
5. Priority inversion detection
6. Deadline monitoring

### Metrics Collected

**Performance:**
- Throughput (ops/sec)
- Latency (average, min, max, percentiles)
- Execution time

**Synchronization:**
- CAS retry count
- Lock contention events
- Priority inversion occurrences

**RT Characteristics:**
- Deadline miss count
- Worst-case response time
- Jitter

### Experiments to Try

1. **CPU affinity:**
   ```c
   // Pin threads to different cores
   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(core_id, &cpuset);
   pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
   ```

2. **NUMA effects:**
   - Test on multi-socket systems
   - Measure cross-socket atomic costs

3. **Memory ordering relaxation:**
   ```c
   // Try relaxed ordering
   __atomic_load_n(&var, __ATOMIC_RELAXED);
   ```

4. **Backoff strategies:**
   ```c
   // Exponential backoff on CAS failure
   for (int i = 0; i < retries; i++) {
       usleep(1 << i);  // 1, 2, 4, 8... µs
   }
   ```

5. **Hybrid approach:**
   - Lock-free for RT critical path
   - Locks for non-critical operations

### Questions to Answer

1. At what contention level does lock-free win?
2. How does CPU count affect results?
3. What's the cost of sequential consistency?
4. When would you choose locks over lock-free?
5. How do you profile CAS retry overhead?

---

## Advanced Topics

### Lock-Free Queue

More complex than stack - needs careful ABA handling:

```c
typedef struct QNode {
    int data;
    struct QNode* next;
} QNode;

typedef struct {
    QNode* head;
    QNode* tail;
} LockFreeQueue;
```

**Challenges:**
- Two pointers to update (head and tail)
- More complex CAS sequences
- Dummy node often required
- ABA problem more severe

### Memory Reclamation Strategies

**1. Hazard Pointers:**
```c
// Thread announces "I'm using this pointer"
hazard_pointer[thread_id] = ptr;
memory_barrier();
// Use ptr safely
hazard_pointer[thread_id] = NULL;
```

**2. Epoch-Based Reclamation:**
```c
global_epoch = 0;
thread_local_epoch[tid] = 0;

// Periodically:
global_epoch++;
// Free nodes from epoch < min(thread_local_epoch)
```

**3. Reference Counting:**
```c
typedef struct {
    Node* ptr;
    uint32_t refcount;
} RCPointer;
```

### Double-Width CAS (DWCAS)

CAS on two adjacent words atomically:

```c
// x86-64: CMPXCHG16B instruction
__int128 old, new;
__atomic_compare_exchange_n((__int128*)ptr, &old, new, ...);
```

**Use cases:**
- Tagged pointers (pointer + counter)
- Queue head+tail
- ABA prevention

**Portability:** Not all architectures support DWCAS!

### Memory Barriers and Fences

**Compiler Barrier:**
```c
asm volatile("" ::: "memory");  // Prevent compiler reordering
```

**Memory Fence:**
```c
__atomic_thread_fence(__ATOMIC_SEQ_CST);  // Full barrier
```

**When needed:**
- Publishing data structures
- Ensuring visibility across threads
- Implementing custom synchronization

---

## Common Pitfalls and Solutions

### Pitfall 1: ABA Problem

**Symptom:** Intermittent corruption, usually under high load

**Detection:**
```c
// Add validation checks
assert(node->next->prev == node);
```

**Solution:** Use tagged pointers or hazard pointers

### Pitfall 2: Memory Ordering Bugs

**Symptom:** Works on x86, fails on ARM/PowerPC

**Detection:**
- Test on weak memory model architectures
- Use ThreadSanitizer

**Solution:** Use proper memory ordering modes

### Pitfall 3: Livelock

**Symptom:** Threads keep retrying, making no progress

**Detection:**
```c
if (retries > MAX_RETRIES) {
    log_warning("Excessive retries detected");
}
```

**Solution:** Exponential backoff, fallback to locks

### Pitfall 4: Memory Leaks

**Symptom:** Memory usage grows over time

**Detection:** Valgrind, AddressSanitizer

**Solution:** Proper reclamation scheme (hazard pointers, epochs)

### Pitfall 5: False Sharing

**Symptom:** Poor scalability despite lock-free design

**Detection:** `perf c2c` (cache-to-cache transfer analysis)

**Solution:** Pad structures to cache line boundaries

```c
struct {
    atomic_int counter;
    char padding[64 - sizeof(atomic_int)];
} __attribute__((aligned(64)));
```

---

## Best Practices

### Design Guidelines

1. **Start with locks** - Optimize only if needed
2. **Measure first** - Profile before going lock-free
3. **Keep it simple** - Simpler lock-free algorithms (stack) before complex ones (tree)
4. **Formal verification** - Use model checkers for correctness
5. **Test extensively** - Concurrency bugs are rare and hard to reproduce

### Implementation Checklist

- [ ] Use sequential consistency initially
- [ ] Add ABA protection (tags or hazard pointers)
- [ ] Test on weak memory models
- [ ] Use ThreadSanitizer
- [ ] Profile CAS retry rates
- [ ] Document memory ordering requirements
- [ ] Plan memory reclamation strategy
- [ ] Pad structures to avoid false sharing

### Testing Strategy

**Unit Tests:**
- Single-threaded correctness
- Sequential behavior

**Concurrency Tests:**
- Multi-threaded stress tests
- Randomized operation sequences
- Long-duration runs

**Model Checking:**
- Spin, TLA+, or similar
- Exhaustive state exploration
- Proof of correctness

---

## Real-World Examples

### Linux Kernel

**RCU (Read-Copy-Update):**
- Lock-free reads
- Synchronize updates with epochs
- Used extensively in networking

**Per-CPU Counters:**
- Lock-free increment
- Relaxed memory ordering
- Aggregate on read

### Userspace Libraries

**Folly (Facebook):**
- Lock-free queues and stacks
- Hazard pointer implementation
- Production-tested

**libcds (Concurrent Data Structures):**
- Wide variety of lock-free structures
- Multiple memory reclamation schemes

**Boost.Lockfree:**
- Stack, queue, SPSC queue
- Portable across platforms

---

## Summary

### Week 4 Key Takeaways

✓ **Lock-Free Fundamentals:**
- CAS as basic primitive
- Progress guarantees (wait-free > lock-free > blocking)
- When to use lock-free vs locks

✓ **Implementation:**
- Lock-free stack algorithm
- ABA problem and solutions
- Memory ordering requirements

✓ **Performance:**
- Often faster under high contention
- Better RT characteristics (no blocking)
- Trade-off: complexity vs performance

✓ **Best Practices:**
- Start simple, optimize when needed
- Proper testing is essential
- Memory reclamation is critical

### Commands Reference

```bash
# Build Week 4 exercises
make week4

# Run individual exercises
make run8    # Lock-free stack
make run9    # Performance comparison

# Run all Week 4
make run-week4

# With ThreadSanitizer
make CFLAGS="-fsanitize=thread" exercise8
./exercise8

# Performance profiling
perf stat -e cache-misses,cycles ./exercise9
```

---

## Assignment Ideas

### Part 1: Implementation
1. Implement lock-free queue (FIFO)
2. Add hazard pointers to Exercise 8
3. Implement exponential backoff on CAS failure

### Part 2: Analysis
1. Measure CAS retry rate vs thread count
2. Compare memory orderings (relaxed vs seq_cst)
3. Profile cache miss rates

### Part 3: Advanced
1. Implement epoch-based memory reclamation
2. Port to ARM and compare with x86
3. Build lock-free hash table

---

## Next Steps

**Week 5 Preview:** Real-Time Communication
- Message queues for RT systems
- Lock-free bounded queues
- Priority-aware messaging
- Zero-copy techniques

## Resources

- "The Art of Multiprocessor Programming" - Herlihy & Shavit
- "Is Parallel Programming Hard?" - Paul McKenney (RCU expert)
- Linux kernel RCU documentation
- Intel® 64 Architecture Memory Ordering White Paper
- ARM Memory Model documentation
- C11/C++11 atomic operations reference
