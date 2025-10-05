# Real-Time Systems Lab - Week 1

## Quick Start

```bash
# 1. Create project structure
mkdir -p rt-week1/{exercises,scripts,results}
cd rt-week1

# 2. Build Docker environment
docker-compose build

# 3. Run container
docker-compose run --rm rt-lab

# 4. Inside container - compile and run
make all
./exercise1
./exercise2  
./exercise3 --rt
```

## Project Structure

```
rt-week1/
├── Dockerfile                 # Container definition
├── docker-compose.yml         # Container orchestration
├── Makefile                   # Build automation
├── exercises/
│   ├── exercise1.c           # Latency measurement
│   ├── exercise2.c           # Scheduling policies
│   └── exercise3.c           # Periodic tasks
├── scripts/
│   └── plot_latency.py       # Visualization
└── results/                   # Output data and graphs
    ├── latency_raw.txt
    └── latency_raw.png
```

## File Checklist

Before building, ensure you have created these files:

- [ ] `Dockerfile` - From artifact "RT Systems Week 1 - Dockerfile"
- [ ] `docker-compose.yml` - From artifact "RT Systems Week 1 - Docker Compose"  
- [ ] `Makefile` - From artifact "Makefile for Week 1 Exercises"
- [ ] `exercises/exercise1.c` - From artifact "Exercise 1: Basic Latency Measurement"
- [ ] `exercises/exercise2.c` - From artifact "Exercise 2: Scheduling Policy Comparison"
- [ ] `exercises/exercise3.c` - From artifact "Exercise 3: Periodic Task Simulation"
- [ ] `scripts/plot_latency.py` - From artifact "Visualization Script - Plot Latency"

## What Each Exercise Teaches

### Exercise 1: Latency Measurement
**Concepts**: Latency, jitter, timing accuracy
- Measures sleep() precision
- Calculates statistical timing metrics
- Visualizes timing distribution
- Shows impact of system load

**Key Takeaway**: Understanding the difference between average latency and jitter

### Exercise 2: Scheduling Policies  
**Concepts**: SCHED_OTHER vs SCHED_FIFO vs SCHED_RR
- Compares different Linux schedulers
- Demonstrates priority-based scheduling
- Shows RT advantage under load
- Illustrates fairness vs determinism trade-off

**Key Takeaway**: RT scheduling prioritizes important tasks over fair sharing

### Exercise 3: Periodic Tasks
**Concepts**: Deadlines, WCET, utilization
- Simulates real-time control loop
- Detects deadline misses
- Measures task latency
- Compares normal vs RT scheduling

**Key Takeaway**: Meeting deadlines requires bounded execution time and proper scheduling

## Important Notes

### Docker Limitations
⚠️ **Docker containers share the host kernel**. This means:
- True RT guarantees require RT kernel on host
- Results demonstrate concepts, not absolute RT performance  
- Focus on relative differences between configurations
- Use `--privileged` flag for RT scheduling APIs

### RT Kernel Note
For production RT systems, you need:
- PREEMPT_RT patched kernel
- Real hardware (not virtualized)
- Proper BIOS/firmware configuration
- Isolated CPU cores

This lab focuses on **learning RT concepts** using accessible tools.

## Common Commands

```bash
# Inside container

# Build all exercises
make all

# Run with visualization
make run1

# Run under stress
make stress-test

# Check scheduling policies
chrt -m

# Monitor in real-time
htop

# Clean everything
make clean
```

## Expected Results

### Exercise 1 - Typical Output
- Average latency: 20-200 µs (Docker overhead)
- Jitter: 100-500 µs (non-RT kernel)
- Under stress: 10x higher jitter

### Exercise 2 - Typical Output  
- RT threads faster than NORMAL
- FIFO ≈ RR completion times
- Priority clearly affects execution order

### Exercise 3 - Typical Output
- Few/no deadline misses with RT + low utilization
- Many misses with high utilization or normal scheduling
- Jitter: <100 µs (RT) vs >500 µs (normal)

## Experiments to Try

1. **Baseline measurements** - Clean system, no load
2. **CPU stress** - `stress-ng --cpu 4`
3. **I/O stress** - `stress-ng --io 2`  
4. **Memory pressure** - `stress-ng --vm 2 --vm-bytes 512M`
5. **Priority inversion preview** - Modify exercise2 with locks

## Learning Objectives

After completing Week 1, students should understand:

✓ **Fundamental Concepts**
- Latency vs jitter
- Hard vs soft real-time
- Determinism vs performance trade-offs

✓ **Scheduling**  
- Linux scheduling policies (OTHER, FIFO, RR)
- Priority-based scheduling
- Impact of system load

✓ **Measurement**
- Timing measurement techniques
- Statistical analysis of latency
- Deadline detection

✓ **Practical Skills**
- Using RT scheduling APIs
- Interpreting timing data
- Identifying timing problems

## Week 1 Assignment

See the complete lab guide for:
- Measurement report requirements
- Critical analysis questions  
- Code modification tasks
- Deliverables checklist

## Next Steps

**Week 2 Preview**: Priority Inversion and Synchronization
- Understanding priority inversion problem
- Priority inheritance protocol
- Using RT-safe mutexes
- Trace analysis with trace-cmd

## Troubleshooting

### Container won't start
```bash
# Check Docker is running
docker ps

# Rebuild container
docker-compose build --no-cache
```

### Cannot set RT priority
```bash
# Run with elevated privileges
docker-compose run --rm --privileged rt-lab

# Or modify docker-compose.yml to include 'privileged: true'
```

### Compilation errors
```bash
# Check file structure
ls exercises/
ls scripts/

# Verify file permissions
chmod +x scripts/*.py
```

### High latency values
This is expected in Docker without RT kernel. Focus on:
- Relative comparisons (RT vs normal)
- Impact of load on jitter
- Conceptual understanding

## Resources

- **Man pages**: `man sched_setscheduler`, `man clock_nanosleep`
- **Kernel docs**: /usr/share/doc/linux-doc/scheduler/
- **Tool docs**: `cyclictest --help`, `stress-ng --help`

## Support

For questions:
1. Check the FAQ in lab guide
2. Review man pages for API details
3. Consult instructor during office hours
4. Post on course forum with error messages

---

**Happy experimenting with real-time systems! 🚀**
