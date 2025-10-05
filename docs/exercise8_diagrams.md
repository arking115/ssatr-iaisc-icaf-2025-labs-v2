# Exercise 8: Lock-Free Stack - Visual Documentation

## Overview

This document provides visual explanations of the lock-free stack implementation in `exercise8.c` (the safe, phased version).

---

## 1. Lock-Free Stack Architecture

```mermaid
graph TB
    subgraph "LockFreeStack Structure"
        Head[head: _Atomic Node*]
        Retries[cas_retries: _Atomic uint64_t]
    end

    subgraph "Linked List of Nodes"
        NodeA[Node A<br/>┌─────────┐<br/>│ data: 42 │<br/>│ next: ─┐ │<br/>└────────┘]
        NodeB[Node B<br/>┌─────────┐<br/>│ data: 17 │<br/>│ next: ─┐ │<br/>└────────┘]
        NodeC[Node C<br/>┌─────────┐<br/>│ data: 99 │<br/>│ next: NULL │<br/>└────────┘]
    end

    Head -.->|atomic_load| NodeA
    NodeA -->|next| NodeB
    NodeB -->|next| NodeC
    NodeC -->|next| NULL[NULL ⊥]

    Push["lockfree_stack_push()"] -.->|atomic_compare_exchange| Head
    Pop["lockfree_stack_pop()"] -.->|atomic_compare_exchange| Head

    style Head fill:#ffcccc
    style Retries fill:#ccffcc
    style Push fill:#ccccff
    style Pop fill:#ccccff
```

**Key Points:**
- `head` is an atomic pointer - can be read/written atomically
- All modifications use Compare-And-Swap (CAS)
- `cas_retries` tracks contention

---

## 2. Push Operation Flowchart

```mermaid
flowchart TD
    Start([lockfree_stack_push value]) --> Malloc[Allocate new_node<br/>Set new_node->data = value]
    Malloc --> LoadHead[old_head = atomic_load &head]
    LoadHead --> SetNext[new_node->next = old_head]
    SetNext --> CAS{atomic_compare_exchange<br/>head == old_head?}

    CAS -->|SUCCESS<br/>head swapped| Done([Return - Push Complete])

    CAS -->|FAIL<br/>head changed| IncrRetry[Increment cas_retries counter]
    IncrRetry --> LoadHead

    style Start fill:#90EE90
    style Done fill:#90EE90
    style CAS fill:#FFD700
    style IncrRetry fill:#FFA07A
```

**Algorithm Explanation:**
1. Create new node with data
2. Read current head atomically
3. Point new node's `next` to current head
4. Try to CAS: if `head` still equals `old_head`, swap to `new_node`
5. If CAS fails (another thread changed head), retry from step 2
6. If CAS succeeds, push is complete

**Lock-Free Property:** Even if one thread is delayed, others can make progress.

---

## 3. Pop Operation Flowchart

```mermaid
flowchart TD
    Start([lockfree_stack_pop *value]) --> LoadHead[old_head = atomic_load &head]
    LoadHead --> CheckNull{old_head == NULL?}

    CheckNull -->|YES<br/>Stack Empty| ReturnFalse([Return false])

    CheckNull -->|NO<br/>Has nodes| ReadNext[new_head = old_head->next]
    ReadNext --> Validate[current_head = atomic_load &head]
    Validate --> ValidCheck{current_head == old_head?}

    ValidCheck -->|NO<br/>Changed| IncrRetry1[Increment cas_retries]
    IncrRetry1 --> LoadHead

    ValidCheck -->|YES<br/>Still valid| CAS{atomic_compare_exchange<br/>head == old_head?}

    CAS -->|FAIL<br/>Changed| IncrRetry2[Increment cas_retries]
    IncrRetry2 --> LoadHead

    CAS -->|SUCCESS| Extract[*value = old_head->data<br/>free old_head]
    Extract --> ReturnTrue([Return true])

    style Start fill:#90EE90
    style ReturnFalse fill:#FFA07A
    style ReturnTrue fill:#90EE90
    style CAS fill:#FFD700
    style ValidCheck fill:#87CEEB
```

**Algorithm Explanation:**
1. Read current head atomically
2. If NULL, stack is empty - return false
3. Read `old_head->next` to get new head
4. **Validate** head didn't change (prevents accessing freed memory)
5. Try to CAS: if `head` still equals `old_head`, swap to `new_head`
6. If CAS fails, retry from step 1
7. If CAS succeeds, extract value and free node

**Safety Note:** The validation step (green) prevents the race condition where another thread frees the node between reading head and reading next.

---

## 4. Phased Performance Test (Avoiding ABA)

```mermaid
stateDiagram-v2
    [*] --> Initialize: Create stack<br/>Spawn threads
    Initialize --> Phase1: All threads ready

    state Phase1 {
        [*] --> Pushing
        Pushing --> Pushing: Each thread pushes<br/>N/2 items
        Pushing --> [*]: Thread done
    }

    Phase1 --> Barrier: usleep(1000)<br/>Simple sync

    state Phase2 {
        [*] --> Popping
        Popping --> Popping: Each thread pops<br/>N/2 items
        Popping --> [*]: Thread done
    }

    Barrier --> Phase2: All threads finished pushing
    Phase2 --> Aggregate: Collect statistics
    Aggregate --> [*]: Print results

    note right of Phase1
        Only PUSH operations
        - No concurrent pop
        - No ABA risk
        - All nodes stay valid
    end note

    note right of Phase2
        Only POP operations
        - No concurrent push
        - No ABA risk
        - Safe to free nodes
    end note
```

**Why Phased?**
- **Phase 1 (Push only):** All threads only push - no pops happening, so no nodes are freed
- **Barrier:** Simple sleep ensures all threads finish pushing before any start popping
- **Phase 2 (Pop only):** All threads only pop - no new pushes, so no memory reuse

**Result:** ABA problem is avoided because we never have concurrent push/pop!

---

## 5. CAS Retry Sequence (Contention)

```mermaid
sequenceDiagram
    participant T1 as Thread 1
    participant T2 as Thread 2
    participant Stack as Lock-Free Stack<br/>(head)

    Note over Stack: Initial: head → A → B → C

    T1->>Stack: Load head (A)
    T1->>T1: new_node->next = A

    T2->>Stack: Load head (A)
    T2->>T2: new_node->next = A

    T2->>Stack: CAS(head, A, new_B)
    Note over Stack: ✓ SUCCESS<br/>head → new_B → A → B → C

    T1->>Stack: CAS(head, A, new_C)
    Note over Stack: ✗ FAIL<br/>head is new_B, not A!

    T1->>T1: Increment cas_retries

    T1->>Stack: Load head (new_B)
    T1->>T1: new_node->next = new_B

    T1->>Stack: CAS(head, new_B, new_C)
    Note over Stack: ✓ SUCCESS<br/>head → new_C → new_B → A → B → C
```

**What Happened:**
1. Both threads read head (A) simultaneously
2. Thread 2's CAS succeeds first
3. Thread 1's CAS fails (head changed to new_B)
4. Thread 1 retries with updated head value
5. Thread 1's second CAS succeeds

**Lock-Free Guarantee:** Thread 2 made progress even though Thread 1 was delayed!

---

## 6. Comparison: exercise8.c vs exercise8_aba.c

| Aspect | exercise8.c (Safe) | exercise8_aba.c (ABA Demo) |
|--------|-------------------|---------------------------|
| **Test Pattern** | Phased: Push → Barrier → Pop | Concurrent: 50% push, 50% pop interleaved |
| **Operations** | Phase 1: All push<br/>Phase 2: All pop | Each iteration: push OR pop |
| **Concurrency** | Push-only phase, then pop-only phase | Push and pop happening simultaneously |
| **ABA Risk** | ✗ **None** (no concurrent push/pop) | ✓ **High** (memory reuse during CAS) |
| **Memory Management** | ✓ Immediate free() in pop | ✗ Deferred (commented out free) |
| **Memory Leak** | ✗ None | ✓ **All popped nodes leaked** |
| **Crashes** | ✗ None | ✗ None (leak prevents double-free) |
| **CAS Retries** | Lower (less contention) | Higher (more contention) |
| **Purpose** | Demonstrate lock-free performance | Demonstrate ABA problem |
| **Production Readiness** | Closer (phasing is valid pattern) | Educational only |

---

## 7. Performance Metrics Explained

### Throughput
```
Throughput = Total Operations / Execution Time
```
- Measures ops/sec
- Higher = better
- Lock-free often wins under contention

### CAS Retry Rate
```
Retry Rate = cas_retries / Total Operations * 100%
```
- Indicates contention level
- 0% = no contention (all CAS succeed first try)
- 10-20% = moderate contention
- >50% = high contention

### Latency
```
Average Latency = Total Time / Total Operations
```
- Time per operation
- Lower = better
- Lock-free has more predictable worst-case

---

## 8. Thread Timeline (4 Threads, Phased)

```
Time  →
      0ms        500ms      1000ms     1500ms
      │          │          │          │
T1:   [─ Push ─>][── Pop ──>]
T2:   [─ Push ─>][── Pop ──>]
T3:   [─ Push ─>][── Pop ──>]
T4:   [─ Push ─>][── Pop ──>]
      └─Phase 1──┘└─Phase 2──┘
                 ▲
                 │
              Barrier
           (usleep 1ms)

Stack State:
t=0:     Empty
t=500:   Full (all pushes done)
t=1000:  Empty again (all pops done)
```

**Key Insight:** At barrier time, all pushes complete before any pops start!

---

## Summary

**exercise8.c demonstrates:**
- ✅ Lock-free stack works correctly
- ✅ CAS-based synchronization
- ✅ Better performance than mutex under contention
- ✅ No priority inversion
- ✅ Safe memory management (phased approach avoids ABA)

**Not demonstrated here (see exercise8_aba.c):**
- ❌ ABA problem (intentionally avoided)
- ❌ Memory reclamation challenges

**For ABA problem visualization, see `exercise8_aba_diagrams.md`**
