# Real-Time Systems Laboratory

## Quick Start

```bash
# 1. Build the Docker environment
docker-compose build

# 2. Run the container with RT capabilities
docker-compose run --rm --privileged rt-lab

# 3. Inside the container, build exercises
make all

# 4. Run an exercise
./exercise1
```

If container is already running, attach with exec:
```bash
# 1. Find container name/ID
docker ps

# 2. Connect to it
docker exec -it <container_name_or_id> /bin/bash
```

## Project Structure

```
.
├── Dockerfile              # RT-enabled development environment
├── docker-compose.yml      # Container configuration with RT privileges
├── Makefile               # Build automation for all exercises
├── exercises/             # C source files for each exercise
│   ├── exercise0a-0f.c   # Week 0: C programming foundations
│   ├── exercise1-3.c     # Week 1: RT fundamentals
│   ├── exercise4-5.c     # Week 2: Priority inversion
│   ├── exercise6-7.c     # Week 3: Deadlock
│   └── exercise8-9.c     # Week 4: Lock-free programming
├── scripts/               # Python visualization scripts
├── results/               # Output data and graphs
├── docs/                  # Additional documentation 
├── rt_week0_guide.md      # Week 0: C foundations
├── rt_week1_guide.md      # Week 1: RT basics
├── rt_week2_guide.md      # Week 2: Synchronization
├── rt_week3_guide.md      # Week 3: Deadlock
└── rt_week4_guide.md      # Week 4: Lock-free
```

## Available Commands

### Docker Commands
```bash
# Build container
docker-compose build

# Run interactive shell with RT capabilities
docker-compose run --rm --privileged rt-lab

# Rebuild without cache
docker-compose build --no-cache
```

### Build Commands (inside container)
```bash
make all          # Build all exercises
make week0        # Build all Week 0 exercises
make week1        # Build all Week 1 exercises
make exercise1    # Build individual exercise
make clean        # Remove binaries
make help         # Show all commands
```

### Run Commands (inside container)
```bash
make run-week0    # Run all Week 0 exercises
make run-week1    # Run all Week 1 exercises
make run1         # Run Exercise 1 with visualization
make run2         # Run Exercise 2
make run-all      # Run all exercises
make stress-test  # Test under system load
```

## Weekly Lab Guides

### Week 0: C Programming Foundations
**Prerequisite exercises to prepare for RT programming**
- Exercise 0A: Time basics (`clock_gettime`, `struct timespec`)
- Exercise 0B: Simple threading (`pthread_create`, `pthread_join`)
- Exercise 0C: Structs and file I/O
- Exercise 0D: Command-line arguments and busy-wait loops
- Exercise 0E: Scheduling introduction (`sched_setscheduler`)
- Exercise 0F: Statistics and array manipulation

**Guide:** `rt_week0_guide.md`

---

### Week 1: RT Fundamentals
**Core real-time concepts and measurements**
- Exercise 1: Latency and jitter measurement
- Exercise 2: Scheduling policy comparison (SCHED_OTHER, SCHED_FIFO, SCHED_RR)
- Exercise 3: Periodic task simulation with deadline detection

**Key Concepts:** Latency vs jitter, hard vs soft real-time, determinism, scheduling policies
**Guide:** `rt_week1_guide.md`

---

### Week 2: Priority Inversion and Synchronization
**Understanding and solving priority inversion**
- Exercise 4: Unbounded priority inversion demonstration
- Exercise 5: Priority Inheritance Protocol (PIP)

**Key Concepts:** Priority inversion, bounded vs unbounded blocking, PI mutexes, real-time synchronization
**Guide:** `rt_week2_guide.md`

---

### Week 3: Deadlock
**Circular wait conditions and prevention strategies**
- Exercise 6: Deadlock demonstration and detection
- Exercise 7: Deadlock prevention through lock ordering

**Key Concepts:** Coffman conditions, circular wait, timeout-based detection, resource ordering, deadlock prevention
**Guide:** `rt_week3_guide.md`

---

### Week 4: Lock-Free Programming
**Alternative synchronization without locks**
- Exercise 8: Lock-free stack implementation
- Exercise 9: Comprehensive performance analysis (locks vs lock-free)

**Key Concepts:** CAS operations, ABA problem, atomic operations, memory ordering, wait-free vs lock-free, progress guarantees
**Guide:** `rt_week4_guide.md`

---

## Requirements

- Docker and Docker Compose
- Host system with sufficient privileges for RT scheduling (container runs with `--privileged`). While this will be possible difficult to achieve the focus should be in understand programming concepts related to RT programming, even if during tests and evaluation effective RT behavior will not be observed.
- For visualization: Python 3 with matplotlib (included in Docker image)

## Notes

- The container requires `--privileged` mode to use RT scheduling APIs (`sched_setscheduler`)
- Results demonstrate RT concepts; true RT guarantees require PREEMPT_RT kernel on host
- Focus on relative differences between configurations (normal vs RT scheduling, stressed vs idle)
- Docker adds overhead; measurements show conceptual behavior, not production RT performance

## Common Commands Reference

```bash
# Build and run all Week 0 exercises
make week0 && make run-week0

# Build and run a specific week
make week1 && make run-week1

# Run under stress to observe jitter
stress-ng --cpu 4 &
./exercise1

# Check scheduling policy limits
chrt -m

# View running task priorities
ps -eo pid,class,rtprio,cmd | grep exercise

# Monitor system performance
htop
```

