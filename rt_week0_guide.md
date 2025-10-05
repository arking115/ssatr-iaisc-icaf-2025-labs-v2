# Week 0: Introductory Exercises - C Programming Foundations for Real-Time Systems

## Overview

Before diving into real-time system programming (Exercises 1-9), students need to be comfortable with specific C programming concepts and POSIX APIs. This Week 0 series provides **six focused introductory exercises** that build the foundational skills used throughout the RT labs.

**Target Audience:** Students familiar with C programming basics who need preparation for RT-specific APIs and patterns.

**Total Time:** ~90 minutes

---

## Learning Path

### Exercise 0A: Time Basics (15 min)
**File:** `exercises/exercise0a_time_basics.c`

**Core Concepts:**
- High-precision time measurement with `clock_gettime()`
- Understanding `struct timespec` (seconds + nanoseconds)
- Time arithmetic and difference calculation
- Unit conversions (nanoseconds ↔ microseconds ↔ milliseconds)
- `CLOCK_MONOTONIC` vs wall-clock time

**Key Takeaway:** All RT exercises use `CLOCK_MONOTONIC` for precise, non-decreasing time measurements.

**Build & Run:**
```bash
make exercise0a_time_basics
./exercise0a_time_basics
```

---

### Exercise 0B: Simple Threading (20 min)
**File:** `exercises/exercise0b_threading.c`

**Core Concepts:**
- Creating threads with `pthread_create()`
- Waiting for completion with `pthread_join()`
- Thread function signature: `void* func(void* arg)`
- Passing data to threads via structs
- Collecting results from threads

**Key Takeaway:** RT systems run multiple concurrent tasks; pthreads provide the foundation.

**Build & Run:**
```bash
make exercise0b_threading
./exercise0b_threading
```

---

### Exercise 0C: Structs and File I/O (15 min)
**File:** `exercises/exercise0c_file_io.c`

**Core Concepts:**
- Defining custom structs with `typedef`
- Arrays of structs for storing measurements
- File I/O: `fopen()`, `fprintf()`, `fclose()`
- Error handling with NULL checks
- CSV-style output for data analysis

**Key Takeaway:** RT exercises save timing data to files for visualization and analysis.

**Build & Run:**
```bash
make exercise0c_file_io
./exercise0c_file_io
cat results/exercise0c_data.txt
```

---

### Exercise 0D: Command-Line Arguments and Busy-Wait (15 min)
**File:** `exercises/exercise0d_busywait.c`

**Core Concepts:**
- Parsing command-line arguments (`argc`, `argv[]`)
- String comparison with `strcmp()`
- String-to-number conversion (`atoi()`, `atof()`)
- Busy-wait loops (CPU-bound computation)
- `volatile` keyword to prevent compiler optimization

**Key Takeaway:** RT exercises use flags like `--rt` to switch between normal and real-time scheduling.

**Build & Run:**
```bash
make exercise0d_busywait
./exercise0d_busywait --help
./exercise0d_busywait --duration 100 --iterations 5
./exercise0d_busywait --duration 50 --verbose
```

---

### Exercise 0E: Scheduling Introduction (20 min)
**File:** `exercises/exercise0e_scheduling.c`

**Core Concepts:**
- Linux scheduling policies: `SCHED_OTHER`, `SCHED_FIFO`, `SCHED_RR`
- Setting scheduling policy with `sched_setscheduler()`
- Priority ranges: 0 (normal) vs 1-99 (real-time)
- `struct sched_param` for priority configuration
- Handling `EPERM` errors (permission denied)
- Checking capabilities with `sched_get_priority_max()`

**Key Takeaway:** Real-time scheduling requires elevated privileges; RT policies provide deterministic execution.

**Build & Run:**
```bash
make exercise0e_scheduling
./exercise0e_scheduling

# For RT scheduling (requires privileges):
sudo ./exercise0e_scheduling
# OR in Docker:
docker-compose run --rm --privileged rt-lab
./exercise0e_scheduling
```

---

### Exercise 0F: Statistics and Array Manipulation (15 min)
**File:** `exercises/exercise0f_statistics.c`

**Core Concepts:**
- Calculating min, max, average, jitter
- Jitter = max - min (critical RT metric)
- Bubble sort algorithm
- Percentiles (50th, 95th, 99th)
- Distribution analysis
- Formatted table output with `printf`

**Key Takeaway:** RT systems care about worst-case latency (max, 99th percentile), not just averages.

**Build & Run:**
```bash
make exercise0f_statistics
./exercise0f_statistics
```

---

## Quick Start - Run All Week 0 Exercises

```bash
# Build all introductory exercises
make week0

# Run all in sequence
make run-week0

# Or run individually
make run0a  # Time basics
make run0b  # Threading
make run0c  # File I/O
make run0d  # Busy-wait
make run0e  # Scheduling
make run0f  # Statistics
```

---

## Concept-to-Exercise Mapping

| C Concept | Introduced in | Used in Main Exercises |
|-----------|---------------|------------------------|
| `clock_gettime()`, `struct timespec` | 0A | Exercise 1, 2, 3 (all timing) |
| `pthread_create()`, `pthread_join()` | 0B | Exercise 2 (multi-policy threading) |
| Structs, arrays, file I/O | 0C | Exercise 1, 3 (saving results) |
| `argc`/`argv`, busy-wait, `volatile` | 0D | Exercise 2, 3 (computation, flags) |
| `sched_setscheduler()`, RT policies | 0E | Exercise 2, 3 (RT scheduling) |
| Min/max/avg, jitter, sorting | 0F | Exercise 1, 2, 3 (analysis) |

---

## How These Skills Connect to Main Exercises

### Exercise 1: Latency Measurement
- Uses: 0A (time measurement), 0C (file I/O), 0F (statistics)
- **Pattern:** Measure `clock_nanosleep()` precision, calculate min/max/jitter, save to file

### Exercise 2: Scheduling Policies
- Uses: 0B (threading), 0D (busy-wait), 0E (scheduling), 0F (sorting)
- **Pattern:** Create 3 threads with different policies, compare execution times

### Exercise 3: Periodic Tasks
- Uses: 0A (time), 0D (command-line), 0E (RT scheduling), 0F (statistics)
- **Pattern:** Simulate control loop, detect deadline misses, compare normal vs RT

---

## Key C Idioms for Real-Time Programming

### 1. Time Arithmetic Pattern
```c
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * NSEC_PER_SEC) + diff_nsec;
}
```
*Used in: 0A, and Exercises 1, 2, 3*

### 2. Thread Data Passing Pattern
```c
typedef struct {
    int thread_id;      // Input
    int config;         // Input
    long result;        // Output
} thread_data_t;

void* worker(void* arg) {
    thread_data_t *data = (thread_data_t *)arg;
    // ... use data->config ...
    data->result = computed_value;
    return data;
}
```
*Used in: 0B, and Exercise 2*

### 3. RT Scheduling Pattern
```c
struct sched_param param;
param.sched_priority = 80;

if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
    perror("sched_setscheduler");
    // Handle error or fall back to SCHED_OTHER
}
```
*Used in: 0E, and Exercises 2, 3*

### 4. Busy-Wait Pattern (for known computation time)
```c
void do_computation(long duration_usec) {
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    volatile long sum = 0;
    do {
        for (int i = 0; i < 1000; i++) sum += i;
        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (timespec_diff_ns(&start, &now) < duration_usec * 1000L);
}
```
*Used in: 0D, and Exercises 2, 3*

### 5. Statistical Analysis Pattern
```c
long min = LONG_MAX, max = 0, total = 0;
for (int i = 0; i < count; i++) {
    total += samples[i];
    if (samples[i] < min) min = samples[i];
    if (samples[i] > max) max = samples[i];
}
long avg = total / count;
long jitter = max - min;
```
*Used in: 0F, and Exercises 1, 3*

---

## Common Pitfalls and Best Practices

### ✓ DO:
- Always check return values from system calls
- Use `CLOCK_MONOTONIC` for timing (never `CLOCK_REALTIME` for RT)
- Declare timing loop variables as `volatile` to prevent optimization
- Initialize structs with designated initializers: `.field = value`
- Close files with `fclose()` after opening

### ✗ DON'T:
- Assume sleep/nanosleep durations are exact
- Forget to link `-pthread -lrt` flags
- Mix seconds and nanoseconds without conversion
- Use `printf` inside timing-critical loops (adds latency)
- Run RT scheduling (`SCHED_FIFO`) without yields/sleeps (can lock system!)

---

## Environment Setup

### Docker (Recommended for Labs)
```bash
# Build container with RT capabilities
docker-compose build

# Run with RT scheduling enabled
docker-compose run --rm --privileged rt-lab

# Inside container:
make week0
make run-week0
```

### Native Linux
```bash
# Install dependencies
sudo apt-get install build-essential

# Compile
make week0

# Run (RT scheduling requires privileges)
./exercise0a_time_basics
sudo ./exercise0e_scheduling  # Needs sudo for RT
```

---

## Progression Summary

```
Week 0 (Foundations)          Week 1 (RT Fundamentals)
├── 0A: Time Basics          → Exercise 1: Latency Measurement
├── 0B: Threading            → Exercise 2: Scheduling Comparison
├── 0C: File I/O             → Exercise 3: Periodic Tasks
├── 0D: Busy-Wait            ↗
├── 0E: Scheduling           ↗
└── 0F: Statistics           ↗
```

**After Week 0, you'll be ready to:**
1. Measure timing with nanosecond precision
2. Create and manage concurrent tasks
3. Configure real-time scheduling policies
4. Analyze system performance with statistical rigor
5. Build and debug real-time applications

---

## Additional Resources

### Makefile Targets
```bash
make help              # Show all available commands
make week0             # Build all Week 0 exercises
make run-week0         # Run all Week 0 exercises
make clean             # Remove compiled binaries
```

### Next Steps
- Complete all Week 0 exercises sequentially (0A → 0F)
- Experiment with parameters (e.g., duration in 0D, iterations in 0F)
- Read source code comments for detailed explanations
- Proceed to Exercise 1 once comfortable with these concepts

### References
- [POSIX Threads Programming](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Linux Scheduling Policies](https://man7.org/linux/man-pages/man7/sched.7.html)
- [clock_gettime(2)](https://man7.org/linux/man-pages/man2/clock_gettime.2.html)
- [Real-Time Linux](https://wiki.linuxfoundation.org/realtime/start)

---

## Questions & Troubleshooting

**Q: Exercise 0E fails with "Operation not permitted"?**
A: RT scheduling requires elevated privileges. Run with `sudo` or in privileged Docker container.

**Q: Timing measurements show high jitter?**
A: Expected in Docker/virtualized environments. Focus on relative differences (RT vs normal).

**Q: Can't create results directory?**
A: Run `mkdir -p results` or let Exercise 0C create it automatically.

**Q: Compilation errors with pthread functions?**
A: Ensure `-pthread` flag is used: `gcc -pthread -lrt file.c -o output`

---

**Ready to start?** Run `make run-week0` and follow the exercises in order!
