# 3-Thread Real-Time Application Demo

## Overview

This demo application showcases a multi-threaded real-time system with three periodic tasks implementing **Rate Monotonic Scheduling (RMS)**. It simulates a typical embedded control system with sensor reading, control computation, and data logging.

## Architecture

### Thread Design

The application consists of three periodic threads with different priorities based on RMS principles:

| Thread | Role | Period | Computation | Priority | Utilization |
|--------|------|--------|-------------|----------|-------------|
| Thread 1 | Sensor | 5 ms | 800 µs | 90 (highest) | 16% |
| Thread 2 | Controller | 10 ms | 2000 µs | 80 (medium) | 20% |
| Thread 3 | Logger | 20 ms | 3000 µs | 70 (lowest) | 15% |

**Total System Utilization**: ~51%

### Rate Monotonic Scheduling (RMS)

RMS assigns priorities based on task periods:
- **Shorter period → Higher priority**
- Thread 1 (5ms) gets priority 90
- Thread 2 (10ms) gets priority 80
- Thread 3 (20ms) gets priority 70

**Schedulability**: For 3 tasks, RMS guarantees schedulability if utilization ≤ 78% (Liu & Layland bound). Our system uses ~51%, so it's safely schedulable.

### Data Flow

```
Sensor Thread → reads data → shared_data.sensor_value
                                    ↓
Controller Thread → processes → shared_data.control_output
                                    ↓
Logger Thread → logs system state → shared_data.log_counter
```

## Building and Running

### Quick Start

```bash
# Inside Docker container or on Linux with RT support
cd demo

# Build
make all

# Run with normal scheduling
make run

# Run with RT scheduling (requires --privileged)
make run-rt
```

### Docker Environment

```bash
# From project root
docker-compose run --rm --privileged rt-lab

# Inside container
cd demo
make run-rt
```

### Manual Execution

```bash
# Build manually
gcc -Wall -Wextra -O2 -pthread -lm -o sample_3threads sample_3threads.c -lrt

# Run options
./sample_3threads                    # Normal scheduling
./sample_3threads --rt               # RT scheduling (SCHED_FIFO)
./sample_3threads --rt --duration 10 # Run for 10 seconds
./sample_3threads --help             # Show options
```

## Command-Line Options

- `--rt` - Enable real-time scheduling (SCHED_FIFO). Requires `CAP_SYS_NICE` capability or `--privileged` container.
- `--duration SEC` - Set run duration in seconds (default: 5)
- `--help` - Display help message

## Understanding the Output

### Configuration Section

```
=== Thread Configuration ===
Thread | Name       | Period | Computation | Utilization | Priority
-------|------------|--------|-------------|-------------|----------
  0    | Sensor     |    5 ms|     800 µs  |    16.0%    |    90
  1    | Controller |   10 ms|    2000 µs  |    20.0%    |    80
  2    | Logger     |   20 ms|    3000 µs  |    15.0%    |    70

Total Utilization: 51.0%
RMS Schedulability Bound: 78.0%
✓ System is schedulable under RMS
```

This confirms the system design meets RMS schedulability criteria.

### Results Summary

```
=== Results Summary ===

Thread | Name       | Cycles | Misses | Miss % | Min Lat | Max Lat | Avg Lat | Jitter
-------|------------|--------|--------|---------|---------|---------|---------|--------
  0    | Sensor     |   1000 |      0 |   0.00% |     8 µs|    45 µs|    12 µs|   37 µs
  1    | Controller |    500 |      0 |   0.00% |    10 µs|    52 µs|    15 µs|   42 µs
  2    | Logger     |    250 |      0 |   0.00% |    15 µs|    89 µs|    28 µs|   74 µs
```

**What to look for:**
- **Misses**: Should be 0 for schedulable system
- **Latency**: Higher priority threads typically have lower latency
- **Jitter**: RT scheduling significantly reduces jitter vs normal scheduling
- **Priority inversion**: Lower priority threads should show more variability

## Experiments

### 1. Normal vs RT Scheduling Comparison

```bash
make compare
```

This runs the application twice (normal and RT scheduling) and shows the difference. RT scheduling should demonstrate:
- Lower average latency
- Significantly reduced jitter
- More deterministic behavior

### 2. System Under Load

```bash
make run-stressed
```

Runs the application with CPU stress in the background. RT scheduling should maintain performance while normal scheduling degrades significantly.

### 3. Extended Duration Test

```bash
make run-long
```

Runs for 10 seconds to collect more statistics and observe long-term behavior.

### 4. Manual Experiments

**Increase computation time:**
1. Edit `sample_3threads.c`
2. Change `computation_usec` values in `init_thread_configs()`
3. Increase values until deadline misses occur
4. Rebuild: `make all`
5. Test: `make run-rt`

**Observe priority inversion:**
- Run under load: `stress-ng --cpu 4 & ./sample_3threads --rt`
- Lower priority threads (Logger) should miss more deadlines
- Higher priority threads (Sensor) should remain stable

**Change priorities:**
- Modify priority values in `init_thread_configs()`
- Try violating RMS: give longer period tasks higher priority
- Observe increased deadline misses and jitter

## Key Concepts Demonstrated

### 1. Rate Monotonic Scheduling (RMS)
- Static priority assignment based on task periods
- Optimal fixed-priority algorithm for periodic tasks
- Schedulability bound: U ≤ n(2^(1/n) - 1)
- For 3 tasks: U ≤ 0.78 (78%)

### 2. Real-Time Scheduling
- `SCHED_FIFO`: Real-time FIFO scheduling policy
- Fixed priorities (1-99, higher number = higher priority)
- Preemptive: higher priority tasks preempt lower priority
- No time-slicing within same priority

### 3. Periodic Task Model
- Fixed period for each task
- Computation time << period
- Absolute deadline = next period start
- Deadline detection and miss counting

### 4. Timing Analysis
- **Latency**: Delay between intended and actual activation
- **Jitter**: Variation in latency (max - min)
- **Deadline Miss**: Computation extends beyond next period
- **Utilization**: Sum of (computation_time / period)

### 5. Multi-threaded RT Systems
- Thread synchronization via shared data
- Priority-based execution order
- Predictable timing behavior
- Statistics collection per thread

## Implementation Details

### Time Management
- Uses `CLOCK_MONOTONIC` for precise timing
- `clock_nanosleep()` with `TIMER_ABSTIME` for absolute timing
- Nanosecond precision calculations
- Avoids drift accumulation

### Scheduling Setup
```c
struct sched_param param;
param.sched_priority = priority;
pthread_setschedparam(thread, SCHED_FIFO, &param);
```

### Periodic Execution Pattern
```c
clock_gettime(CLOCK_MONOTONIC, &next_activation);
for (each cycle) {
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);
    // Do work
    next_activation += period;  // Maintain absolute timing
}
```

### Deadline Detection
```c
deadline = next_activation + period;
clock_gettime(CLOCK_MONOTONIC, &completion);
if (completion > deadline) {
    // Deadline miss!
}
```

## Troubleshooting

### "Operation not permitted" Error

**Cause**: Insufficient privileges to set RT priorities

**Solutions:**
- Run container with `--privileged` flag
- Or add `--cap-add=SYS_NICE`
- Or set `ulimit -r 99` for RT priority limit

### High Jitter Even with RT Scheduling

**Cause**: Running in non-RT kernel (standard Linux/Docker)

**Explanation**: Container shares host kernel. Without `PREEMPT_RT` kernel patch, results show relative improvements rather than absolute RT guarantees. This is expected and still demonstrates RMS concepts.

### Compilation Errors

**Missing libm**: Add `-lm` to LDFLAGS
**Missing librt**: Add `-lrt` to LDFLAGS
**pthread errors**: Ensure `-pthread` is in both CFLAGS and LDFLAGS

## Educational Value

This demo is designed for learning RT systems concepts:

1. **RMS Algorithm**: See priority assignment in action
2. **Schedulability Analysis**: Calculate and verify utilization bounds
3. **RT vs Normal**: Compare determinism with/without RT scheduling
4. **Priority Effects**: Observe how priorities affect latency and misses
5. **System Load**: Test behavior under stress
6. **Measurement**: Collect and analyze timing statistics

## Next Steps

After understanding this demo:

1. **Week 1 Exercises**: Complete exercises 1-3 for deeper RT fundamentals
2. **Week 2**: Priority inversion and inheritance (exercises 4-5)
3. **Week 3**: Deadlock prevention (exercises 6-7)
4. **Week 4**: Lock-free programming (exercises 8-9)

## References

- Liu & Layland (1973): "Scheduling Algorithms for Multiprogramming in a Hard Real-Time Environment"
- `man 7 sched` - Linux scheduling policies
- `man 2 sched_setscheduler` - Setting scheduling parameters
- `man 2 clock_nanosleep` - High-precision sleep

## Performance Expectations

**In Docker (non-RT kernel):**
- Average latency: 10-50 µs
- Jitter: 50-200 µs (normal), 20-100 µs (RT)
- Deadline misses: None for default configuration
- RT shows 2-5x jitter reduction vs normal

**On PREEMPT_RT kernel:**
- Average latency: 5-20 µs
- Jitter: 5-30 µs
- Near-deterministic behavior
- Suitable for hard RT requirements

## Code Structure

```
sample_3threads.c (490 lines)
├── Configuration defines
├── Data structures (thread_config_t, cycle_stat_t, shared_data_t)
├── Time utilities (timespec_add_ns, timespec_diff_ns)
├── Computation simulation (do_computation)
├── Thread functions (sensor_thread, controller_thread, logger_thread)
├── Initialization (init_thread_configs)
├── Display functions (display_config, display_results)
└── Main (argument parsing, thread creation, result analysis)
```

## License

This code is part of the RT Systems Lab course materials (SSATR/IAISC/ICAF 2025).
Educational use permitted. Not for production RT systems without proper validation.

---

**Questions or Issues?**
Consult the main lab guides (`rt_week1_guide.md`) or course instructor.
