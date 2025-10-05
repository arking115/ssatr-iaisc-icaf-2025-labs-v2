# Real-Time Systems Laboratory

SSATR/IAISC/ICAF 2025 - Practical exercises for learning real-time systems concepts.

## Quick Start

```bash
# 1. Build the Docker environment
docker-compose build

# 2. Run the container
docker-compose run --rm rt-lab

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

# 3. Start interactive session with full privileges (needed for RT scheduling):
  docker-compose run --rm --privileged rt-lab
```

## Project Structure

```
.
├── Dockerfile              # RT-enabled development environment
├── docker-compose.yml      # Container configuration with RT privileges
├── Makefile               # Build automation
├── exercises/             # C source files for each exercise
├── scripts/               # Python visualization scripts
└── results/               # Output data and graphs
```

## Available Commands

### Docker Commands
```bash
# Build container
docker-compose build

# Run interactive shell
docker-compose run --rm rt-lab

# Rebuild without cache
docker-compose build --no-cache
```

### Build Commands (inside container)
```bash
make all          # Build all exercises
make exercise1    # Build Exercise 1
make clean        # Remove binaries
make help         # Show all commands
```

### Run Commands (inside container)
```bash
make run1         # Run Exercise 1 with visualization
make run2         # Run Exercise 2
make run3         # Run Exercise 3 (both modes)
make run-all      # Run all exercises
make stress-test  # Test under system load
```

## Weeks

### Week 1: RT Fundamentals
- Exercise 1: Latency and jitter measurement
- Exercise 2: Scheduling policy comparison (SCHED_OTHER, FIFO, RR)
- Exercise 3: Periodic task simulation with deadline detection

See `rt_week1_guide.md` for detailed instructions.

## Requirements

- Docker and Docker Compose
- Host system with sufficient privileges for RT scheduling (container runs with `--privileged`)

## Notes

- The container requires `--privileged` mode to use RT scheduling APIs
- Results demonstrate RT concepts; true RT guarantees require PREEMPT_RT kernel on host
- Focus on relative differences between configurations

## Resources

- Lab guides: `rt_week*_guide.md`
- Quick reference: `rt_week*_readme.md`
- Claude AI guidance: `CLAUDE.md`
