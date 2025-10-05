# Real-Time Systems - Week 1 Lab Guide

## Overview
This week focuses on fundamental RT concepts: **latency, jitter, determinism, and scheduling policies**. You'll measure these properties and understand the difference between real-time and general-purpose scheduling.

## Learning Objectives
- Understand latency vs jitter
- Differentiate hard vs soft real-time systems  
- Compare Linux scheduling policies (SCHED_OTHER, SCHED_FIFO, SCHED_RR)
- Measure and analyze timing characteristics
- Recognize the impact of system load on determinism

## Setup Instructions

### 1. Build the Docker Environment
```bash
# Clone or create the lab directory structure
mkdir -p rt-labs/week1/{exercises,scripts,results}
cd rt-labs/week1

# Copy the provided files:
# - Dockerfile
# - docker-compose.yml
# - exercises/*.c
# - scripts/plot_latency.py
# - Makefile

# Build and run
docker-compose build
docker-compose run --rm rt-lab
```

### 2. Inside the Container
```bash
# You should now be in /workspace
# Compile all exercises
make all

# Or compile individually
make exercise1
make exercise2
make exercise3
```

## Exercise 1: Basic Latency Measurement

### Concept
This exercise measures how accurately the system can sleep for a specified interval. The **deviation** from the target is the latency, and the **variation** in latency is jitter.

### Run the Exercise
```bash
./exercise1
```

### Expected Output
```
=== Exercise 1: Basic Latency Measurement ===
Target sleep interval: 1000000 ns (1.000 ms)
Running 1000 iterations...

=== Results ===
Average latency: 45230 ns (45.230 µs)
Min deviation:   12450 ns (12.450 µs)
Max deviation:   234120 ns (234.120 µs)
Jitter:          221670 ns (221.670 µs)
```

### Visualization
```bash
python3 scripts/plot_latency.py results/latency_raw.txt
```

### Experiments to Try
1. **Baseline measurement**: Run without any load
2. **Under CPU stress**: 
   ```bash
   # Terminal 1
   stress-ng --cpu 4
   
   # Terminal 2
   ./exercise1
   ```
3. **Under I/O stress**:
   ```bash
   stress-ng --io 2 &
   ./exercise1
   ```

### Questions to Answer
1. What causes latency deviation from the target sleep time?
2. How does jitter change under stress?
3. Why is low jitter more important than low latency for RT systems?
4. What jitter level would be acceptable for a 10ms control loop?

---

## Exercise 2: Scheduling Policy Comparison

### Concept
Linux supports multiple scheduling policies:
- **SCHED_OTHER**: Default time-sharing (CFS - Completely Fair Scheduler)
- **SCHED_FIFO**: Real-time FIFO (runs until yields/blocks)
- **SCHED_RR**: Real-time Round-Robin (time-sliced RT)

### Run the Exercise
```bash
./exercise2
```

### Expected Behavior
- RT threads (FIFO/RR) should complete faster than NORMAL thread
- FIFO and RR should have similar completion times (no preemption in this test)
- Results show priority impact on execution

### Experiments to Try
1. **Compare with system load**:
   ```bash
   stress-ng --cpu 4 &
   ./exercise2
   ```
2. **Modify priorities** in the code and rebuild
3. **Run with nice value**:
   ```bash
   nice -n -20 ./exercise2
   ```

### Questions to Answer
1. Why do SCHED_FIFO and SCHED_RR complete at similar times?
2. When would you choose RR over FIFO?
3. What happens if two FIFO tasks at same priority compete?
4. How does SCHED_OTHER handle competing tasks differently?

---

## Exercise 3: Periodic Task Simulation

### Concept
Many RT systems have **periodic tasks** that must execute at fixed intervals (e.g., control loops, sensor sampling). This exercise simulates a 10ms periodic task and detects deadline misses.

### Run the Exercise

**Normal scheduling**:
```bash
./exercise3
```

**Real-time scheduling**:
```bash
./exercise3 --rt
```

### Expected Output
```
=== Exercise 3: Periodic Task Simulation ===
Running with SCHED_FIFO priority 80

Task Configuration:
  Period: 10 ms
  Computation time: 2000 µs
  Duration: 5 seconds

Cycle | Latency (µs) | Status
------|--------------|-------
    0 |         12.3 | OK
  100 |         15.7 | OK
  200 |         14.2 | OK
  ...

=== Results ===
Total cycles:      500
Missed deadlines:  0 (0.00%)

Latency Statistics:
  Min:     8 µs
  Max:     45 µs
  Average: 18 µs
  Jitter:  37 µs
```

### Experiments to Try

1. **Compare RT vs Normal scheduling**:
   ```bash
   # Normal
   ./exercise3 > normal.log
   
   # Real-time
   ./exercise3 --rt > rt.log
   
   # Compare jitter
   grep "Jitter:" normal.log rt.log
   ```

2. **Increase computation time**:
   - Edit `exercise3.c` and change `COMPUTATION_USEC` to `8000`
   - Recompile: `make exercise3`
   - Run: `./exercise3 --rt`
   - Observe deadline misses!

3. **System under load**:
   ```bash
   stress-ng --cpu 4 --io 2 &
   ./exercise3 --rt
   ```

4. **Find the breaking point**:
   - Gradually increase `COMPUTATION_USEC` until deadlines are missed
   - This demonstrates WCET (Worst Case Execution Time) analysis

### Questions to Answer
1. How does RT scheduling affect jitter compared to normal?
2. At what computation time do deadlines start being missed?
3. What is the utilization factor? (computation_time / period)
4. Why can't we use 100% CPU utilization in RT systems?

---

## Using cyclictest (Advanced)

cyclictest is the industry-standard latency measurement tool for Linux RT systems.

### Basic Usage
```bash
# Simple test
cyclictest -p 80 -t1 -n -i 1000 -l 10000

# Explanation:
# -p 80      : priority 80
# -t1        : 1 thread
# -n         : use clock_nanosleep
# -i 1000    : interval 1000µs (1ms)
# -l 10000   : 10000 loops
```

### With System Load
```bash
# Terminal 1: Start load
stress-ng --cpu 4 --io 2 --vm 1

# Terminal 2: Measure latency
cyclictest -p 80 -t1 -n -i 1000 -l 10000 -q
```

### Histogram Output
```bash
cyclictest -p 80 -t1 -i 1000 -l 10000 -h 100 -q > histogram.txt
```

### Compare with Your Exercise 1
```bash
# Run both and compare jitter
./exercise1
cyclictest -p 80 -t1 -n -i 1000 -l 1000 -q
```

---

## Analysis and Discussion

### Key Metrics Explained

**Latency**: Time between when a task should execute and when it actually starts
- Low latency = responsive system
- Measured in microseconds (µs) or nanoseconds (ns)

**Jitter**: Variation in latency over time
- Low jitter = predictable system  
- Critical for RT: prefer consistent 10ms over variable 1-100ms
- Formula: `jitter = max_latency - min_latency`

**Determinism**: Ability to predict system behavior
- RT systems prioritize determinism over performance
- Bounded worst-case behavior is essential

### Hard vs Soft Real-Time

**Hard Real-Time** (Exercise 3 with strict deadlines):
- Missing deadline = system failure
- Examples: airbag deployment, pacemaker
- Requires WCET analysis and schedulability proof
- Zero tolerance for deadline misses

**Soft Real-Time** (Exercise 1 measurements):
- Missing deadline = degraded performance
- Examples: video streaming, audio playback
- Best effort with quality degradation
- Occasional misses acceptable

### Scheduling Policy Selection Guide

| Policy | Use Case | Priority | Preemption |
|--------|----------|----------|------------|
| SCHED_OTHER | General purpose | Nice value | Yes (time-sliced) |
| SCHED_FIFO | Hard RT, highest priority | 1-99 | Only by higher priority |
| SCHED_RR | Hard RT, shared priority | 1-99 | Time-sliced at same priority |
| SCHED_DEADLINE | EDF scheduling | Automatic | Deadline-based |

**Decision Tree**:
- Need RT guarantees? → SCHED_FIFO or SCHED_RR
- Multiple tasks same priority? → SCHED_RR
- Single high-priority task? → SCHED_FIFO
- Normal application? → SCHED_OTHER

---

## Common Issues and Solutions

### Issue 1: "Operation not permitted" when setting RT priority
**Cause**: Container needs elevated privileges

**Solution**:
```bash
# Run container with capabilities
docker run --cap-add=SYS_NICE ...

# Or with full privileges
docker run --privileged ...
```

### Issue 2: High jitter even with RT scheduling
**Cause**: Host kernel is not RT-patched

**Solution**:
- Results show relative differences, not absolute RT performance
- For true RT, host needs PREEMPT_RT kernel
- Current setup demonstrates concepts, not guarantees

### Issue 3: Cannot plot graphs
**Cause**: Missing matplotlib or display issues

**Solution**:
```bash
# Install if missing
pip3 install matplotlib

# Save instead of display
# Modify plot script to use plt.savefig() only
```

---

## Week 1 Assignment

### Part 1: Measurement Report
Run all three exercises and create a report with:
1. Latency/jitter comparison (normal vs RT vs stressed)
2. Scheduling policy performance data
3. Periodic task deadline analysis
4. Graphs showing timing distributions

### Part 2: Critical Analysis
Answer these questions:
1. Why does Docker without RT kernel still show scheduling differences?
2. Calculate the maximum safe utilization for Exercise 3
3. Design a simple control system with 3 periodic tasks:
   - Sensor read: 5ms period, 0.5ms execution
   - Controller: 10ms period, 2ms execution  
   - Actuator: 20ms period, 1ms execution
   
   Are these tasks schedulable? Prove using Rate Monotonic Analysis.

### Part 3: Experimentation
Modify Exercise 3 to:
1. Add a second periodic task with different period
2. Implement CPU affinity (pin to core 0)
3. Add deadline miss logging to a file
4. Create a gnuplot visualization of deadline misses over time

### Deliverables
- Code modifications
- Measurement data (CSV format)
- Analysis report (PDF)
- Graphs and visualizations

---

## Additional Resources

### Documentation
- `man 2 sched_setscheduler` - Scheduling policy details
- `man 2 clock_nanosleep` - High-precision sleep
- `man 1 chrt` - Command-line priority control

### Further Reading
- "The Linux Scheduling Policy" - kernel.org documentation
- "A Survey of Real-Time Scheduling" - Liu & Layland paper
- cyclictest source code - understanding latency measurement

### Tools for Next Week
- `perf` - performance profiling
- `ftrace` - kernel function tracing  
- `trace-cmd` - trace analysis
- Priority inheritance mutexes

---

## Quick Reference Commands

```bash
# Build everything
make all

# Run all exercises
make run-all

# Individual exercises
make run1  # Latency measurement + plot
make run2  # Scheduling comparison
make run3  # Periodic tasks (both modes)

# With stress test
make stress-test

# Check scheduling info
chrt -m                    # Show policy limits
ps -eo pid,class,rtprio,cmd | grep exercise  # Check running tasks

# Monitor system
htop                       # See task priorities
watch -n 1 'cat /proc/loadavg'  # System load

# Clean up
make clean
```

---

## Success Criteria

By the end of Week 1, you should be able to:
- ✓ Explain latency vs jitter with examples
- ✓ Differentiate hard and soft real-time requirements
- ✓ Use Linux RT scheduling APIs (sched_setscheduler)
- ✓ Measure and analyze timing characteristics
- ✓ Understand the impact of system load on determinism
- ✓ Calculate basic schedulability for periodic tasks
- ✓ Identify sources of non-determinism in systems

**Next Week Preview**: Priority Inversion, Priority Inheritance, and Synchronization in RT Systems

---

## Troubleshooting FAQ

**Q: Results don't show RT advantage**  
A: Ensure container runs with `--privileged` or `--cap-add=SYS_NICE`

**Q: Very high latency values (>1ms)**  
A: Normal in non-RT Docker. Focus on relative comparisons.

**Q: Compilation errors**  
A: Check that all `.c` files are in `exercises/` directory

**Q: Python plotting fails**  
A: Install matplotlib: `pip3 install matplotlib`

**Q: Deadline misses even with low utilization**  
A: Expected in Docker - host kernel interference causes this

For additional help, consult the instructor or course forum.