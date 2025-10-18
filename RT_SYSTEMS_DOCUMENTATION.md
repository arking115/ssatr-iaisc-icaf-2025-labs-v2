# Real-Time Systems Laboratory Documentation

**SSATR/IAISC/ICAF 2025**
Practical Exercises for Learning Real-Time Systems Concepts

---

## About This Documentation

This comprehensive guide accompanies the Real-Time Systems laboratory course. It provides step-by-step instructions, theoretical foundations, practical exercises, and reference materials for mastering real-time programming concepts using Linux and POSIX APIs.

**Target Audience:** Students with basic C programming knowledge who want to learn real-time systems concepts through hands-on exercises.

**Course Structure:** 5 weeks of progressive learning, from C foundations through advanced lock-free programming.

---

## Table of Contents

### **Part I: Getting Started**

#### **[1. Introduction](#1-introduction)**
   - [1.1 Course Overview](#11-course-overview)
   - [1.2 Learning Objectives](#12-learning-objectives)
   - [1.3 Prerequisites](#13-prerequisites)
   - [1.4 Repository Structure](#14-repository-structure)

#### **[2. Environment Setup](#2-environment-setup)**
   - [2.1 Docker Installation and Configuration](#21-docker-installation-and-configuration)
   - [2.2 Building the RT Development Environment](#22-building-the-rt-development-environment)
   - [2.3 Running Containers with RT Capabilities](#23-running-containers-with-rt-capabilities)
   - [2.4 Development Workflow](#24-development-workflow)
   - [2.5 Troubleshooting Common Setup Issues](#25-troubleshooting-common-setup-issues)

#### **[3. Quick Start Guide](#3-quick-start-guide)**
   - [3.1 First Steps](#31-first-steps)
   - [3.2 Building Exercises](#32-building-exercises)
   - [3.3 Running Your First Exercise](#33-running-your-first-exercise)
   - [3.4 Understanding the Output](#34-understanding-the-output)
   - [3.5 Available Makefile Commands](#35-available-makefile-commands)

---

### **Part II: Foundations (Week 0)**

#### **[4. Week 0: C Programming Foundations for Real-Time Systems](#4-week-0-c-programming-foundations-for-real-time-systems)**
   - [4.1 Overview and Learning Path](#41-overview-and-learning-path)
   - [4.2 Exercise 0A: Time Basics](#42-exercise-0a-time-basics)
   - [4.3 Exercise 0B: Simple Threading](#43-exercise-0b-simple-threading)
   - [4.4 Exercise 0C: Structs and File I/O](#44-exercise-0c-structs-and-file-io)
   - [4.5 Exercise 0D: Command-Line Arguments and Busy-Wait](#45-exercise-0d-command-line-arguments-and-busy-wait)
   - [4.6 Exercise 0E: Scheduling Introduction](#46-exercise-0e-scheduling-introduction)
   - [4.7 Exercise 0F: Statistics and Array Manipulation](#47-exercise-0f-statistics-and-array-manipulation)
   - [4.8 Concept-to-Exercise Mapping](#48-concept-to-exercise-mapping)
   - [4.9 Key C Idioms for RT Programming](#49-key-c-idioms-for-rt-programming)

---

### **Part III: Real-Time Fundamentals (Week 1)**

#### **[5. Week 1: Core Real-Time Concepts](#5-week-1-core-real-time-concepts)**
   - [5.1 Overview: Latency, Jitter, and Determinism](#51-overview-latency-jitter-and-determinism)
   - [5.2 Exercise 1: Basic Latency Measurement](#52-exercise-1-basic-latency-measurement)
   - [5.3 Exercise 2: Scheduling Policy Comparison](#53-exercise-2-scheduling-policy-comparison)
   - [5.4 Exercise 3: Periodic Task Simulation](#54-exercise-3-periodic-task-simulation)
   - [5.5 Hard vs Soft Real-Time Systems](#55-hard-vs-soft-real-time-systems)
   - [5.6 Key Metrics and Analysis Methods](#56-key-metrics-and-analysis-methods)
   - [5.7 Using cyclictest (Advanced)](#57-using-cyclictest-advanced)
   - [5.8 Week 1 Assignment and Deliverables](#58-week-1-assignment-and-deliverables)

---

### **Part IV: Synchronization Problems (Week 2)**

#### **[6. Week 2: Priority Inversion and Synchronization](#6-week-2-priority-inversion-and-synchronization)**
   - [6.1 Understanding Priority Inversion](#61-understanding-priority-inversion)
   - [6.2 Unbounded vs Bounded Blocking](#62-unbounded-vs-bounded-blocking)
   - [6.3 Exercise 4: Priority Inversion Demonstration](#63-exercise-4-priority-inversion-demonstration)
   - [6.4 Exercise 5: Priority Inheritance Protocol (PIP)](#64-exercise-5-priority-inheritance-protocol-pip)
   - [6.5 Advanced Topics](#65-advanced-topics)
   - [6.6 Real-World Case Study: Mars Pathfinder](#66-real-world-case-study-mars-pathfinder)
   - [6.7 Week 2 Assignment and Analysis](#67-week-2-assignment-and-analysis)

---

### **Part V: Deadlock (Week 3)**

#### **[7. Week 3: Deadlock Detection and Prevention](#7-week-3-deadlock-detection-and-prevention)**
   - [7.1 What is Deadlock?](#71-what-is-deadlock)
   - [7.2 The Four Coffman Conditions](#72-the-four-coffman-conditions)
   - [7.3 Exercise 6: Deadlock Demonstration and Detection](#73-exercise-6-deadlock-demonstration-and-detection)
   - [7.4 Exercise 7: Deadlock Prevention with Lock Ordering](#74-exercise-7-deadlock-prevention-with-lock-ordering)
   - [7.5 Advanced Topics](#75-advanced-topics)
   - [7.6 Best Practices and Checklist](#76-best-practices-and-checklist)
   - [7.7 Week 3 Assignment Ideas](#77-week-3-assignment-ideas)

---

### **Part VI: Lock-Free Programming (Week 4)**

#### **[8. Week 4: Lock-Free Synchronization](#8-week-4-lock-free-synchronization)**
   - [8.1 Introduction to Lock-Free Programming](#81-introduction-to-lock-free-programming)
   - [8.2 Progress Guarantees (Wait-Free, Lock-Free, Blocking)](#82-progress-guarantees-wait-free-lock-free-blocking)
   - [8.3 Atomic Operations](#83-atomic-operations)
   - [8.4 Exercise 8: Lock-Free Stack Implementation](#84-exercise-8-lock-free-stack-implementation)
   - [8.5 The ABA Problem](#85-the-aba-problem)
   - [8.6 Exercise 9: Comprehensive Performance Analysis](#86-exercise-9-comprehensive-performance-analysis)
   - [8.7 Advanced Topics](#87-advanced-topics)
   - [8.8 Common Pitfalls and Solutions](#88-common-pitfalls-and-solutions)
   - [8.9 Week 4 Assignment Ideas](#89-week-4-assignment-ideas)

---

### **Part VII: Tools and Commands**

#### **[14. Command Reference](#14-command-reference)**
   - [14.1 Docker Commands](#141-docker-commands)
   - [14.2 Build Commands (Makefile targets)](#142-build-commands-makefile-targets)
   - [14.3 Execution Commands](#143-execution-commands)
   - [14.4 Analysis and Debugging Tools](#144-analysis-and-debugging-tools)
   - [14.5 System Monitoring](#145-system-monitoring)

---

### **Part VIII: Appendices**

#### **[Appendix A: Docker Environment Details](#appendix-a-docker-environment-details)**
   - [A.1 Dockerfile Explained](#a1-dockerfile-explained)
   - [A.2 Container Capabilities](#a2-container-capabilities)
   - [A.3 Limitations in Containerized RT](#a3-limitations-in-containerized-rt)

#### **[Appendix E: POSIX API Reference](#appendix-e-posix-api-reference)**
   - [E.1 Time Functions](#e1-time-functions)
   - [E.2 Thread Functions](#e2-thread-functions)
   - [E.3 Scheduling Functions](#e3-scheduling-functions)
   - [E.4 Synchronization Primitives](#e4-synchronization-primitives)
   - [E.5 Atomic Operations](#e5-atomic-operations)

---

## How to Use This Documentation

### For Students

**First Time:**
1. Start with [Part I: Getting Started](#part-i-getting-started) to set up your environment
2. Complete [Week 0 exercises](#4-week-0-c-programming-foundations-for-real-time-systems) to build foundations
3. Progress through Weeks 1-4 in order
4. Use reference materials as needed

**During Labs:**
- Each week builds on previous concepts
- Complete exercises in order within each week
- Try the experiments suggested in each section
- Answer the questions to test your understanding

### Progressive Learning Path

```
Week 0: C Foundations
   ↓
Week 1: RT Fundamentals (Latency, Jitter, Scheduling)
   ↓
Week 2: Priority Inversion & Synchronization
   ↓
Week 3: Deadlock Detection & Prevention
   ↓
Week 4: Lock-Free Programming
```

### Document Conventions

- **Code blocks** show example code or commands to run
- **Expected Output** sections show what you should see
- **Experiments to Try** suggest variations to explore
- **Questions to Answer** test your understanding

---

# Part I: Getting Started

## 1. Introduction

### 1.1 Course Overview

This laboratory course (SSATR/IAISC/ICAF 2025) provides hands-on experience with real-time systems programming. Through practical exercises, you'll learn fundamental RT concepts, synchronization problems, and advanced programming techniques essential for developing time-critical software systems.

**What You'll Learn:**
- Measuring and analyzing timing behavior (latency, jitter)
- Using Linux real-time scheduling policies
- Solving synchronization problems (priority inversion, deadlock)
- Implementing lock-free data structures
- Analyzing schedulability and meeting deadlines

### 1.2 Learning Objectives

By the end of this course, you will be able to:

**Technical Skills:**
- ✓ Measure timing characteristics with nanosecond precision
- ✓ Configure and use Linux RT scheduling policies
- ✓ Implement periodic real-time tasks
- ✓ Detect and prevent priority inversion
- ✓ Avoid and resolve deadlock situations
- ✓ Design and implement lock-free data structures
- ✓ Analyze system schedulability
- ✓ Profile and optimize RT performance

**Conceptual Understanding:**
- ✓ Differentiate hard vs soft real-time requirements
- ✓ Understand determinism and predictability
- ✓ Recognize sources of non-determinism
- ✓ Compare synchronization approaches
- ✓ Make informed design decisions for RT systems

**Practical Experience:**
- ✓ Work with POSIX real-time APIs
- ✓ Use Docker for RT development
- ✓ Apply scheduling analysis methods
- ✓ Debug timing-related issues
- ✓ Document and present performance measurements

### 1.3 Prerequisites

**Required Knowledge:**
- C programming (functions, pointers, structs, arrays)
- Basic Linux command line usage
- Understanding of compilation (gcc, Makefile basics)
- Familiarity with basic algorithms and data structures

**Helpful But Not Required:**
- Operating systems concepts (processes, threads, scheduling)
- Experience with multi-threaded programming
- Knowledge of computer architecture (CPU, memory, cache)

**Software Requirements:**
- Computer with Docker and Docker Compose installed
- 4GB+ RAM recommended
- Linux, Windows (WSL2), or macOS

**Note:** Week 0 exercises review essential C programming concepts needed for RT work. Start there if you need a refresher.

### 1.4 Repository Structure

The course repository is organized as follows:

```
.
├── RT_SYSTEMS_DOCUMENTATION.md    # This comprehensive guide
├── README.md                      # Quick start and overview
├── CLAUDE.md                      # AI assistant guidance
│
├── rt_week0_guide.md             # Week 0: C foundations
├── rt_week1_guide.md             # Week 1: RT fundamentals
├── rt_week2_guide.md             # Week 2: Synchronization
├── rt_week3_guide.md             # Week 3: Deadlock
├── rt_week4_guide.md             # Week 4: Lock-free
│
├── Dockerfile                     # RT-enabled development environment
├── docker-compose.yml            # Container orchestration
├── Makefile                      # Build automation
│
├── exercises/                    # All exercise source code
│   ├── exercise0a_time_basics.c
│   ├── exercise0b_threading.c
│   ├── exercise0c_file_io.c
│   ├── exercise0d_busywait.c
│   ├── exercise0e_scheduling.c
│   ├── exercise0f_statistics.c
│   ├── exercise1.c               # Latency measurement
│   ├── exercise2.c               # Scheduling policies
│   ├── exercise3.c               # Periodic tasks
│   ├── exercise4.c               # Priority inversion
│   ├── exercise5.c               # Priority inheritance
│   ├── exercise6.c               # Deadlock detection
│   ├── exercise7.c               # Deadlock prevention
│   ├── exercise8.c               # Lock-free stack
│   ├── exercise8_aba.c          # ABA problem
│   └── exercise9.c               # Performance analysis
│
├── scripts/                      # Visualization and analysis
│   └── plot_latency.py          # Latency graph generation
│
├── results/                      # Output data and graphs
│
├── docs/                         # Additional reference materials
│   ├── rt_concepts_reference.md
│   ├── scheduling_policies_guide.md
│   ├── test_setup.md
│   ├── exercise8_diagrams.md
│   ├── exercise8_aba_diagrams.md
│   └── exercise8_design_rationale.md
│
└── examples/                     # Demo applications
    ├── control-app/             # Control system example
    └── sched-fifo/              # RT scheduling demo
```

**Key Files:**
- **This file** (`RT_SYSTEMS_DOCUMENTATION.md`): Complete course documentation
- **README.md**: Quick start guide and project overview
- **rt_week*_guide.md**: Original per-week lab guides (still useful as focused references)
- **exercises/*.c**: Source code for all exercises
- **Makefile**: Build targets for all exercises
- **docs/**: Supplementary technical references

**Workflow:**
1. Read relevant section in this documentation
2. Navigate to exercises/ directory
3. Build and run exercises using Makefile
4. Analyze results in results/ directory
5. Consult docs/ for deep dives into specific topics

---

## 2. Environment Setup

### 2.1 Docker Installation and Configuration

The labs use Docker to provide a consistent, RT-capable development environment across different host systems.

**Install Docker:**

**Linux:**
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install docker.io docker-compose

# Add yourself to docker group (logout/login after)
sudo usermod -aG docker $USER
```

**Windows:**
1. Install Docker Desktop: https://docs.docker.com/desktop/install/windows-install/
2. Enable WSL2 backend
3. Open WSL2 terminal for lab work

**macOS:**
1. Install Docker Desktop: https://docs.docker.com/desktop/install/mac-install/
2. Launch Docker Desktop application

**Verify Installation:**
```bash
docker --version
docker-compose --version
```

Expected output:
```
Docker version 20.10.x or higher
docker-compose version 1.29.x or higher
```

### 2.2 Building the RT Development Environment

**Clone/Download Repository:**
```bash
# If using git
git clone <repository-url>
cd <repository-name>

# Or download and extract ZIP
unzip rt-systems-labs.zip
cd rt-systems-labs
```

**Build Docker Image:**
```bash
# Build the RT-enabled container image
docker-compose build

# This will:
# - Pull base Ubuntu image
# - Install development tools (gcc, make, gdb)
# - Install Python and matplotlib for visualization
# - Configure RT capabilities
# - Set up working directory
```

Build time: 2-5 minutes depending on network speed.

**Verify Build:**
```bash
docker images | grep rt-lab
```

You should see an image named something like `rt-lab` or with your project name.

### 2.3 Running Containers with RT Capabilities

**Start Interactive Container:**
```bash
# Run with RT privileges (required for scheduling APIs)
docker-compose run --rm --privileged rt-lab

# You should now be at a bash prompt inside the container:
# root@<container-id>:/workspace#
```

**Flags Explained:**
- `--rm`: Remove container when you exit (clean up automatically)
- `--privileged`: Grant RT scheduling capabilities (CAP_SYS_NICE)
- `rt-lab`: Service name from docker-compose.yml

**Alternative Without Compose:**
```bash
docker run --rm -it --privileged \
  -v $(pwd):/workspace \
  -w /workspace \
  rt-lab /bin/bash
```

**Verify RT Capabilities:**
```bash
# Inside container, check scheduling policy limits
chrt -m

# Expected output showing RT priorities 1-99:
# SCHED_OTHER min/max priority    : 0/0
# SCHED_FIFO min/max priority     : 1/99
# SCHED_RR min/max priority       : 1/99
```

**Working with Container:**

Attach to running container:
```bash
# Terminal 1: Start container
docker-compose run --rm --privileged rt-lab

# Terminal 2: Find container name
docker ps

# Terminal 2: Attach to same container
docker exec -it <container-name> /bin/bash
```

Exit container:
```bash
exit  # or Ctrl+D
```

### 2.4 Development Workflow

**Recommended Workflow:**

1. **Edit Code:** Use your favorite editor on host machine
   - Files in project directory are mounted in container
   - Changes are immediately visible inside container

2. **Build in Container:**
   ```bash
   # Inside container
   make all
   ```

3. **Run in Container:**
   ```bash
   ./exercise1
   ```

4. **View Results:** On host or in container
   ```bash
   # Results are written to results/ directory
   cat results/latency_raw.txt
   python3 scripts/plot_latency.py results/latency_raw.txt
   ```

**Example Session:**
```bash
# Host: Start container
$ docker-compose run --rm --privileged rt-lab

# Container: Build exercises
root@abc123:/workspace# make all
gcc -Wall -pthread -lrt exercises/exercise1.c -o exercise1
...

# Container: Run exercise
root@abc123:/workspace# ./exercise1
=== Exercise 1: Basic Latency Measurement ===
Running 1000 iterations...
...

# Container: Exit
root@abc123:/workspace# exit

# Host: View results
$ cat results/latency_raw.txt
$ ls -lh results/
```

### 2.5 Troubleshooting Common Setup Issues

**Issue: "Permission denied" when running docker**

Solution:
```bash
# Add yourself to docker group
sudo usermod -aG docker $USER
# Logout and login, or:
newgrp docker
```

**Issue: "Operation not permitted" for RT scheduling**

Solution:
```bash
# Make sure using --privileged flag
docker-compose run --rm --privileged rt-lab

# Or add to docker-compose.yml:
privileged: true
cap_add:
  - SYS_NICE
```

**Issue: Docker build fails**

Solution:
```bash
# Clear cache and rebuild
docker-compose build --no-cache

# Check disk space
df -h
```

**Issue: "Cannot connect to Docker daemon"**

Solution:
```bash
# Start Docker service (Linux)
sudo systemctl start docker

# Or start Docker Desktop (Windows/Mac)
```

**Issue: Files not updating in container**

Solution:
```bash
# Verify volume mount in docker-compose.yml:
volumes:
  - .:/workspace

# Restart container
exit
docker-compose run --rm --privileged rt-lab
```

**Issue: High latency/jitter even with RT scheduling**

Expected behavior:
- Docker adds overhead; host kernel affects RT performance
- Focus on relative differences (RT vs normal, loaded vs idle)
- True RT requires PREEMPT_RT kernel on host
- Labs demonstrate concepts, not production RT performance

---

## 3. Quick Start Guide

### 3.1 First Steps

After completing environment setup, follow these steps to verify everything works:

**Step 1: Enter Container**
```bash
docker-compose run --rm --privileged rt-lab
```

**Step 2: Verify Tools**
```bash
# Check compiler
gcc --version

# Check make
make --version

# Check Python
python3 --version

# Check RT capabilities
chrt -m
```

**Step 3: List Available Exercises**
```bash
ls exercises/
```

You should see exercise0a through exercise9.

### 3.2 Building Exercises

**Build Everything:**
```bash
make all
```

This compiles all exercises (0a-9) into executable binaries in the current directory.

**Build by Week:**
```bash
make week0  # Exercises 0a-0f
make week1  # Exercises 1-3
make week2  # Exercises 4-5
make week3  # Exercises 6-7
make week4  # Exercises 8-9
```

**Build Individual Exercise:**
```bash
make exercise1
```

**View Available Targets:**
```bash
make help
```

**Clean Build Artifacts:**
```bash
make clean
```

### 3.3 Running Your First Exercise

**Run Exercise 0A (Time Basics):**
```bash
./exercise0a_time_basics
```

**Expected Output:**
```
=== Exercise 0A: Time Basics ===

1. Getting current time:
   Seconds: 1234567890
   Nanoseconds: 123456789

2. Time arithmetic:
   Start time: 1234567890.123456789
   End time:   1234567891.234567890
   Difference: 1.111111101 seconds
   Difference: 1111111101 nanoseconds
   Difference: 1111.111 microseconds
   Difference: 1111 milliseconds

3. Timing a sleep operation:
   Target sleep: 100 ms
   Actual sleep: 100.234 ms
   Deviation:    0.234 ms (234 µs)

✓ Exercise completed successfully
```

**What This Demonstrates:**
- `clock_gettime()` for precise timing
- `struct timespec` manipulation
- Time difference calculations
- Unit conversions
- Measuring actual vs requested sleep time

### 3.4 Understanding the Output

**Common Output Sections:**

1. **Header:** Shows exercise name and configuration
```
=== Exercise X: Description ===
Configuration details...
```

2. **Execution:** Shows what the program is doing
```
Running 1000 iterations...
Cycle | Latency (µs) | Status
------|--------------|-------
    0 |         12.3 | OK
```

3. **Results:** Summary statistics
```
=== Results ===
Average latency: 45.2 µs
Min/Max: 12.3 / 234.5 µs
Jitter: 222.2 µs
```

4. **Analysis:** Interpretation (some exercises)
```
=== Analysis ===
✓ All tests passed
✗ Deadline misses detected
```

**Units Used:**
- `ns` = nanoseconds (10⁻⁹ seconds)
- `µs` or `us` = microseconds (10⁻⁶ seconds)
- `ms` = milliseconds (10⁻³ seconds)
- `s` = seconds

**Status Indicators:**
- `✓` = Success/OK
- `✗` = Failure/Problem
- `⚠` = Warning

### 3.5 Available Makefile Commands

**Build Commands:**
```bash
make all         # Build all exercises
make week0       # Build Week 0 exercises (0a-0f)
make week1       # Build Week 1 exercises (1-3)
make week2       # Build Week 2 exercises (4-5)
make week3       # Build Week 3 exercises (6-7)
make week4       # Build Week 4 exercises (8-9)
make exercise1   # Build specific exercise
make clean       # Remove all binaries
make help        # Show available commands
```

**Run Commands:**
```bash
make run-week0   # Run all Week 0 exercises
make run-week1   # Run all Week 1 exercises
make run-week2   # Run all Week 2 exercises
make run-week3   # Run all Week 3 exercises
make run-week4   # Run all Week 4 exercises
make run-all     # Run ALL exercises sequentially

make run1        # Run Exercise 1 with visualization
make run2        # Run Exercise 2
make run3        # Run Exercise 3 (normal and RT modes)
make run4        # Run Exercise 4
make run5        # Run Exercise 5
make run6        # Run Exercise 6
make run7        # Run Exercise 7
make run8        # Run Exercise 8
make run9        # Run Exercise 9
```

**Special Commands:**
```bash
make stress-test # Run Exercise 1 under system load
```

**Common Workflow:**
```bash
# Clean, build, and run a specific week
make clean && make week1 && make run-week1
```

---

**Next:** Proceed to [Week 0: C Programming Foundations](#4-week-0-c-programming-foundations-for-real-time-systems) to begin the exercises, or jump to a specific week if you're already familiar with the prerequisites.

---

# Part II: Foundations (Week 0)

## 4. Week 0: C Programming Foundations for Real-Time Systems

### 4.1 Overview and Learning Path

Before diving into real-time system programming (Exercises 1-9), students need to be comfortable with specific C programming concepts and POSIX APIs. This Week 0 series provides **six focused introductory exercises** that build the foundational skills used throughout the RT labs.

**Target Audience:** Students familiar with C programming basics who need preparation for RT-specific APIs and patterns.

**Total Time:** ~90 minutes

**Learning Objectives:**
- Master high-precision time measurement
- Understand POSIX threading basics
- Work with structs and file I/O for data logging
- Parse command-line arguments
- Configure real-time scheduling policies
- Perform statistical analysis of timing data

**Exercise Flow:**
```
0A: Time Basics → 0B: Threading → 0C: File I/O
                    ↓
0D: Busy-Wait ← 0E: Scheduling ← 0F: Statistics
```

Each exercise is self-contained but builds skills needed for the next. Complete them in order for best results.

---

### 4.2 Exercise 0A: Time Basics

**File:** `exercises/exercise0a_time_basics.c`
**Time:** ~15 minutes

#### Core Concepts

- High-precision time measurement with `clock_gettime()`
- Understanding `struct timespec` (seconds + nanoseconds)
- Time arithmetic and difference calculation
- Unit conversions (nanoseconds ↔ microseconds ↔ milliseconds)
- `CLOCK_MONOTONIC` vs wall-clock time

#### Key Takeaway

All RT exercises use `CLOCK_MONOTONIC` for precise, non-decreasing time measurements. This clock is not affected by system time adjustments and is ideal for measuring intervals.

#### Build & Run

```bash
make exercise0a_time_basics
./exercise0a_time_basics
```

#### Expected Output

```
=== Exercise 0A: Time Basics ===

1. Getting current time:
   Seconds: 1234567890
   Nanoseconds: 123456789

2. Time arithmetic:
   Start time: 1234567890.123456789
   End time:   1234567891.234567890
   Difference: 1.111111101 seconds
   Difference: 1111111101 nanoseconds
   Difference: 1111.111 microseconds
   Difference: 1111 milliseconds

3. Timing a sleep operation:
   Target sleep: 100 ms
   Actual sleep: 100.234 ms
   Deviation:    0.234 ms (234 µs)

✓ Exercise completed successfully
```

#### Code Walkthrough

**Getting Current Time:**
```c
struct timespec ts;
clock_gettime(CLOCK_MONOTONIC, &ts);
printf("Seconds: %ld\n", ts.tv_sec);
printf("Nanoseconds: %ld\n", ts.tv_nsec);
```

**Time Difference Function:**
```c
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * 1000000000L) + diff_nsec;
}
```

**Unit Conversions:**
- 1 second = 1,000,000,000 nanoseconds (10⁹ ns)
- 1 millisecond = 1,000,000 nanoseconds (10⁶ ns)
- 1 microsecond = 1,000 nanoseconds (10³ ns)

#### Experiments to Try

1. **Measure different sleep durations:**
   ```bash
   # Modify TARGET_SLEEP_MS in the code
   # Try: 1ms, 10ms, 100ms, 1000ms
   ```

2. **Compare CLOCK_MONOTONIC vs CLOCK_REALTIME:**
   ```c
   // Change clock type and observe differences
   clock_gettime(CLOCK_REALTIME, &ts);
   ```

3. **Stress the system:**
   ```bash
   stress-ng --cpu 4 &
   ./exercise0a_time_basics
   # Observe if sleep deviations increase
   ```

#### Questions to Answer

1. Why use `CLOCK_MONOTONIC` instead of `CLOCK_REALTIME` for RT systems?
2. What causes the deviation between target and actual sleep time?
3. How does system load affect sleep precision?
4. What is the smallest time interval you can reliably measure?

---

### 4.3 Exercise 0B: Simple Threading

**File:** `exercises/exercise0b_threading.c`
**Time:** ~20 minutes

#### Core Concepts

- Creating threads with `pthread_create()`
- Waiting for completion with `pthread_join()`
- Thread function signature: `void* func(void* arg)`
- Passing data to threads via structs
- Collecting results from threads

#### Key Takeaway

RT systems run multiple concurrent tasks; pthreads provide the foundation for parallel execution and task management.

#### Build & Run

```bash
make exercise0b_threading
./exercise0b_threading
```

#### Expected Output

```
=== Exercise 0B: Simple Threading ===

Main thread: Creating 4 worker threads...

Thread 0: Started with ID 0
Thread 1: Started with ID 1
Thread 2: Started with ID 2
Thread 3: Started with ID 3

Thread 0: Working... (iteration 0)
Thread 1: Working... (iteration 0)
Thread 2: Working... (iteration 0)
Thread 3: Working... (iteration 0)
...
Thread 0: Completed, result = 50
Thread 1: Completed, result = 50
Thread 2: Completed, result = 50
Thread 3: Completed, result = 50

Main thread: All threads completed

=== Results ===
Thread 0: Result = 50, Duration = 0.105 seconds
Thread 1: Result = 50, Duration = 0.106 seconds
Thread 2: Result = 50, Duration = 0.105 seconds
Thread 3: Result = 50, Duration = 0.107 seconds

✓ Exercise completed successfully
```

#### Code Walkthrough

**Thread Data Structure:**
```c
typedef struct {
    int thread_id;        // Input: thread identifier
    int iterations;       // Input: work to do
    int result;          // Output: computed result
    double duration;     // Output: execution time
} thread_data_t;
```

**Creating Threads:**
```c
pthread_t threads[NUM_THREADS];
thread_data_t thread_data[NUM_THREADS];

for (int i = 0; i < NUM_THREADS; i++) {
    thread_data[i].thread_id = i;
    thread_data[i].iterations = 50;

    pthread_create(&threads[i], NULL, worker_thread, &thread_data[i]);
}
```

**Worker Thread Function:**
```c
void* worker_thread(void* arg) {
    thread_data_t *data = (thread_data_t *)arg;

    printf("Thread %d: Started\n", data->thread_id);

    // Do some work
    for (int i = 0; i < data->iterations; i++) {
        // Simulate computation
        usleep(1000);  // 1ms
    }

    data->result = data->iterations;
    return data;  // Return pointer to data
}
```

**Waiting for Threads:**
```c
for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
    printf("Thread %d: Result = %d\n",
           thread_data[i].thread_id,
           thread_data[i].result);
}
```

#### Experiments to Try

1. **Vary thread count:**
   ```c
   #define NUM_THREADS 8  // Try 1, 2, 4, 8, 16
   ```

2. **Add synchronization (preview of Week 2):**
   ```c
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   // Protect shared resources
   ```

3. **Set thread priorities (preview of Week 1):**
   ```c
   struct sched_param param;
   param.sched_priority = 50;
   pthread_setschedparam(thread, SCHED_FIFO, &param);
   ```

4. **Measure thread creation overhead:**
   ```c
   // Time the pthread_create() calls
   ```

#### Questions to Answer

1. What is the overhead of creating a thread vs calling a function?
2. How do multiple threads share CPU time?
3. What happens if you don't call `pthread_join()`?
4. When should you use threads vs processes?

---

### 4.4 Exercise 0C: Structs and File I/O

**File:** `exercises/exercise0c_file_io.c`
**Time:** ~15 minutes

#### Core Concepts

- Defining custom structs with `typedef`
- Arrays of structs for storing measurements
- File I/O: `fopen()`, `fprintf()`, `fclose()`
- Error handling with NULL checks
- CSV-style output for data analysis

#### Key Takeaway

RT exercises save timing data to files for visualization and analysis. File I/O is essential for logging performance metrics.

#### Build & Run

```bash
make exercise0c_file_io
./exercise0c_file_io
cat results/exercise0c_data.txt
```

#### Expected Output

**Console:**
```
=== Exercise 0C: Structs and File I/O ===

Generating 100 sample measurements...

Sample measurements:
ID:   0, Latency:  12.345 µs, Status: OK
ID:  10, Latency:  45.678 µs, Status: OK
ID:  20, Latency:  23.456 µs, Status: OK
...

Saving to file: results/exercise0c_data.txt
✓ Saved 100 measurements

Statistics:
  Min latency:  10.123 µs
  Max latency:  98.765 µs
  Avg latency:  45.234 µs

✓ Exercise completed successfully
```

**File Content (results/exercise0c_data.txt):**
```
# Latency Measurements
# ID, Latency_us, Status
0, 12.345, OK
1, 23.456, OK
2, 34.567, OK
...
```

#### Code Walkthrough

**Measurement Structure:**
```c
typedef struct {
    int id;
    double latency_us;
    char status[16];
} measurement_t;
```

**Array of Measurements:**
```c
#define MAX_MEASUREMENTS 1000
measurement_t measurements[MAX_MEASUREMENTS];

// Populate with data
for (int i = 0; i < count; i++) {
    measurements[i].id = i;
    measurements[i].latency_us = (rand() % 100) + 10.0;
    strcpy(measurements[i].status, "OK");
}
```

**Writing to File:**
```c
FILE *file = fopen("results/exercise0c_data.txt", "w");
if (file == NULL) {
    perror("fopen");
    return -1;
}

// Write header
fprintf(file, "# ID, Latency_us, Status\n");

// Write data
for (int i = 0; i < count; i++) {
    fprintf(file, "%d, %.3f, %s\n",
            measurements[i].id,
            measurements[i].latency_us,
            measurements[i].status);
}

fclose(file);
```

**Reading from File:**
```c
FILE *file = fopen("results/exercise0c_data.txt", "r");
char line[256];

while (fgets(line, sizeof(line), file)) {
    if (line[0] == '#') continue;  // Skip comments

    measurement_t m;
    sscanf(line, "%d, %lf, %s", &m.id, &m.latency_us, m.status);
    // Process measurement
}

fclose(file);
```

#### Experiments to Try

1. **Change output format:**
   ```c
   // Try JSON, XML, or binary format
   fprintf(file, "{\"id\": %d, \"latency\": %.3f}\n", ...);
   ```

2. **Add error conditions:**
   ```c
   // Randomly mark some measurements as deadline misses
   if (latency > THRESHOLD) strcpy(status, "MISS");
   ```

3. **Append to existing file:**
   ```c
   FILE *file = fopen("results/data.txt", "a");  // Append mode
   ```

4. **Use gnuplot for visualization:**
   ```bash
   gnuplot -e "plot 'results/exercise0c_data.txt' using 1:2 with lines"
   ```

#### Questions to Answer

1. When should you use text files vs binary files for data?
2. How do you handle large datasets that don't fit in memory?
3. What's the overhead of frequent file I/O operations?
4. How can you ensure data integrity when writing to files?

---

### 4.5 Exercise 0D: Command-Line Arguments and Busy-Wait

**File:** `exercises/exercise0d_busywait.c`
**Time:** ~15 minutes

#### Core Concepts

- Parsing command-line arguments (`argc`, `argv[]`)
- String comparison with `strcmp()`
- String-to-number conversion (`atoi()`, `atof()`)
- Busy-wait loops (CPU-bound computation)
- `volatile` keyword to prevent compiler optimization

#### Key Takeaway

RT exercises use flags like `--rt` to switch between normal and real-time scheduling. Command-line parsing provides flexibility in testing different scenarios.

#### Build & Run

```bash
make exercise0d_busywait
./exercise0d_busywait --help
./exercise0d_busywait --duration 100 --iterations 5
./exercise0d_busywait --duration 50 --verbose
```

#### Expected Output

**Help:**
```
=== Exercise 0D: Command-Line Arguments and Busy-Wait ===

Usage: ./exercise0d_busywait [options]

Options:
  --duration <ms>     Duration of busy-wait in milliseconds (default: 10)
  --iterations <n>    Number of iterations (default: 3)
  --verbose           Enable verbose output
  --help              Show this help message

Examples:
  ./exercise0d_busywait --duration 50 --iterations 5
  ./exercise0d_busywait --duration 100 --verbose
```

**Normal Run:**
```
=== Exercise 0D: Command-Line Arguments and Busy-Wait ===

Configuration:
  Duration: 100 ms
  Iterations: 5
  Verbose: disabled

Starting busy-wait test...

Iteration 1/5: Busy-waiting for 100 ms...
  Target:  100.000 ms
  Actual:  100.234 ms
  Error:   0.234 ms (0.23%)

Iteration 2/5: Busy-waiting for 100 ms...
  Target:  100.000 ms
  Actual:  100.187 ms
  Error:   0.187 ms (0.19%)

...

=== Results ===
Average error: 0.211 ms (0.21%)
Min error: 0.156 ms
Max error: 0.287 ms

✓ Exercise completed successfully
```

#### Code Walkthrough

**Parsing Arguments:**
```c
int duration_ms = 10;
int iterations = 3;
int verbose = 0;

for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--duration") == 0) {
        duration_ms = atoi(argv[++i]);
    } else if (strcmp(argv[i], "--iterations") == 0) {
        iterations = atoi(argv[++i]);
    } else if (strcmp(argv[i], "--verbose") == 0) {
        verbose = 1;
    } else if (strcmp(argv[i], "--help") == 0) {
        print_usage();
        return 0;
    }
}
```

**Busy-Wait Implementation:**
```c
void busy_wait(long duration_us) {
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    volatile long sum = 0;  // volatile prevents optimization
    do {
        // Perform CPU-intensive work
        for (int i = 0; i < 1000; i++) {
            sum += i;
        }

        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (timespec_diff_ns(&start, &now) < duration_us * 1000L);
}
```

**Why `volatile`?**
```c
// Without volatile, compiler might optimize away the loop:
long sum = 0;
for (int i = 0; i < 1000; i++) {
    sum += i;  // Compiler: "This doesn't affect output, remove it!"
}

// With volatile, compiler must keep the loop:
volatile long sum = 0;
for (int i = 0; i < 1000; i++) {
    sum += i;  // Compiler: "Must execute this, can't optimize"
}
```

#### Experiments to Try

1. **Test with different durations:**
   ```bash
   ./exercise0d_busywait --duration 1     # 1ms
   ./exercise0d_busywait --duration 1000  # 1 second
   ```

2. **Compare with `sleep()`:**
   ```c
   // Replace busy_wait with nanosleep and compare accuracy
   ```

3. **Measure CPU usage:**
   ```bash
   # Terminal 1
   ./exercise0d_busywait --duration 5000  # 5 seconds
   
   # Terminal 2
   top  # Observe 100% CPU usage
   ```

4. **Remove `volatile` and observe:**
   ```c
   // Change volatile long sum to long sum
   // Recompile and run - notice timing changes!
   ```

#### Questions to Answer

1. Why does busy-wait consume 100% CPU while `sleep()` doesn't?
2. When would you use busy-wait vs `sleep()` in RT systems?
3. What happens if you remove the `volatile` keyword?
4. How accurate is busy-wait timing compared to `nanosleep()`?

---

### 4.6 Exercise 0E: Scheduling Introduction

**File:** `exercises/exercise0e_scheduling.c`
**Time:** ~20 minutes

#### Core Concepts

- Linux scheduling policies: `SCHED_OTHER`, `SCHED_FIFO`, `SCHED_RR`
- Setting scheduling policy with `sched_setscheduler()`
- Priority ranges: 0 (normal) vs 1-99 (real-time)
- `struct sched_param` for priority configuration
- Handling `EPERM` errors (permission denied)
- Checking capabilities with `sched_get_priority_max()`

#### Key Takeaway

Real-time scheduling requires elevated privileges; RT policies provide deterministic execution by preempting normal tasks.

#### Build & Run

```bash
make exercise0e_scheduling
./exercise0e_scheduling

# For RT scheduling (requires privileges):
sudo ./exercise0e_scheduling
# OR in Docker:
docker-compose run --rm --privileged rt-lab
./exercise0e_scheduling
```

#### Expected Output

**Without Privileges:**
```
=== Exercise 0E: Scheduling Introduction ===

Current scheduling policy: SCHED_OTHER (normal)
Current priority: 0

Attempting to set SCHED_FIFO priority 50...
✗ Failed: Operation not permitted

Note: Real-time scheduling requires elevated privileges.
  Run with 'sudo' or in privileged container.

Demonstrating with SCHED_OTHER instead...
```

**With Privileges:**
```
=== Exercise 0E: Scheduling Introduction ===

System scheduling policy limits:
  SCHED_OTHER: priority 0 to 0
  SCHED_FIFO:  priority 1 to 99
  SCHED_RR:    priority 1 to 99

Current scheduling policy: SCHED_OTHER
Current priority: 0

=== Test 1: Setting SCHED_FIFO ===
Attempting to set SCHED_FIFO priority 50...
✓ Successfully set SCHED_FIFO with priority 50

Current scheduling policy: SCHED_FIFO
Current priority: 50

=== Test 2: Setting SCHED_RR ===
Attempting to set SCHED_RR priority 30...
✓ Successfully set SCHED_RR with priority 30

Current scheduling policy: SCHED_RR
Current priority: 30

=== Test 3: Returning to SCHED_OTHER ===
Resetting to SCHED_OTHER priority 0...
✓ Successfully reset to SCHED_OTHER

✓ Exercise completed successfully
```

#### Code Walkthrough

**Checking Policy Limits:**
```c
printf("SCHED_OTHER: priority %d to %d\n",
       sched_get_priority_min(SCHED_OTHER),
       sched_get_priority_max(SCHED_OTHER));

printf("SCHED_FIFO:  priority %d to %d\n",
       sched_get_priority_min(SCHED_FIFO),
       sched_get_priority_max(SCHED_FIFO));
```

**Getting Current Policy:**
```c
int policy = sched_getscheduler(0);  // 0 = current process
struct sched_param param;
sched_getparam(0, &param);

printf("Policy: %d, Priority: %d\n", policy, param.sched_priority);
```

**Setting RT Policy:**
```c
struct sched_param param;
param.sched_priority = 50;  // 1-99 for RT policies

int result = sched_setscheduler(0, SCHED_FIFO, &param);
if (result != 0) {
    if (errno == EPERM) {
        printf("✗ Permission denied - need elevated privileges\n");
    } else {
        perror("sched_setscheduler");
    }
    return -1;
}

printf("✓ Successfully set SCHED_FIFO priority 50\n");
```

**Policy Names:**
```c
const char* policy_name(int policy) {
    switch (policy) {
        case SCHED_OTHER: return "SCHED_OTHER";
        case SCHED_FIFO:  return "SCHED_FIFO";
        case SCHED_RR:    return "SCHED_RR";
        default:          return "UNKNOWN";
    }
}
```

#### Scheduling Policies Explained

**SCHED_OTHER (Default):**
- Time-sharing policy for normal tasks
- Priority 0 (can use `nice` values -20 to +19)
- Tasks get fair CPU time
- Preempted by RT tasks

**SCHED_FIFO (Real-Time):**
- First-In-First-Out
- Priority 1-99 (higher = more important)
- Runs until it yields, blocks, or is preempted by higher priority
- No time-slicing at same priority

**SCHED_RR (Real-Time):**
- Round-Robin
- Priority 1-99
- Like FIFO but with time-slicing at same priority level
- Good for multiple RT tasks at same priority

#### Experiments to Try

1. **Test priority preemption:**
   ```c
   // Create two threads with different priorities
   // Observe which runs first
   ```

2. **Measure scheduling overhead:**
   ```c
   // Time the sched_setscheduler() call
   ```

3. **Test with different priorities:**
   ```bash
   # Try priorities: 1, 50, 99
   # Observe system responsiveness
   ```

4. **Check thread scheduling:**
   ```c
   pthread_attr_t attr;
   pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
   pthread_create(&thread, &attr, func, NULL);
   ```

#### Questions to Answer

1. Why does RT scheduling require elevated privileges?
2. What's the difference between SCHED_FIFO and SCHED_RR?
3. When would you use priority 99 vs priority 1?
4. What happens if two SCHED_FIFO tasks at same priority compete?

---

### 4.7 Exercise 0F: Statistics and Array Manipulation

**File:** `exercises/exercise0f_statistics.c`
**Time:** ~15 minutes

#### Core Concepts

- Calculating min, max, average, jitter
- Jitter = max - min (critical RT metric)
- Bubble sort algorithm
- Percentiles (50th, 95th, 99th)
- Distribution analysis
- Formatted table output with `printf`

#### Key Takeaway

RT systems care about worst-case latency (max, 99th percentile), not just averages. Statistical analysis reveals system behavior under various conditions.

#### Build & Run

```bash
make exercise0f_statistics
./exercise0f_statistics
```

#### Expected Output

```
=== Exercise 0F: Statistics and Array Manipulation ===

Generating 1000 sample measurements...

Raw data (first 10):
  [0]: 45.2 µs
  [1]: 67.8 µs
  [2]: 23.4 µs
  ...

=== Basic Statistics ===
Count:      1000
Min:        10.5 µs
Max:        198.7 µs
Average:    52.3 µs
Jitter:     188.2 µs (max - min)

=== Percentile Analysis ===
50th (median):  51.2 µs
75th:           78.9 µs
90th:           105.6 µs
95th:           134.2 µs
99th:           176.5 µs

=== Distribution ===
Range             Count    Percentage
< 25 µs           123      12.3%
25-50 µs          456      45.6%
50-75 µs          278      27.8%
75-100 µs         98       9.8%
> 100 µs          45       4.5%

✓ Exercise completed successfully
```

#### Code Walkthrough

**Basic Statistics:**
```c
double calculate_statistics(double *data, int count,
                           double *min, double *max, double *avg) {
    *min = data[0];
    *max = data[0];
    double sum = 0;

    for (int i = 0; i < count; i++) {
        if (data[i] < *min) *min = data[i];
        if (data[i] > *max) *max = data[i];
        sum += data[i];
    }

    *avg = sum / count;
    return *max - *min;  // jitter
}
```

**Sorting (Bubble Sort):**
```c
void sort_array(double *data, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                double temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}
```

**Percentile Calculation:**
```c
double calculate_percentile(double *sorted_data, int count, int percentile) {
    // percentile must be 0-100
    int index = (percentile * count) / 100;
    if (index >= count) index = count - 1;
    return sorted_data[index];
}

// Usage:
sort_array(data, count);
double median = calculate_percentile(data, count, 50);
double p95 = calculate_percentile(data, count, 95);
double p99 = calculate_percentile(data, count, 99);
```

**Distribution Analysis:**
```c
void analyze_distribution(double *data, int count) {
    int bins[5] = {0};  // 5 bins

    for (int i = 0; i < count; i++) {
        if (data[i] < 25)          bins[0]++;
        else if (data[i] < 50)     bins[1]++;
        else if (data[i] < 75)     bins[2]++;
        else if (data[i] < 100)    bins[3]++;
        else                       bins[4]++;
    }

    printf("< 25 µs:    %4d  (%.1f%%)\n", bins[0], 100.0 * bins[0] / count);
    printf("25-50 µs:   %4d  (%.1f%%)\n", bins[1], 100.0 * bins[1] / count);
    // ... etc
}
```

#### Why These Metrics Matter for RT

**Average:** Not enough! Hides outliers.
```
Samples: [10, 10, 10, 10, 1000]
Average: 208 µs  ← Looks okay
Max:     1000 µs ← Deadline miss!
```

**Jitter:** Variation in latency (unpredictability)
```
Low jitter:  [50, 51, 49, 50, 51] → jitter = 2 µs ✓ Good!
High jitter: [10, 90, 20, 80, 30] → jitter = 80 µs ✗ Bad!
```

**99th Percentile:** Worst-case for 99% of samples
```
If p99 = 100 µs and deadline = 150 µs:
→ 99% of operations meet deadline ✓
→ 1% might miss (need investigation)
```

#### Experiments to Try

1. **Add more percentiles:**
   ```c
   double p999 = calculate_percentile(data, count, 99.9);
   ```

2. **Visualize distribution:**
   ```c
   // Print ASCII histogram
   for (int i = 0; i < bins; i++) {
       printf("%s", repeat_char('*', bins[i] / 10));
   }
   ```

3. **Compare sorting algorithms:**
   ```c
   // Implement quicksort and compare performance
   ```

4. **Calculate standard deviation:**
   ```c
   double stddev = calculate_stddev(data, count, avg);
   ```

#### Questions to Answer

1. Why is the 99th percentile more important than average for RT?
2. How much jitter is acceptable for a 10ms deadline?
3. What causes the long-tail in latency distributions?
4. When would you use median vs mean?

---

### 4.8 Concept-to-Exercise Mapping

This table shows which Week 0 skills are used in the main RT exercises:

| C Concept | Introduced in | Used in Main Exercises |
|-----------|---------------|------------------------|
| `clock_gettime()`, `struct timespec` | 0A | Exercise 1, 2, 3 (all timing) |
| `pthread_create()`, `pthread_join()` | 0B | Exercise 2 (multi-policy threading), 4-9 |
| Structs, arrays, file I/O | 0C | Exercise 1, 3 (saving results) |
| `argc`/`argv`, busy-wait, `volatile` | 0D | Exercise 2, 3 (computation, flags) |
| `sched_setscheduler()`, RT policies | 0E | Exercise 2, 3, 4, 5 (RT scheduling) |
| Min/max/avg, jitter, sorting | 0F | Exercise 1, 2, 3, 9 (analysis) |

**Exercise Dependencies:**
```
Week 0 Foundations
├── Exercise 1: Latency Measurement
│   └── Uses: 0A (time), 0C (file I/O), 0F (statistics)
│
├── Exercise 2: Scheduling Comparison
│   └── Uses: 0B (threading), 0D (busy-wait), 0E (scheduling), 0F (sorting)
│
└── Exercise 3: Periodic Tasks
    └── Uses: 0A (time), 0D (command-line), 0E (RT scheduling), 0F (statistics)
```

---

### 4.9 Key C Idioms for RT Programming

These patterns appear repeatedly in RT code. Master them now:

#### 1. Time Arithmetic Pattern

```c
long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    long diff_sec = end->tv_sec - start->tv_sec;
    long diff_nsec = end->tv_nsec - start->tv_nsec;
    return (diff_sec * 1000000000L) + diff_nsec;
}

// Usage:
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
// ... do work ...
clock_gettime(CLOCK_MONOTONIC, &end);
long duration_ns = timespec_diff_ns(&start, &end);
```

*Used in: Exercises 0A, 1, 2, 3, 4-9*

#### 2. Thread Data Passing Pattern

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

// Usage:
thread_data_t data = {.thread_id = 0, .config = 100};
pthread_create(&thread, NULL, worker, &data);
pthread_join(thread, NULL);
printf("Result: %ld\n", data.result);
```

*Used in: Exercises 0B, 2, 4-9*

#### 3. RT Scheduling Pattern

```c
struct sched_param param;
param.sched_priority = 80;

if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
    perror("sched_setscheduler");
    // Handle error or fall back to SCHED_OTHER
}
```

*Used in: Exercises 0E, 2, 3, 4, 5*

#### 4. Busy-Wait Pattern

```c
void do_computation(long duration_us) {
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    volatile long sum = 0;
    do {
        for (int i = 0; i < 1000; i++) sum += i;
        clock_gettime(CLOCK_MONOTONIC, &now);
    } while (timespec_diff_ns(&start, &now) < duration_us * 1000L);
}
```

*Used in: Exercises 0D, 2, 3, 8, 9*

#### 5. Statistical Analysis Pattern

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

*Used in: Exercises 0F, 1, 2, 3, 9*

---

### Common Pitfalls and Best Practices

#### DO:
- ✓ Always check return values from system calls
- ✓ Use `CLOCK_MONOTONIC` for timing (never `CLOCK_REALTIME` for RT)
- ✓ Declare timing loop variables as `volatile` to prevent optimization
- ✓ Initialize structs with designated initializers: `.field = value`
- ✓ Close files with `fclose()` after opening

#### DON'T:
- ✗ Assume sleep/nanosleep durations are exact
- ✗ Forget to link `-pthread -lrt` flags
- ✗ Mix seconds and nanoseconds without conversion
- ✗ Use `printf` inside timing-critical loops (adds latency)
- ✗ Run RT scheduling (`SCHED_FIFO`) without yields/sleeps (can lock system!)

---

### Progression Summary

After completing Week 0, you'll be ready to:

1. ✓ Measure timing with nanosecond precision
2. ✓ Create and manage concurrent tasks
3. ✓ Configure real-time scheduling policies
4. ✓ Analyze system performance
5. ✓ Build and debug real-time applications

**Next Step:** [Week 1: Core Real-Time Concepts](#5-week-1-core-real-time-concepts)

---

# Part III: Real-Time Fundamentals (Week 1)

## 5. Week 1: Core Real-Time Concepts

### 5.1 Overview: Latency, Jitter, and Determinism

Week 1 focuses on fundamental RT concepts: **latency, jitter, determinism, and scheduling policies**. You'll measure these properties and understand the difference between real-time and general-purpose scheduling.

**Learning Objectives:**
- Understand latency vs jitter
- Differentiate hard vs soft real-time systems
- Compare Linux scheduling policies (SCHED_OTHER, SCHED_FIFO, SCHED_RR)
- Measure and analyze timing characteristics
- Recognize the impact of system load on determinism

**Week Structure:**
```
Exercise 1: Latency Measurement
     ↓
Exercise 2: Scheduling Policy Comparison
     ↓
Exercise 3: Periodic Task Simulation
```

**Time Required:** ~3-4 hours for all exercises and experiments

---

### 5.2 Exercise 1: Basic Latency Measurement

**File:** `exercises/exercise1.c`
**Time:** ~45 minutes

#### Concept

This exercise measures how accurately the system can sleep for a specified interval. The **deviation** from the target is the latency, and the **variation** in latency is jitter.

**Key Terms:**
- **Latency:** Time between when an event should occur and when it actually occurs
- **Jitter:** Variation in latency (max - min)
- **Determinism:** Predictability of system behavior

#### Run the Exercise

```bash
make exercise1
./exercise1
```

#### Expected Output

```
=== Exercise 1: Basic Latency Measurement ===
Target sleep interval: 1000000 ns (1.000 ms)
Running 1000 iterations...

=== Results ===
Average latency: 45230 ns (45.230 µs)
Min deviation:   12450 ns (12.450 µs)
Max deviation:   234120 ns (234.120 µs)
Jitter:          221670 ns (221.670 µs)

Data saved to: results/latency_raw.txt
```

#### Visualization

```bash
python3 scripts/plot_latency.py results/latency_raw.txt
```

This generates a graph showing latency distribution over time.

#### What's Being Measured

The exercise repeatedly:
1. Records start time with `clock_gettime()`
2. Calls `clock_nanosleep()` for target interval (1ms)
3. Records end time
4. Calculates deviation: `actual_time - target_time`
5. Collects statistics (min, max, average, jitter)

**Code Pattern:**
```c
struct timespec target, start, end;
target.tv_sec = 0;
target.tv_nsec = 1000000;  // 1ms

clock_gettime(CLOCK_MONOTONIC, &start);
clock_nanosleep(CLOCK_MONOTONIC, 0, &target, NULL);
clock_gettime(CLOCK_MONOTONIC, &end);

long actual_ns = timespec_diff_ns(&start, &end);
long deviation_ns = actual_ns - 1000000;
```

#### Experiments to Try

**1. Baseline measurement** - Run without any load:
```bash
./exercise1
```

**2. Under CPU stress:**
```bash
# Terminal 1
stress-ng --cpu 4

# Terminal 2
./exercise1
```

Expected: Higher jitter under load

**3. Under I/O stress:**
```bash
stress-ng --io 2 &
./exercise1
```

**4. Compare different sleep intervals:**
```c
// Edit exercise1.c, change TARGET_INTERVAL_NS
#define TARGET_INTERVAL_NS 100000    // 100µs
#define TARGET_INTERVAL_NS 10000000  // 10ms
```

#### Questions to Answer

1. What causes latency deviation from the target sleep time?
2. How does jitter change under stress?
3. Why is low jitter more important than low latency for RT systems?
4. What jitter level would be acceptable for a 10ms control loop?

#### Analysis

**Typical Results (Docker environment):**
- Baseline jitter: 100-500 µs
- Under stress: 500-2000 µs (10x worse)
- RT scheduling reduces jitter by 50-80%

**Why Jitter Matters:**
```
Scenario: 10ms control loop with 5ms computation

Low jitter (100µs):
  Cycles: 9.9ms, 10.0ms, 10.1ms, 10.0ms
  ✓ Predictable! All cycles fit within 10ms budget

High jitter (5ms):
  Cycles: 8ms, 12ms, 9ms, 11ms
  ✗ Unpredictable! Some cycles exceed 10ms deadline
```

---

### 5.3 Exercise 2: Scheduling Policy Comparison

**File:** `exercises/exercise2.c`
**Time:** ~60 minutes

#### Concept

Linux supports multiple scheduling policies with different characteristics:
- **SCHED_OTHER:** Default time-sharing (CFS - Completely Fair Scheduler)
- **SCHED_FIFO:** Real-time FIFO (runs until yields/blocks)
- **SCHED_RR:** Real-time Round-Robin (time-sliced RT)

This exercise creates multiple threads with different policies and compares their execution.

#### Run the Exercise

```bash
make exercise2
./exercise2
```

#### Expected Output

```
=== Exercise 2: Scheduling Policy Comparison ===

Creating 3 threads with different scheduling policies...

Thread NORMAL (SCHED_OTHER, priority 0): Starting
Thread FIFO (SCHED_FIFO, priority 80): Starting
Thread RR (SCHED_RR, priority 80): Starting

Thread FIFO: Iteration 0/100
Thread RR: Iteration 0/100
Thread NORMAL: Iteration 0/100
...

=== Results ===
Thread NORMAL: Completed in 523.45 ms
Thread FIFO:   Completed in 312.12 ms
Thread RR:     Completed in 315.87 ms

Analysis:
✓ RT threads (FIFO/RR) completed ~40% faster than NORMAL
✓ FIFO and RR had similar performance (no RT contention)
```

#### Scheduling Policies Explained

**SCHED_OTHER (Default):**
- Priority: 0 (uses nice values -20 to +19 for relative priority)
- Preemption: Yes, time-sliced fairly among tasks
- Use case: General-purpose applications
- Behavior: Tries to give all tasks fair CPU time

**SCHED_FIFO (Real-Time):**
- Priority: 1-99 (higher = more important)
- Preemption: Only by higher-priority RT tasks
- Use case: Hard real-time tasks
- Behavior: Runs until it yields, blocks, or is preempted
- **Warning:** Can starve lower-priority tasks!

**SCHED_RR (Real-Time):**
- Priority: 1-99
- Preemption: By higher-priority RT tasks + time-slicing at same priority
- Use case: Multiple RT tasks at same priority level
- Behavior: Like FIFO but with round-robin at same priority

**Priority Hierarchy:**
```
SCHED_FIFO/RR Priority 99  ← Highest
        ...
SCHED_FIFO/RR Priority 1
        ↓
SCHED_OTHER (all tasks)    ← Lowest
```

#### Code Walkthrough

**Thread Configuration:**
```c
typedef struct {
    int policy;
    int priority;
    const char *name;
} thread_config_t;

thread_config_t configs[] = {
    {SCHED_OTHER, 0,  "NORMAL"},
    {SCHED_FIFO,  80, "FIFO"},
    {SCHED_RR,    80, "RR"}
};
```

**Setting Thread Scheduling:**
```c
void* worker_thread(void* arg) {
    thread_config_t *config = (thread_config_t *)arg;

    // Set scheduling policy
    struct sched_param param;
    param.sched_priority = config->priority;

    if (pthread_setschedparam(pthread_self(), config->policy, &param) != 0) {
        perror("pthread_setschedparam");
    }

    // Do work...
    for (int i = 0; i < ITERATIONS; i++) {
        do_computation(WORK_DURATION_US);
    }
}
```

#### Experiments to Try

**1. Compare with system load:**
```bash
stress-ng --cpu 4 &
./exercise2
```
Expected: RT threads maintain performance; NORMAL thread slows down

**2. Modify priorities in code:**
```c
// Try different RT priorities
{SCHED_FIFO, 90, "HIGH_FIFO"},
{SCHED_FIFO, 50, "LOW_FIFO"}
```

**3. Run with nice value:**
```bash
nice -n -20 ./exercise2  # Higher priority for NORMAL
```

**4. Create priority inversion scenario (preview Week 2):**
```c
// Low priority holds resource, high priority waits
```

#### Questions to Answer

1. Why do SCHED_FIFO and SCHED_RR complete at similar times?
2. When would you choose RR over FIFO?
3. What happens if two FIFO tasks at same priority compete?
4. How does SCHED_OTHER handle competing tasks differently?
5. What are the dangers of using SCHED_FIFO priority 99?

#### Policy Selection Guide

| Requirement | Recommended Policy |
|-------------|-------------------|
| Hard RT deadline | SCHED_FIFO |
| Multiple RT tasks, same priority | SCHED_RR |
| Soft RT, best effort | SCHED_OTHER |
| Mixed priorities | SCHED_FIFO with hierarchy |
| Periodic tasks | SCHED_FIFO |

---

### 5.4 Exercise 3: Periodic Task Simulation

**File:** `exercises/exercise3.c`
**Time:** ~90 minutes

#### Concept

Many RT systems have **periodic tasks** that must execute at fixed intervals (e.g., control loops, sensor sampling). This exercise simulates a 10ms periodic task and detects deadline misses.

**Real-World Examples:**
- Motor control: 1-10ms cycle
- Sensor sampling: 10-100ms cycle
- Network packet processing: <1ms cycle
- Video frame processing: 16.67ms (60 FPS)

#### Run the Exercise

**Normal scheduling:**
```bash
make exercise3
./exercise3
```

**Real-time scheduling:**
```bash
./exercise3 --rt
```

#### Expected Output

**Without RT scheduling:**
```
=== Exercise 3: Periodic Task Simulation ===
Running with SCHED_OTHER

Task Configuration:
  Period: 10 ms
  Computation time: 2000 µs
  Duration: 5 seconds

Cycle | Latency (µs) | Status
------|--------------|-------
    0 |         45.3 | OK
  100 |        123.7 | OK
  200 |        567.2 | OK
  300 |       1234.5 | ⚠ HIGH
  400 |        234.1 | OK
  ...

=== Results ===
Total cycles:      500
Missed deadlines:  3 (0.60%)

Latency Statistics:
  Min:     12 µs
  Max:     1456 µs
  Average: 156 µs
  Jitter:  1444 µs  ← High variation!
```

**With RT scheduling:**
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
  Jitter:  37 µs  ← Much better!
```

#### What's Being Measured

The exercise implements a classic periodic task pattern:

```c
struct timespec next_activation;
clock_gettime(CLOCK_MONOTONIC, &next_activation);

for (int cycle = 0; cycle < total_cycles; cycle++) {
    // 1. Wait until next period
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);

    // 2. Measure wakeup latency
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long latency_ns = timespec_diff_ns(&next_activation, &now);

    // 3. Do computation
    do_work(COMPUTATION_USEC);

    // 4. Check if deadline was missed
    clock_gettime(CLOCK_MONOTONIC, &now);
    long cycle_time_ns = timespec_diff_ns(&next_activation, &now);
    if (cycle_time_ns > PERIOD_NS) {
        deadline_misses++;
    }

    // 5. Calculate next activation (absolute time)
    next_activation.tv_nsec += PERIOD_NS;
    normalize_timespec(&next_activation);
}
```

**Key Pattern: Absolute Time Scheduling**
```c
// ✓ GOOD: Absolute time (no drift)
next_activation.tv_nsec += 10000000;  // Add 10ms
clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);

// ✗ BAD: Relative time (accumulates error)
struct timespec delay = {.tv_sec = 0, .tv_nsec = 10000000};
clock_nanosleep(CLOCK_MONOTONIC, 0, &delay, NULL);  // Drifts over time!
```

#### Experiments to Try

**1. Compare RT vs Normal scheduling:**
```bash
# Normal
./exercise3 > normal.log

# Real-time
./exercise3 --rt > rt.log

# Compare jitter
grep "Jitter:" normal.log rt.log
```

**2. Increase computation time** to cause deadline misses:
```c
// Edit exercise3.c
#define COMPUTATION_USEC 8000  // 8ms computation in 10ms period
```
Rebuild and run:
```bash
make exercise3
./exercise3 --rt
```
Expected: Deadline misses! (80% utilization is too high)

**3. System under load:**
```bash
stress-ng --cpu 4 --io 2 &
./exercise3 --rt
```
Expected: RT scheduling maintains performance

**4. Find the breaking point:**
Gradually increase `COMPUTATION_USEC` until deadlines are missed:
```
2000 µs → 0% misses ✓
4000 µs → 0% misses ✓
6000 µs → 0% misses ✓
8000 µs → 15% misses ✗
9000 µs → 80% misses ✗
```

This demonstrates WCET (Worst Case Execution Time) analysis!

#### Utilization Analysis

**Formula:** `U = C / T`
- `U` = CPU utilization (must be < 1.0)
- `C` = Computation time
- `T` = Period

**Example:**
```
Period: 10ms
Computation: 2ms
U = 2/10 = 0.2 (20% CPU)  ✓ Safe

Period: 10ms
Computation: 9ms
U = 9/10 = 0.9 (90% CPU)  ✗ Dangerous! (no margin for jitter)
```

**Safe Utilization Limits:**
- Theory: 100% possible with perfect scheduling
- Practice: 70-80% recommended (leave margin for OS overhead, jitter)
- Docker: 60-70% (additional container overhead)

#### Questions to Answer

1. How does RT scheduling affect jitter compared to normal?
2. At what computation time do deadlines start being missed?
3. What is the utilization factor? (computation_time / period)
4. Why can't we use 100% CPU utilization in RT systems?
5. How does absolute vs relative time scheduling affect drift?

#### Multiple Periodic Tasks

**Example:** Control system with 3 tasks
```
Task A: Sensor read  - Period: 5ms,  Execution: 0.5ms, U = 0.10
Task B: Controller   - Period: 10ms, Execution: 2ms,   U = 0.20
Task C: Actuator     - Period: 20ms, Execution: 1ms,   U = 0.05

Total utilization: U = 0.35 (35%)  ✓ Schedulable!
```

**Rate Monotonic Priority Assignment:**
- Shorter period → Higher priority
- Task A (5ms) gets priority 90
- Task B (10ms) gets priority 80
- Task C (20ms) gets priority 70

---

### 5.5 Hard vs Soft Real-Time Systems

#### Hard Real-Time

**Definition:** Missing a deadline is a **system failure**.

**Characteristics:**
- Deadline miss = catastrophic consequence
- Must prove schedulability formally
- Requires WCET analysis
- Zero tolerance for deadline violations

**Examples:**
- Airbag deployment (5-10ms deadline)
- Anti-lock braking system (10-20ms)
- Flight control systems (1-10ms)
- Medical device pacemaker (milliseconds)
- Industrial robot control (1-10ms)

**Requirements:**
- ✓ Formal verification
- ✓ WCET guarantees
- ✓ Schedulability proof
- ✓ Resource reservation
- ✓ Real-time OS (PREEMPT_RT)

#### Soft Real-Time

**Definition:** Missing a deadline causes **degraded performance** but not failure.

**Characteristics:**
- Deadline miss = quality degradation
- Best-effort scheduling acceptable
- Occasional misses tolerable
- Focus on average-case performance

**Examples:**
- Video streaming (frame drop OK)
- Audio playback (glitch acceptable)
- Video games (frame rate dip)
- Network VoIP (packet loss tolerable)
- GUI responsiveness (lag noticeable but OK)

**Requirements:**
- ✓ Low average latency
- ✓ Bounded worst-case latency
- ✓ Statistical guarantees (99th percentile)
- ✗ Formal proof not required

#### Comparison Table

| Aspect | Hard RT | Soft RT |
|--------|---------|---------|
| Deadline miss | System failure | Quality loss |
| Tolerance | 0% | <1% acceptable |
| Verification | Formal proof | Testing/profiling |
| Scheduling | Guaranteed | Best-effort |
| Priority | Safety-critical | User experience |
| Cost | Very high | Moderate |
| OS | RTOS required | Standard OK |

#### Example: Video Streaming

**Hard RT approach:**
- Every frame must arrive within 16.67ms (60 FPS)
- Buffer underrun causes system halt
- Formal analysis of network timing
- Dedicated bandwidth reservation

**Soft RT approach:**
- Target 60 FPS, tolerate 59 FPS average
- Dropped frames cause brief glitch
- Adaptive bitrate based on network
- Best-effort bandwidth sharing

---

### 5.6 Key Metrics and Analysis Methods

#### Essential Metrics

**1. Latency**
```
Definition: Time from event trigger to response
Measured: Target time - Actual time
Unit: Microseconds (µs) or nanoseconds (ns)
Goal: Minimize average and worst-case
```

**2. Jitter**
```
Definition: Variation in latency
Formula: Jitter = Max latency - Min latency
Unit: Same as latency
Goal: Minimize (predictability > speed)
```

**3. Deadline Miss Rate**
```
Definition: Percentage of cycles that exceed deadline
Formula: (Missed / Total) × 100%
Unit: Percentage
Goal: 0% for hard RT, <0.1% for soft RT
```

**4. Utilization**
```
Definition: Fraction of CPU time used
Formula: U = Σ(Execution time / Period)
Unit: Percentage (0-100%)
Goal: <70% for safety margin
```

**5. Response Time**
```
Definition: End-to-end time from input to output
Includes: Latency + computation + communication
Unit: Milliseconds or microseconds
Goal: Meet system requirements
```

#### Statistical Analysis

**Why Percentiles Matter:**
```
Average: 50µs      ← Looks good!
Max:     10ms      ← Disaster!
99th:    8ms       ← Most cycles bad
95th:    5ms
Median:  45µs

Conclusion: Average hides the problem!
```

**Distribution Example:**
```
<100µs:    800 samples (80%)  ✓ Good
100-500µs: 150 samples (15%)  ⚠ OK
500µs-1ms:  40 samples (4%)   ⚠ Borderline
>1ms:       10 samples (1%)   ✗ Problem!
```

#### Rate Monotonic Analysis (RMA)

**Theorem:** For periodic tasks with fixed priorities:
```
U ≤ n(2^(1/n) - 1)

Where:
  U = Total utilization
  n = Number of tasks

Examples:
  n=1: U ≤ 1.00 (100%)
  n=2: U ≤ 0.828 (82.8%)
  n=3: U ≤ 0.780 (78.0%)
  n→∞: U ≤ 0.693 (69.3%)
```

**Application:**
```
3 tasks with utilizations: 0.20, 0.30, 0.15
Total: U = 0.65 (65%)
Limit: 0.780 (78%)
Result: ✓ Schedulable by RMA!
```

---

### 5.7 Using cyclictest (Advanced)

**cyclictest** is the industry-standard latency measurement tool for Linux RT systems.

#### Basic Usage

```bash
# Simple test
cyclictest -p 80 -t1 -n -i 1000 -l 10000

# Explanation:
# -p 80      : priority 80 (RT FIFO)
# -t1        : 1 thread
# -n         : use clock_nanosleep (like our exercises)
# -i 1000    : interval 1000µs (1ms)
# -l 10000   : 10000 loops
```

**Output:**
```
T: 0 ( 1234) P:80 I:1000 C:  10000 Min:     5 Act:   12 Avg:   18 Max:     87
```

Interpretation:
- Thread 0, PID 1234, Priority 80
- 1000µs interval, 10000 cycles
- Min latency: 5µs
- Actual (last): 12µs
- Average: 18µs
- Max: 87µs

#### With System Load

```bash
# Terminal 1: Start load
stress-ng --cpu 4 --io 2 --vm 1

# Terminal 2: Measure latency
cyclictest -p 80 -t1 -n -i 1000 -l 10000 -q
```

#### Histogram Output

```bash
cyclictest -p 80 -t1 -i 1000 -l 10000 -h 100 -q > histogram.txt
```

Creates distribution histogram:
```
# Latency histogram (microseconds)
0-10:    ████████████████████ 8234
10-20:   ████████ 1456
20-30:   ██ 234
30-40:   ▌ 56
40-50:    12
50-100:   8
```

#### Compare with Exercise 1

```bash
# Run both and compare jitter
./exercise1
cyclictest -p 80 -t1 -n -i 1000 -l 1000 -q

# Extract jitter values
grep "Jitter:" results/latency_raw.txt
```

Expected: Similar results (both use same APIs)

---

### 5.8 Week 1 Assignment and Deliverables

#### Part 1: Measurement Report

Run all three exercises and create a report with:

**Required Measurements:**
1. Exercise 1: Baseline, stressed, and under I/O load
2. Exercise 2: All three policies, with and without load
3. Exercise 3: Normal vs RT, varying computation times

**Data Format:**
```csv
Exercise,Scenario,AvgLatency_us,Jitter_us,DeadlineMisses_%
1,Baseline,45.2,221.5,N/A
1,CPU_Stress,123.7,1234.5,N/A
...
```

**Required Graphs:**
- Latency distribution over time (Exercise 1)
- Policy comparison bar chart (Exercise 2)
- Deadline miss rate vs utilization (Exercise 3)

#### Part 2: Critical Analysis

Answer these questions in your report:

**Conceptual:**
1. Why does Docker without RT kernel still show scheduling differences?
2. Explain why jitter is more critical than average latency for RT.
3. What causes the long tail in latency distributions?

**Analytical:**
1. Calculate the maximum safe utilization for Exercise 3
2. Design a simple control system with 3 periodic tasks:
   - Sensor read: 5ms period, 0.5ms execution
   - Controller: 10ms period, 2ms execution
   - Actuator: 20ms period, 1ms execution

   **Answer:** Are these tasks schedulable? Prove using Rate Monotonic Analysis.

**Practical:**
1. At what system load does Exercise 1 jitter exceed 1ms?
2. How many deadline misses occur in Exercise 3 when computation = 8ms?

#### Part 3: Experimentation

Modify Exercise 3 to:

**Required Changes:**
1. Add a second periodic task with different period (e.g., 20ms)
2. Implement CPU affinity (pin to core 0 using `pthread_setaffinity_np`)
3. Add deadline miss logging to a file
4. Calculate and display CPU utilization percentage

---

**Next:** [Week 2: Priority Inversion and Synchronization](#6-week-2-priority-inversion-and-synchronization)

---

# Part IV: Synchronization Problems (Week 2)

## 6. Week 2: Priority Inversion and Synchronization

### 6.1 Understanding Priority Inversion

Week 2 focuses on **synchronization problems in real-time systems**, specifically priority inversion and its solution through priority inheritance protocol.

**Learning Objectives:**
- Understand the priority inversion problem
- Recognize unbounded vs bounded blocking
- Apply Priority Inheritance Protocol (PIP)
- Compare regular mutexes vs PI mutexes
- Analyze the impact on real-time guarantees

#### What is Priority Inversion?

**Priority inversion** occurs when a high-priority task is indirectly blocked by a lower-priority task through resource contention.

**Classic Scenario:**
```
Time    Low (P=10)        Medium (P=50)      High (P=90)
----    ----------        -------------      -----------
  0     lock(mutex) ✓
  1                                          needs mutex → BLOCKS
  2                       becomes ready
  3     PREEMPTED ✗       runs...
  4                       runs...
  5                       runs...            STILL BLOCKED!
  6                       done
  7     resumes
  8     unlock(mutex)
  9                                          finally runs

Problem: High waited for Medium to finish!
```

**The Issue:**
1. Low-priority task (L) acquires a mutex
2. High-priority task (H) tries to acquire the same mutex → blocks on L
3. Medium-priority task (M) preempts L
4. **Result:** H is effectively blocked by M, violating priority ordering!

This is called **priority inversion** because the effective priority order is inverted.

---

### 6.2 Unbounded vs Bounded Blocking

#### Unbounded Priority Inversion

**Definition:** High-priority task can block for an indefinite time.

**Characteristics:**
- Blocking duration depends on lower-priority task execution
- Can be arbitrarily long
- Unpredictable and dangerous for RT systems
- Can cause deadline misses

**Example:**
```
High blocks on Low's mutex
Medium preempts Low
If Medium runs for 1 second, High blocks for 1 second!
```

**Demonstrated in:** Exercise 4

#### Bounded Priority Inversion

**Definition:** High-priority task blocks for at most the duration of the critical section.

**Characteristics:**
- Blocking time is bounded by critical section length
- Predictable and analyzable
- Safe for RT systems (with proper design)
- Achieved through Priority Inheritance Protocol

**Example:**
```
High blocks on Low's mutex
Low inherits High's priority (can't be preempted by Medium)
High blocks only for Low's critical section duration
```

**Demonstrated in:** Exercise 5

#### Comparison Table

| Aspect | Unbounded (Ex 4) | Bounded (Ex 5) |
|--------|------------------|----------------|
| Blocking time | Variable, unpredictable | Fixed, predictable |
| Medium interference | YES | NO |
| RT suitability | ✗ Dangerous | ✓ Safe |
| Analysis | Impossible | Possible |
| Max blocking | Unknown | Critical section length |

---

### 6.3 Exercise 4: Priority Inversion Demonstration

**File:** `exercises/exercise4.c`
**Time:** ~45 minutes

#### Concept

Demonstrate the unbounded priority inversion problem using a regular mutex.

**Scenario Setup:**
- **Low task (P=10):** Holds mutex for 50ms
- **High task (P=90):** Needs same mutex
- **Medium task (P=50):** CPU-intensive work for 100ms

#### Run the Exercise

```bash
make exercise4
./exercise4
```

#### Expected Output

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

Priority inversion factor: 2.4x
```

#### What's Happening?

**Timeline:**
```
 0ms: Low acquires regular mutex, starts 50ms work
30ms: High tries mutex → blocks on Low
55ms: Medium becomes runnable → preempts Low (50 > 10)
     Medium runs for 100ms while High is blocked
155ms: Medium completes
160ms: Low resumes and finishes
170ms: Low releases mutex
170ms: High finally unblocks

Total High blocking: 140ms (should be ~50ms!)
```

#### Code Analysis

**Regular Mutex (NO priority inheritance):**
```c
pthread_mutex_t resource_mutex;
pthread_mutex_init(&resource_mutex, NULL);  // Default attributes
```

**Low Priority Task:**
```c
void* low_priority_task(void* arg) {
    set_priority(SCHED_FIFO, 10);

    pthread_mutex_lock(&resource_mutex);
    printf("LOW: Acquired mutex\n");

    // Do work (50ms)
    do_work(50000);

    pthread_mutex_unlock(&resource_mutex);
}
```

**High Priority Task:**
```c
void* high_priority_task(void* arg) {
    set_priority(SCHED_FIFO, 90);

    struct timespec block_start, block_end;
    clock_gettime(CLOCK_MONOTONIC, &block_start);

    pthread_mutex_lock(&resource_mutex);  // BLOCKS!

    clock_gettime(CLOCK_MONOTONIC, &block_end);
    long blocked_ms = timespec_diff_ns(&block_start, &block_end) / 1000000;

    printf("HIGH: Blocked for %ld ms\n", blocked_ms);
}
```

**Medium Priority Task:**
```c
void* medium_priority_task(void* arg) {
    set_priority(SCHED_FIFO, 50);

    printf("MEDIUM: Starting work (causing priority inversion)\n");
    do_work(100000);  // 100ms - blocks High indirectly!
    printf("MEDIUM: Completed\n");
}
```

#### Experiments to Try

**1. Vary Medium's execution time:**
```c
#define MEDIUM_WORK_MS 200  // Longer = worse blocking
```
Rebuild and run - observe High's blocking time increases proportionally.

**2. Remove Medium task:**
```c
// Comment out Medium thread creation
// pthread_create(&medium_thread, ...);
```
Expected: High blocks only for Low's critical section (~50ms).

**3. Add multiple Medium tasks:**
```c
pthread_t medium_threads[5];
for (int i = 0; i < 5; i++) {
    pthread_create(&medium_threads[i], NULL, medium_priority_task, NULL);
}
```
Expected: Even worse blocking for High!

**4. Measure with different priorities:**
```c
// Try: Low=10, Medium=70, High=90
// Or:  Low=20, Medium=50, High=60
```

#### Questions to Answer

1. How long was High blocked? Compare to Low's critical section time.
2. What would happen if Medium ran indefinitely?
3. Can you predict High's worst-case blocking time? Why not?
4. What are the implications for meeting deadlines?
5. Why doesn't the scheduler prioritize High over Medium?

#### Real-World Impact

**Mars Pathfinder (1997):**
- Information bus task (high priority) blocked by meteorological task (low)
- Communication task (medium) caused priority inversion
- System watchdog reset after long blocking
- **Fixed by enabling priority inheritance in VxWorks**

---

### 6.4 Exercise 5: Priority Inheritance Protocol (PIP)

**File:** `exercises/exercise5.c`
**Time:** ~45 minutes

#### Concept

Solve priority inversion using Priority Inheritance Protocol (PIP) with PI mutexes.

**Same scenario as Exercise 4, but with PI mutex:**
- Low, Medium, High tasks with same priorities
- Same work durations
- **Different mutex type**

#### Run the Exercise

```bash
make exercise5
./exercise5
```

#### Expected Output

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
✓ High's blocking time was bounded by Low's critical section
```

#### What's Happening?

**Timeline with PI:**
```
 0ms: Low acquires PI mutex, starts 50ms work
30ms: High tries mutex → BLOCKS on Low
     Kernel: "High is blocking on Low's mutex"
     Kernel: "Boost Low's priority to 90 (inherit from High)"
55ms: Medium becomes runnable
     Scheduler: "Medium has priority 50, Low has priority 90"
     Decision: Low keeps running! (90 > 50)
     Medium cannot preempt Low
60ms: Low finishes critical section
60ms: Low releases mutex
     Kernel: "No more waiters, return Low to priority 10"
60ms: High unblocks and runs
160ms: High completes
160ms: Medium finally runs

Total High blocking: 60ms ≈ Low's critical section ✓
```

#### Priority Inheritance Protocol Rules

**1. When H blocks on mutex held by L:**
```
Kernel automatically boosts L's priority to H's priority
L temporarily runs at priority = max(L's priority, H's priority)
```

**2. While L holds mutex:**
```
L cannot be preempted by tasks with priority < H
L runs as if it had H's priority
```

**3. When L releases mutex:**
```
L returns to original priority
H unblocks and acquires mutex
```

**4. Transitive inheritance:**
```
If H blocks on M, and M blocks on L:
  L inherits H's priority (through M)
```

#### Code Analysis

**PI Mutex Initialization:**
```c
pthread_mutex_t resource_mutex;
pthread_mutexattr_t mutex_attr;

// Initialize attributes
pthread_mutexattr_init(&mutex_attr);

// Set priority inheritance protocol
pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);

// Create mutex with PI
pthread_mutex_init(&resource_mutex, &mutex_attr);

pthread_mutexattr_destroy(&mutex_attr);
```

**That's it!** The rest of the code is identical to Exercise 4. The kernel handles priority changes automatically.

#### Comparison: Exercise 4 vs Exercise 5

| Aspect | Exercise 4 (Regular) | Exercise 5 (PI) |
|--------|---------------------|-----------------|
| Mutex type | Regular | Priority Inheritance |
| High blocking | ~140ms (unbounded) | ~60ms (bounded) |
| Medium interference | YES - preempts Low | NO - cannot preempt |
| Predictability | Unpredictable | Predictable |
| RT suitability | ✗ Dangerous | ✓ Safe |
| Code complexity | Simple | Simple (just attr change!) |
| Performance overhead | None | Minimal (kernel tracking) |

#### Experiments to Try

**1. Increase Low's critical section:**
```c
#define LOW_WORK_MS 200  // 200ms instead of 50ms
```
Expected: High's blocking increases, but still bounded (no Medium interference).

**2. Compare directly with Exercise 4:**
```bash
./exercise4 > ex4.log
./exercise5 > ex5.log
grep "Blocked for" ex4.log ex5.log
```

**3. Multiple high-priority tasks:**
```c
// Create 3 high-priority tasks blocking on same mutex
for (int i = 0; i < 3; i++) {
    pthread_create(&high_threads[i], NULL, high_priority_task, NULL);
}
```
Expected: All boost Low; all experience bounded blocking.

**4. Measure priority changes:**
```c
// Add to Low task:
while (holding_mutex) {
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    printf("LOW: Current priority = %d\n", param.sched_priority);
    usleep(10000);
}
```

#### Questions to Answer

1. How does PI guarantee bounded blocking?
2. What happens if Low holds multiple PI mutexes simultaneously?
3. Can you calculate the maximum blocking time for High?
4. When would PI NOT be sufficient?
5. What's the overhead of priority inheritance?

---

### 6.5 Advanced Topics

#### Transitive Priority Inheritance

**Scenario:** Chain of blocking
```
High (P=90) blocks on mutex1 held by Medium (P=50)
Medium (P=50) blocks on mutex2 held by Low (P=10)

PI Solution:
  Low inherits Medium's priority → 50
  Medium inherits High's priority → 90
  Transitive: Low inherits High's priority → 90

Result: Low runs at P=90, completing both critical sections quickly
```

**Code Example:**
```c
// Low holds mutex2
pthread_mutex_lock(&mutex2);

// Medium holds mutex1, tries mutex2
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);  // Blocks on Low

// High tries mutex1
pthread_mutex_lock(&mutex1);  // Blocks on Medium

// Result: Low gets priority 90 (transitively from High)
```

#### Priority Ceiling Protocol (PCP)

**Alternative to PI:**

**Concept:** Each mutex has a **ceiling priority** (highest priority of any task that uses it).

**Rule:** Task acquiring mutex runs at ceiling priority immediately.

**Advantages:**
- Prevents priority inversion entirely
- Bounded blocking guaranteed
- No priority changes during execution

**Disadvantages:**
- May cause unnecessary priority elevation
- Requires static analysis of all tasks
- More complex to configure

**Comparison:**

| Protocol | When priority changes | Blocking |
|----------|----------------------|----------|
| PI (Exercise 5) | When higher-priority task blocks | Bounded |
| PCP | Immediately on lock | Bounded + prevents inversion |

**Linux support:**
```c
pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_PROTECT);
pthread_mutexattr_setprioceiling(&attr, 90);  // Ceiling priority
```

#### Nested Mutexes and Deadlock

**PI doesn't prevent deadlock!**

```c
// Thread A
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);  // Opposite order!

// Thread B
pthread_mutex_lock(&mutex2);
pthread_mutex_lock(&mutex1);  // Risk of deadlock

// PI will inherit priorities, but can't prevent circular wait
```

**Solutions:**
- Lock ordering (Week 3)
- Timeout with trylock
- Avoid nested locks

---

### 6.6 Real-World Case Study: Mars Pathfinder

#### The Problem (July 1997)

**System:**
- Mars Pathfinder lander
- VxWorks real-time operating system
- Multiple tasks with different priorities

**Tasks:**
- **bc_dist** (high priority): Bus communication, time-critical
- **ASI/MET** (low priority): Meteorological data collection
- **bc_sched** (medium priority): Bus scheduling

**Shared Resource:**
- Information bus mutex (used by bc_dist and ASI/MET)

#### What Happened

```
1. ASI/MET (low) acquires bus mutex
2. bc_dist (high) tries to acquire mutex → blocks
3. bc_sched (medium) becomes runnable → preempts ASI/MET
4. bc_dist blocked for duration of bc_sched execution
5. System watchdog timeout → reset!
```

**Symptom:** Intermittent system resets during communication.

#### The Fix

**Solution:** Enable priority inheritance for VxWorks mutexes.

```c
// Before (default):
semMCreate(SEM_Q_PRIORITY);

// After (with PI):
semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
```

**Result:**
- ASI/MET inherited bc_dist's priority when holding mutex
- bc_sched could not preempt ASI/MET
- bc_dist's blocking time bounded by critical section
- System stable!

#### Lessons Learned

1. **Priority inversion can occur in production systems**
2. **Intermittent failures are hard to debug**
3. **Always use PI mutexes in RT systems**
4. **Test under realistic system load**
5. **Simple fix, huge impact**

**Reference:** [Mars Pathfinder Analysis](https://research.microsoft.com/en-us/um/people/mbj/Mars_Pathfinder/)

---

### 6.7 Week 2 Assignment and Analysis

#### Part 1: Comparative Analysis

**Required Measurements:**

1. **Run both exercises multiple times:**
   ```bash
   for i in {1..10}; do
       ./exercise4 >> ex4_results.log
       ./exercise5 >> ex5_results.log
   done
   ```

2. **Extract and compare blocking times:**
   ```bash
   grep "Blocked for" ex4_results.log | awk '{print $4}' > ex4_blocking.csv
   grep "Blocked for" ex5_results.log | awk '{print $4}' > ex5_blocking.csv
   ```

3. **Calculate statistics:**
   - Average blocking time (Ex 4 vs Ex 5)
   - Standard deviation
   - Min/Max blocking
   - Speedup factor: `Ex4_avg / Ex5_avg`

**Expected Results:**
```
Exercise 4 (Regular mutex):
  Average: 120-140 ms
  Std dev: 10-20 ms
  Range:   100-160 ms

Exercise 5 (PI mutex):
  Average: 55-65 ms
  Std dev: 2-5 ms
  Range:   50-70 ms

Speedup: ~2.2x faster with PI
```

#### Part 2: Experimentation

**Task 1: Vary Medium's Work Duration**

Modify `MEDIUM_WORK_MS` in both exercises:
- 50ms, 100ms, 200ms, 500ms, 1000ms

**Create table:**
```
Medium_Work_ms | Ex4_Blocking_ms | Ex5_Blocking_ms | Difference
50             | 80              | 55              | 25
100            | 130             | 58              | 72
200            | 230             | 60              | 170
500            | 530             | 62              | 468
1000           | 1030            | 65              | 965
```

**Graph:** Plot blocking time vs Medium work duration (both exercises on same axes).

**Analysis:** What does this tell you about bounded vs unbounded blocking?

**Task 2: Implement Three-Task Chain**

Create a transitive inheritance scenario:
```c
// Resources
pthread_mutex_t mutex1, mutex2, mutex3;  // All with PI

// Task priorities
Low:   P=10  (holds mutex3)
Med:   P=50  (holds mutex2, needs mutex3)
High:  P=90  (holds mutex1, needs mutex2)
VHigh: P=99  (needs mutex1)

// Expected: Low inherits priority 99 (transitively)
```

**Measure:**
- Low's effective priority while holding mutex3
- VHigh's total blocking time
- Verify transitive inheritance works

#### Part 3: Critical Questions

**Conceptual:**
1. Explain why PI solves priority inversion but PCP prevents it.
2. Why doesn't the Linux scheduler just always run the highest-priority task?
3. When would you choose regular mutex over PI mutex?

**Analytical:**
1. Given: Low's critical section = 10ms, Medium's work = 100ms
   - Calculate High's blocking with regular mutex
   - Calculate High's blocking with PI mutex
   - What's the maximum tolerable critical section for 50ms deadline?

2. Design a system with 5 tasks and 3 shared resources:
   - Assign priorities using Rate Monotonic
   - Identify potential priority inversions
   - Calculate worst-case blocking for each task

**Practical:**
1. How would you detect priority inversion in a running system?
2. What tools would you use to debug intermittent resets like Mars Pathfinder?
3. How do you measure the overhead of priority inheritance?

---

**Next:** [Week 3: Deadlock Detection and Prevention](#7-week-3-deadlock-detection-and-prevention)

---

# Part V: Deadlock (Week 3)

## 7. Week 3: Deadlock Detection and Prevention

### 7.1 What is Deadlock?

Week 3 focuses on **deadlock in real-time systems**, understanding how circular wait conditions occur and how to prevent them through proper resource management.

**Learning Objectives:**
- Understand deadlock and the four Coffman conditions
- Recognize circular wait in multi-threaded systems
- Apply deadlock detection using timeouts
- Implement deadlock prevention through lock ordering
- Compare detection vs prevention strategies

**Deadlock Definition:** A situation where two or more tasks are permanently blocked, each waiting for resources held by the others, creating a circular wait condition.

**Simple Example:**
```
Task A holds Resource 1, wants Resource 2
Task B holds Resource 2, wants Resource 1
→ Both wait forever (deadlock!)
```

---

### 7.2 The Four Coffman Conditions

Deadlock occurs when **ALL FOUR** conditions are present simultaneously:

#### 1. Mutual Exclusion
**Resources cannot be shared** (e.g., mutex locks)
```c
pthread_mutex_t mutex;  // Only one thread can hold at a time
```

#### 2. Hold and Wait
**Tasks hold resources while waiting for others**
```c
pthread_mutex_lock(&mutex1);  // Hold mutex1
pthread_mutex_lock(&mutex2);  // Wait for mutex2 while holding mutex1
```

#### 3. No Preemption
**Resources cannot be forcibly taken away**
```c
// Cannot force a thread to release a mutex
// Must wait for pthread_mutex_unlock()
```

#### 4. Circular Wait
**Task A waits for B, B waits for C, ..., C waits for A**
```
A → needs resource held by B
B → needs resource held by C
C → needs resource held by A
(circular dependency!)
```

**Breaking ANY ONE condition prevents deadlock!**

---

### 7.3 Exercise 6: Deadlock Demonstration and Detection

**File:** `exercises/exercise6.c`
**Time:** ~45 minutes

#### Concept

Demonstrate how opposite lock ordering creates deadlock and use timeouts for detection.

**Scenario:**
- **Task A:** locks mutex1, then tries mutex2
- **Task B:** locks mutex2, then tries mutex1 (opposite order!)
- **Result:** Circular wait → deadlock

#### Run the Exercise

```bash
make exercise6
./exercise6
```

#### Expected Output

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

Recovery: Tasks detected timeout, released locks, and retried
```

#### What's Happening?

**Timeline:**
```
Time  Task A               Task B
----  ------               ------
 0ms  lock(mutex1) ✓
60ms                       lock(mutex2) ✓
110ms wait for mutex2 →
160ms                      wait for mutex1 →
      [DEADLOCK: Both waiting for each other]
1110ms TIMEOUT!
1110ms unlock(mutex1)
1160ms                     TIMEOUT!
1160ms                     unlock(mutex2)
      [Retry phase]
2210ms lock(mutex1) ✓
2310ms lock(mutex2) ✓     [B waiting]
2360ms unlock both
3260ms                     lock(mutex2) ✓
3360ms                     lock(mutex1) ✓
3410ms                     unlock both
```

#### Code Analysis

**Opposite Lock Ordering:**
```c
// Task A: mutex1 → mutex2
void* task_a(void* arg) {
    pthread_mutex_lock(&mutex1);
    printf("TASK-A: Acquired mutex1\n");

    usleep(100000);  // Delay to let Task B start

    // Try mutex2 with timeout
    struct timespec timeout;
    calc_timeout(&timeout, 1000);  // 1 second timeout

    int result = pthread_mutex_timedlock(&mutex2, &timeout);
    if (result == ETIMEDOUT) {
        printf("TASK-A: TIMEOUT! Deadlock detected\n");
        pthread_mutex_unlock(&mutex1);
        // Retry logic...
    }
}

// Task B: mutex2 → mutex1 (OPPOSITE ORDER!)
void* task_b(void* arg) {
    pthread_mutex_lock(&mutex2);
    printf("TASK-B: Acquired mutex2\n");

    usleep(100000);

    // Try mutex1 with timeout
    struct timespec timeout;
    calc_timeout(&timeout, 1000);

    int result = pthread_mutex_timedlock(&mutex1, &timeout);
    if (result == ETIMEDOUT) {
        printf("TASK-B: TIMEOUT! Deadlock detected\n");
        pthread_mutex_unlock(&mutex2);
        // Retry logic...
    }
}
```

**Timeout Mechanism:**
```c
void calc_timeout(struct timespec *ts, long ms) {
    clock_gettime(CLOCK_REALTIME, ts);
    ts->tv_sec += ms / 1000;
    ts->tv_nsec += (ms % 1000) * 1000000;

    // Normalize
    if (ts->tv_nsec >= 1000000000) {
        ts->tv_sec++;
        ts->tv_nsec -= 1000000000;
    }
}
```

#### Problems with Timeout Detection

**Disadvantages:**
1. ✗ **False positives** - Slow system ≠ deadlocked system
2. ✗ **Difficult to choose timeout** - Too short = false alarms; too long = slow detection
3. ✗ **Overhead** - Timeout management adds complexity
4. ✗ **Complex recovery** - What to do after detection?
5. ✗ **Not guaranteed** - May not work in all scenarios
6. ✗ **Wastes time** - Waiting for timeout before recovery

**When to use:**
- Last resort when prevention not possible
- Non-critical systems where retry is acceptable
- Debugging and testing

#### Experiments to Try

**1. Change timeout value:**
```c
#define DEADLOCK_TIMEOUT_MS 100  // Shorter timeout
```
Expected: Faster detection but more false positives

**2. Remove timeout (deadlock forever):**
```c
pthread_mutex_lock(&mutex2);  // Instead of timedlock
```
Expected: Tasks hang indefinitely

**3. Add more tasks:**
```c
// Create Task C that also needs both mutexes
pthread_create(&task_c, NULL, task_c_func, NULL);
```
Expected: Increases deadlock frequency

**4. Vary delays:**
```c
usleep(50000);  // Different delay between locks
```
Expected: Affects deadlock probability

#### Questions to Answer

1. How often does deadlock occur? (Run multiple times)
2. What's the minimum timeout that detects deadlock reliably?
3. How many retries are needed before success?
4. What happens if timeout is too short?
5. Why can't we just always use timeouts to solve deadlock?

---

### 7.4 Exercise 7: Deadlock Prevention with Lock Ordering

**File:** `exercises/exercise7.c`
**Time:** ~45 minutes

#### Concept

Prevent deadlock by enforcing a global lock ordering discipline.

**Key Idea:** If all tasks lock mutexes in the same order, circular wait cannot occur!

**Scenario (same as Ex 6, but fixed):**
- **Task A:** locks mutex1, then mutex2 (order: 1→2)
- **Task B:** locks mutex1, then mutex2 (order: 1→2, SAME!)
- **Result:** No circular wait → no deadlock

#### Run the Exercise

```bash
make exercise7
./exercise7
```

#### Expected Output

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
  • Both tasks acquire locks in SAME order (1 → 2)
  • Task that gets mutex1 first will complete fully
  • Other task waits, then proceeds when locks free
  • No circular wait condition possible
```

#### What's Happening?

**Timeline with Lock Ordering:**
```
Time  Task A               Task B
----  ------               ------
 0ms  lock(mutex1) ✓
60ms                       try lock(mutex1) → WAIT (A has it)
110ms lock(mutex2) ✓
110ms work...
160ms unlock(mutex2)
160ms unlock(mutex1)
160ms DONE
160ms                      lock(mutex1) ✓ (now available)
260ms                      lock(mutex2) ✓
310ms                      unlock both
310ms                      DONE

No deadlock - linear wait, not circular!
```

#### Lock Ordering Discipline

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

**Why it works:** Breaking circular wait condition!
```
For circular wait: A→B→C→A
With ordering:     A→B→C (linear, no cycle back to A)
```

#### Code Analysis

**Task A with Ordering:**
```c
void* task_a(void* arg) {
    // Lock in order: 1 → 2
    pthread_mutex_lock(&mutex1);   // Order 1
    printf("TASK-A: Acquired mutex1\n");

    usleep(100000);

    pthread_mutex_lock(&mutex2);   // Order 2
    printf("TASK-A: Acquired mutex2\n");

    // Critical section
    do_work();

    // Unlock in reverse order (best practice)
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}
```

**Task B with Ordering:**
```c
void* task_b(void* arg) {
    // Lock in SAME order: 1 → 2
    pthread_mutex_lock(&mutex1);   // Order 1
    printf("TASK-B: Acquired mutex1\n");

    usleep(100000);

    pthread_mutex_lock(&mutex2);   // Order 2
    printf("TASK-B: Acquired mutex2\n");

    // Critical section
    do_work();

    // Unlock in reverse order
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}
```

**Key difference from Exercise 6:** Both tasks lock in the same order!

**No timeouts needed** - deadlock is impossible by design.

#### Comparison: Detection vs Prevention

| Aspect | Exercise 6 (Detection) | Exercise 7 (Prevention) |
|--------|------------------------|-------------------------|
| Lock order | Opposite (A≠B) | Same (A=B) |
| Deadlock | YES | NO |
| Timeout needed | YES | NO |
| Retries | Multiple | None |
| Complexity | High (recovery logic) | Low (simple ordering) |
| Execution time | Unpredictable (retries) | Predictable |
| Overhead | High (timeouts, retries) | Minimal |
| Reliability | Fair (false positives) | Excellent |
| RT suitability | ✗ Unbounded delays | ✓ Bounded waiting |

**Conclusion: Prevention >> Detection**

#### Experiments to Try

**1. Violate lock ordering:**
```c
// In Task B, reverse order (create deadlock):
pthread_mutex_lock(&mutex2);  // Wrong order!
pthread_mutex_lock(&mutex1);
```
Expected: Deadlock will occur (like Exercise 6)

**2. Add third mutex:**
```c
pthread_mutex_t mutex3;
// Order: 1 → 2 → 3
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);
pthread_mutex_lock(&mutex3);
```
Expected: Still no deadlock with correct ordering

**3. Measure timing:**
```bash
time ./exercise6  # With detection (includes retries)
time ./exercise7  # With prevention (no retries)
```
Expected: Exercise 7 is faster and more consistent

**4. Stress test:**
```c
// Create 10 tasks all needing same locks
for (int i = 0; i < 10; i++) {
    pthread_create(&threads[i], NULL, worker, NULL);
}
```
Expected: With ordering, no deadlock regardless of task count

#### Questions to Answer

1. Why is Task B's execution time longer?
2. Could deadlock occur with same ordering? Why not?
3. How would you handle dynamic lock acquisition (don't know all locks upfront)?
4. What if lock order can't be predetermined?
5. How do you enforce ordering in a large codebase?

---

### 7.5 Advanced Topics

#### Resource Allocation Graph

**Visual tool for deadlock analysis:**

**Nodes:**
- Circles = Tasks
- Squares = Resources (mutexes)

**Edges:**
- Task → Resource = Request
- Resource → Task = Allocation (holds)

**Deadlock Detection:** Cycle in graph = Deadlock!

**Example from Exercise 6:**
```
Task A → [mutex1] → Task A  (A holds mutex1)
       ↓
     [mutex2]  (A wants mutex2)
       ↑
Task B → [mutex2] → Task B  (B holds mutex2)
       ↓
     [mutex1]  (B wants mutex1)
       ↑
       └─────────────┘  ← Cycle! Deadlock!
```

**Example from Exercise 7 (no deadlock):**
```
Task A → [mutex1] → Task A
Task A → [mutex2] (wants, will get after A)
Task B → [mutex1] (waits for A)
Task B → [mutex2] (will get after B gets mutex1)

No cycle - linear dependency chain
```

#### Hierarchical Locking

**Assign locks to hierarchy levels:**
```
Level 3: Application locks
Level 2: Subsystem locks
Level 1: Low-level locks
```

**Rule:** Lock higher levels before lower levels (or assign global order across levels)

**Example:**
```c
// Database system
pthread_mutex_t table_lock;      // Level 3
pthread_mutex_t index_lock;      // Level 2
pthread_mutex_t buffer_lock;     // Level 1

// Always: 3 → 2 → 1
lock(table_lock);
lock(index_lock);
lock(buffer_lock);
// work
unlock(buffer_lock);
unlock(index_lock);
unlock(table_lock);
```

**Prevents:** Circular dependencies across subsystems

#### Try-Lock Alternative

**Non-blocking lock attempt:**
```c
int result = pthread_mutex_trylock(&mutex);
if (result == 0) {
    // Got the lock
} else if (result == EBUSY) {
    // Lock held by another thread, don't wait
    // Can release held locks and retry
}
```

**Pattern for avoiding deadlock:**
```c
// Try-lock with backoff
while (!done) {
    pthread_mutex_lock(&mutex1);

    if (pthread_mutex_trylock(&mutex2) == 0) {
        // Got both locks!
        do_work();
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
        done = true;
    } else {
        // Couldn't get mutex2, release mutex1 and retry
        pthread_mutex_unlock(&mutex1);
        usleep(rand() % 1000);  // Random backoff
    }
}
```

**Advantages:**
- No blocking
- Can implement custom retry logic

**Disadvantages:**
- Busy-waiting (CPU usage)
- Livelock risk (both keep retrying)
- Complex logic

---

### 7.6 Best Practices and Checklist

#### Design Guidelines

**1. Lock Ordering (Primary Defense)**
- ✓ Document all locks with order numbers
- ✓ Use naming conventions: `lock_L1_xxx`, `lock_L2_yyy`
- ✓ Validate order at runtime with assertions
- ✓ Code review for ordering violations
- ✓ Use static analysis tools

**2. Minimize Lock Scope**
```c
// ✓ Good: Short critical section
pthread_mutex_lock(&mutex);
counter++;
pthread_mutex_unlock(&mutex);

// ✗ Bad: Long critical section
pthread_mutex_lock(&mutex);
do_complex_work();          // Holds lock too long
counter++;
pthread_mutex_unlock(&mutex);
```

**3. Avoid Nested Locks**
```c
// ✓ Best: Single lock when possible
pthread_mutex_lock(&combined_mutex);
update_resource_a();
update_resource_b();
pthread_mutex_unlock(&combined_mutex);

// ⚠ Risky: Nested locks (needs ordering!)
pthread_mutex_lock(&mutex1);
pthread_mutex_lock(&mutex2);
// Must ensure correct order!
pthread_mutex_unlock(&mutex2);
pthread_mutex_unlock(&mutex1);
```

**4. Unlock in Reverse Order**
```c
lock(mutex1);
lock(mutex2);
lock(mutex3);
// work
unlock(mutex3);  // Reverse order
unlock(mutex2);
unlock(mutex1);
```

**5. Use Timeout as Last Resort**
```c
// Only when ordering not possible
struct timespec timeout;
calc_timeout(&timeout, 1000);
if (pthread_mutex_timedlock(&mutex, &timeout) == ETIMEDOUT) {
    // Recovery logic
}
```

#### Deadlock Prevention Checklist

**Before writing code:**
- [ ] Identified all shared resources?
- [ ] Assigned global lock ordering?
- [ ] Documented lock order?
- [ ] Considered lock-free alternatives?

**During implementation:**
- [ ] Following lock order consistently?
- [ ] Minimizing critical sections?
- [ ] Unlocking in reverse order?
- [ ] Handling errors properly?

**During review:**
- [ ] Verified lock ordering?
- [ ] Checked for nested locks?
- [ ] Looked for circular dependencies?
- [ ] Tested with multiple threads?

**During testing:**
- [ ] Stress tested with many threads?
- [ ] Tested under load?
- [ ] Used deadlock detection tools?
- [ ] Verified with ThreadSanitizer?

#### Tools for Detection

**Runtime Tools:**
- **Helgrind** (Valgrind): `valgrind --tool=helgrind ./program`
- **ThreadSanitizer**: `gcc -fsanitize=thread ...`
- **gdb**: Can show thread states and lock ownership

**Static Analysis:**
- **Clang Static Analyzer**
- **Coverity**
- Custom lint rules

---

### 7.7 Week 3 Assignment

#### Part 1: Analysis

**Required:**
1. Run Exercise 6 multiple times (10+ runs)
   - Calculate deadlock frequency
   - Measure average time to detection
   - Count retries before success

2. Compare execution times:
   - Exercise 6 (detection): Measure total time including retries
   - Exercise 7 (prevention): Measure total time
   - Calculate speedup factor

3. Create visualization:
   - Graph: Retry attempts vs timeout value
   - Graph: Execution time distribution (Ex6 vs Ex7)

#### Part 2: Implementation

**Task 1: Three-Mutex Scenario**

Implement system with 3 mutexes and 3 tasks:
```c
pthread_mutex_t mutex1, mutex2, mutex3;

// With lock ordering (1→2→3):
Task A: needs mutex1, mutex2
Task B: needs mutex2, mutex3
Task C: needs mutex1, mutex3

// Question: Can deadlock occur? Prove it.
```

**Task 2: Dynamic Lock Ordering**

Implement a system where locks are acquired based on runtime data:
```c
void transfer(Account* from, Account* to, int amount) {
    // Problem: Don't know which account lock to acquire first
    // Solution: Order by account ID

    Account* first = (from->id < to->id) ? from : to;
    Account* second = (from->id < to->id) ? to : from;

    pthread_mutex_lock(&first->mutex);
    pthread_mutex_lock(&second->mutex);
    // Transfer money
    pthread_mutex_unlock(&second->mutex);
    pthread_mutex_unlock(&first->mutex);
}
```

Test with multiple threads doing transfers.

**Task 3: Deadlock Detector**

Write a tool that monitors lock acquisition:
```c
// Log every lock/unlock
void my_lock(pthread_mutex_t* m, const char* name, int order);
void my_unlock(pthread_mutex_t* m, const char* name);

// Detect violations:
// - Locking out of order
// - Potential circular wait
// - Lock held too long
```

#### Part 3: Critical Questions

**Conceptual:**
1. Why is prevention better than detection for RT systems?
2. Explain why lock ordering breaks the circular wait condition.
3. When would you choose timeout detection over prevention?

**Analytical:**
1. Given 5 tasks and 4 resources, draw resource allocation graph showing:
   - Deadlock scenario
   - Same scenario with lock ordering (no deadlock)

2. Design lock ordering for:
   - Bank system (accounts, transactions, audit log)
   - Robot controller (sensors, motors, safety, logging)

**Practical:**
1. How do you debug deadlock in production system?
2. What metrics would you collect to detect potential deadlock?
3. How would you add lock ordering to existing codebase?

---

**Next:** [Week 4: Lock-Free Synchronization](#8-week-4-lock-free-synchronization)

---

## 8. Week 4: Lock-Free Synchronization

Week 4 introduces **lock-free programming** - an advanced synchronization approach that avoids traditional mutexes and their associated problems (priority inversion, deadlock, blocking). This chapter explores atomic operations, lock-free data structures, and performance trade-offs.

**Learning Objectives:**
- Understand progress guarantees: blocking vs lock-free vs wait-free
- Master atomic operations and Compare-And-Swap (CAS)
- Implement lock-free data structures (stack)
- Recognize and solve the ABA problem
- Analyze performance: locks vs lock-free approaches
- Understand memory ordering and barriers

**Prerequisites:**
- Completed Weeks 0-3
- Strong understanding of concurrency and synchronization
- Familiarity with priority inversion and deadlock

---

### 8.1 What is Lock-Free Programming?

Lock-free programming represents a paradigm shift from traditional mutex-based synchronization to non-blocking algorithms based on atomic hardware instructions.

#### Progress Guarantees

**Blocking (Traditional Approach):**
- Threads can block indefinitely waiting for locks
- Can cause priority inversion
- Can cause deadlock
- Simpler to implement and reason about

**Lock-Free:**
- **Definition:** At least one thread makes progress in a finite number of steps, even if others are delayed
- No locks/mutexes required
- Based on atomic hardware instructions
- Thread delays don't block other threads
- **Not** the same as "lockless" (without locks)

**Wait-Free:**
- **Definition:** Every thread makes progress in a finite number of steps
- Stronger guarantee than lock-free
- Harder to implement
- Best theoretical guarantee

#### Progress Guarantees Hierarchy

```
Wait-Free (strongest guarantee)
    ↓
Lock-Free (system-wide progress)
    ↓
Obstruction-Free (progress when alone)
    ↓
Blocking (weakest guarantee)
```

**Example:**
- **Blocking:** Thread A holds lock, Thread B waits indefinitely
- **Lock-Free:** Thread A's CAS fails, Thread B succeeds (system progresses)
- **Wait-Free:** Both Thread A and B make progress within bounded steps

#### Why Lock-Free?

**Advantages:**
✓ **No priority inversion** - High-priority threads never blocked by low-priority ones
✓ **No deadlock possible** - No locks means no circular wait
✓ **Better scalability** - No lock contention bottlenecks
✓ **Thread failure tolerance** - One thread's failure doesn't block system
✓ **Often better performance** - Under high contention scenarios

**Disadvantages:**
✗ **Complex to implement correctly** - Subtle race conditions and edge cases
✗ **ABA problem** - Pointer reuse can cause corruption
✗ **Memory ordering challenges** - Requires understanding of memory models
✗ **May waste CPU on retries** - Failed CAS attempts consume cycles
✗ **Harder to reason about** - Non-intuitive concurrent behavior

#### When to Use Lock-Free?

**Good Candidates:**
- High contention scenarios (many threads accessing same data)
- RT systems where blocking is unacceptable
- Systems where priority inversion is problematic
- Scalable multi-core performance requirements
- Data structures with simple operations (stack, queue, counter)

**Poor Candidates:**
- Complex data structures (trees, graphs, hash tables)
- When simplicity and maintainability are paramount
- Low contention scenarios (locks perform well)
- When lock performance is already adequate
- When development time is limited

**Rule of Thumb:** Start with locks and PI mutexes. Only move to lock-free if profiling shows it's necessary and beneficial.

---

### 8.2 Atomic Operations and Compare-And-Swap

Lock-free algorithms are built on atomic operations provided by hardware and exposed through compiler intrinsics.

#### Compare-And-Swap (CAS) - The Fundamental Primitive

**Conceptual Definition:**
```c
bool CAS(T* ptr, T expected, T new_value) {
    atomic {  // Executed atomically by hardware
        if (*ptr == expected) {
            *ptr = new_value;
            return true;  // Success - value was expected
        }
        return false;  // Failed - value changed
    }
}
```

**Key Properties:**
1. **Atomic** - Cannot be interrupted mid-execution
2. **Conditional** - Only updates if current value matches expected
3. **Returns status** - Indicates success or failure
4. **Non-blocking** - Never waits for locks

**GCC/Clang Builtin:**
```c
bool __atomic_compare_exchange_n(
    T* ptr,               // Pointer to shared variable
    T* expected,          // Pointer to expected value (updated on failure!)
    T desired,            // Desired new value
    bool weak,            // Allow spurious failures (false = strong)
    int success_memorder, // Memory order on success
    int failure_memorder  // Memory order on failure
);
```

**Example Usage:**
```c
int shared_var = 10;
int old_value = 10;
int new_value = 20;

if (__atomic_compare_exchange_n(&shared_var, &old_value, new_value,
                                  false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
    printf("Success: shared_var was 10, now 20\n");
} else {
    printf("Failure: shared_var wasn't 10\n");
    printf("Actual value: %d\n", old_value);  // old_value updated!
}
```

**Important:** On failure, `expected` is updated with the actual current value. This allows retry loops without additional reads.

#### Other Atomic Operations

**Atomic Load:**
```c
// Read value atomically
int value = __atomic_load_n(&var, __ATOMIC_SEQ_CST);
```

**Atomic Store:**
```c
// Write value atomically
__atomic_store_n(&var, new_value, __ATOMIC_SEQ_CST);
```

**Atomic Fetch-and-Add:**
```c
// Atomically: old = var; var += delta; return old;
int old = __atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST);
```

**Atomic Exchange:**
```c
// Atomically: old = var; var = new_value; return old;
int old = __atomic_exchange_n(&var, new_value, __ATOMIC_SEQ_CST);
```

**Atomic Fetch-and-Or/And/Xor:**
```c
int old = __atomic_fetch_or(&flags, FLAG_BIT, __ATOMIC_SEQ_CST);
int old = __atomic_fetch_and(&flags, ~FLAG_BIT, __ATOMIC_SEQ_CST);
```

#### Memory Ordering

Memory ordering controls how atomic operations synchronize with other operations across threads.

**Memory Order Modes:**

| Mode | Description | Use Case | Performance |
|------|-------------|----------|-------------|
| `__ATOMIC_SEQ_CST` | Sequential consistency | Default, safest | Slowest |
| `__ATOMIC_ACQUIRE` | Acquire semantics | Loads, lock acquisition | Medium |
| `__ATOMIC_RELEASE` | Release semantics | Stores, lock release | Medium |
| `__ATOMIC_ACQ_REL` | Both acquire and release | Read-modify-write | Medium |
| `__ATOMIC_RELAXED` | No ordering constraints | Counters, flags | Fastest |

**Sequential Consistency (`__ATOMIC_SEQ_CST`):**
- All threads see operations in same order
- Simplest mental model
- Highest overhead
- **Recommended starting point**

**Acquire-Release:**
- Acquire: Prevents subsequent operations from moving before
- Release: Prevents prior operations from moving after
- Common pattern: release on write, acquire on read

**Relaxed:**
- No ordering guarantees
- Only atomicity guaranteed
- Use for simple counters where order doesn't matter

**For RT Systems:**
Use `__ATOMIC_SEQ_CST` initially for correctness. Optimize to weaker orderings only after profiling shows it's necessary and you've verified correctness.

**Example:**
```c
// Producer (release)
data = compute_value();
__atomic_store_n(&ready, 1, __ATOMIC_RELEASE);

// Consumer (acquire)
while (!__atomic_load_n(&ready, __ATOMIC_ACQUIRE)) {
    // Wait
}
use_data(data);  // Guaranteed to see updated data
```

---

### 8.3 Exercise 8: Lock-Free Stack Implementation

Exercise 8 implements a lock-free stack using CAS operations and compares it with a mutex-based baseline.

#### Concept

A **stack** (LIFO - Last In, First Out) is an ideal first lock-free data structure because:
- Simple operations (push, pop)
- Single pointer to update (head)
- Clear correctness criteria
- Easy to test

**Comparison:**
- **Mutex-based stack:** Traditional implementation with `pthread_mutex`
- **Lock-free stack:** CAS-based implementation with no locks

#### Running the Exercise

```bash
# Build
make exercise8

# Run
./exercise8

# With ThreadSanitizer (detect data races)
make CFLAGS="-fsanitize=thread" exercise8
./exercise8
```

#### Expected Output

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

#### Lock-Free Stack Structure

```c
// Node in the stack
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Stack with atomic head pointer
typedef struct {
    Node* head;  // Atomically updated
} LockFreeStack;

// Initialize
void init(LockFreeStack* stack) {
    __atomic_store_n(&stack->head, NULL, __ATOMIC_SEQ_CST);
}
```

#### Push Operation

**Algorithm:**
1. Create new node with value
2. Read current head atomically
3. Set new node's next to point to current head
4. Try to CAS head from old value to new node
5. If CAS fails (head changed), retry from step 2
6. If CAS succeeds, push complete

**Implementation:**
```c
void push(LockFreeStack* stack, int value) {
    // Allocate new node
    Node* new_node = malloc(sizeof(Node));
    new_node->data = value;

    Node* old_head;
    do {
        // Read current head
        old_head = __atomic_load_n(&stack->head, __ATOMIC_SEQ_CST);

        // Point new node to current head
        new_node->next = old_head;

        // Try to update head to new node
    } while (!__atomic_compare_exchange_n(
        &stack->head,
        &old_head,     // Expected value (updated on failure)
        new_node,      // New value
        false,         // Strong CAS (no spurious failures)
        __ATOMIC_SEQ_CST,
        __ATOMIC_SEQ_CST
    ));
    // Loop until CAS succeeds
}
```

**Visual Example:**

```
Initial state: head → A → B → C → NULL

Thread wants to push X:

Step 1: Create X
        X (not yet linked)

Step 2: old_head = head (points to A)
        X.next = old_head (X → A)

Step 3: CAS(&head, old_head=A, new_node=X)
        Success! head now points to X

Result: head → X → A → B → C → NULL
```

**Concurrent Push Scenario:**

```
Initial: head → A

Thread 1:              Thread 2:
old_head = A           old_head = A
X.next = A             Y.next = A
CAS(A → X) ✓           CAS(A → Y) ✗ (head is X now!)
head → X → A           Retry:
                       old_head = X (updated by CAS)
                       Y.next = X
                       CAS(X → Y) ✓
                       head → Y → X → A
```

Both pushes succeed, maintaining consistency!

#### Pop Operation

**Algorithm:**
1. Read current head atomically
2. If head is NULL, stack is empty (return false)
3. Read next node (new head after pop)
4. Try to CAS head from old head to next node
5. If CAS fails (head changed), retry from step 1
6. If CAS succeeds, extract value and free old head

**Implementation:**
```c
bool pop(LockFreeStack* stack, int* value) {
    Node* old_head;
    Node* new_head;

    do {
        // Read current head
        old_head = __atomic_load_n(&stack->head, __ATOMIC_SEQ_CST);

        if (old_head == NULL) {
            return false;  // Stack empty
        }

        // Next node becomes new head
        new_head = old_head->next;

        // Try to update head
    } while (!__atomic_compare_exchange_n(
        &stack->head,
        &old_head,
        new_head,
        false,
        __ATOMIC_SEQ_CST,
        __ATOMIC_SEQ_CST
    ));

    // Extract value and free node
    *value = old_head->data;
    free(old_head);  // WARNING: ABA problem! (see next section)
    return true;
}
```

**Visual Example:**

```
Initial: head → A → B → C → NULL

Pop operation:

Step 1: old_head = A
        new_head = A.next = B

Step 2: CAS(&head, old_head=A, new_head=B)
        Success!

Result: head → B → C → NULL
        A is freed
```

#### Correctness Testing

Exercise 8 includes comprehensive correctness tests:

**Test 1: Single-threaded**
- Push N items
- Pop N items
- Verify all values present, no duplicates

**Test 2: Multi-threaded push**
- 4 threads each push 1000 items
- Verify 4000 total items
- Verify no duplicates

**Test 3: Multi-threaded push and pop**
- 2 threads pushing, 2 threads popping
- Verify all pushed items eventually popped
- Verify no data loss or corruption

**Test 4: Stress test**
- Many threads, many operations
- Random mix of push/pop
- Long duration test

#### Performance Comparison

**Mutex-Based Stack:**
```c
typedef struct {
    Node* head;
    pthread_mutex_t lock;
} MutexStack;

void push(MutexStack* stack, int value) {
    pthread_mutex_lock(&stack->lock);    // BLOCKS here if contention
    // ... insert logic ...
    pthread_mutex_unlock(&stack->lock);
}
```

**Performance characteristics:**
- **Contention:** Threads block waiting for lock
- **Priority inversion:** Low-priority holder blocks high-priority waiter
- **Throughput:** Limited by lock hold time
- **Latency:** Variable (depends on lock wait)

**Lock-Free Stack:**
```c
void push(LockFreeStack* stack, int value) {
    do {
        // ... CAS logic ...
    } while (!success);  // RETRIES but never blocks
}
```

**Performance characteristics:**
- **Contention:** Threads retry CAS (no blocking)
- **Priority inversion:** Impossible (no locks)
- **Throughput:** Scales with cores
- **Latency:** More consistent (no blocking)

**Typical Results (4 cores, high contention):**
- Lock-free: 30-50% faster throughput
- Lock-free: 20-40% lower average latency
- Lock-free: Better worst-case latency (no blocking)
- Lock-free: Higher CAS retry count (wasted work)

#### Experiments to Try

**Experiment 1: Thread Count Scaling**
```c
#define NUM_THREADS 2   // Start here
// Try: 4, 8, 16
```
**Expected:** Lock-free advantage increases with more threads.

**Experiment 2: Operation Count**
```c
#define OPS_PER_THREAD 100000
// Try: 10000, 1000000
```
**Expected:** Longer runs show consistent lock-free advantage.

**Experiment 3: Work Simulation**
```c
// After each push/pop:
usleep(10);  // Simulate 10µs of work
```
**Expected:** With delays, lock advantage diminishes (less contention).

**Experiment 4: CAS Retry Measurement**
Add counter in CAS loop:
```c
int retries = 0;
do {
    retries++;
    // ... CAS logic ...
} while (!success);
printf("CAS retries: %d\n", retries);
```
**Expected:** Higher contention → more retries.

**Experiment 5: RT Priority Test**
```bash
# Give mutex version high priority
sudo chrt -f 80 ./exercise8
```
**Expected:** Priority inversion may occur with mutex version under contention.

#### Questions to Answer

1. **When does lock-free outperform mutex-based?**
   - High thread count?
   - High contention?
   - Mixed priorities?

2. **What is the CAS retry rate under various loads?**
   - Low contention: <5% retries?
   - High contention: >20% retries?

3. **How does performance scale with thread count?**
   - Linear scaling?
   - Where does it plateau?

4. **What are the memory ordering requirements?**
   - Can we use weaker orderings?
   - What breaks with `__ATOMIC_RELAXED`?

5. **How would you detect the ABA problem in practice?**
   - Validation checks?
   - Stress testing?
   - Formal verification?

---

### 8.4 The ABA Problem

The ABA problem is a subtle correctness issue that can corrupt lock-free data structures. Understanding and preventing it is crucial for correct implementations.

#### What is the ABA Problem?

**Definition:** A location is read as value A, changes to B, then back to A. A CAS operation sees A and incorrectly assumes nothing changed.

**Why it's a problem:** CAS checks the value, not whether the value *changed*. If a pointer returns to the same address but the pointed-to data is different, corruption occurs.

#### Detailed Scenario

```
Initial state: head → A → B → C → NULL

Thread 1:                        Thread 2:
────────────────────────────────────────────────────────────
1. old_head = head (A)
2. new_head = A->next (B)
3. [PREEMPTED]
                                 4. pop() → returns A
                                    head → B → C
                                 5. pop() → returns B
                                    head → C
                                 6. push(A) → reuses node A!
                                    head → A → C
                                    (A->next now points to C!)

7. CAS(&head, A, B)
   ✓ SUCCESS! head was A
   head → B
   BUT: B->next is undefined!
        (B was freed in step 5)

Result: CORRUPTION! Stack is broken.
```

**The problem:**
- Thread 1 sees head = A at step 1
- Thread 1 is preempted for "a long time"
- Thread 2 pops A, pops B, pushes A back (A is reused)
- Thread 1 resumes, CAS succeeds (head is still A)
- Thread 1 sets head = B, but B was freed!

**Result:** Dangling pointer, use-after-free, data corruption.

#### Why It's Called ABA

```
Value observed by Thread 1:
  Time 0: A (read by thread 1)
  Time 1: B (changed by thread 2)
  Time 2: A (changed back by thread 2)
  Time 3: A (CAS by thread 1 succeeds - thinks nothing changed!)
```

The value goes A → B → A, hence "ABA problem."

#### When Does It Occur?

**Requirements:**
1. **Memory reuse:** Freed nodes are reallocated
2. **Pointer reuse:** New node gets same address as freed node
3. **Preemption window:** Long enough for allocate → free → reallocate cycle
4. **CAS on pointers:** Checking pointer equality, not content

**Common triggers:**
- Fast allocator (reuses memory quickly)
- High thread count (more preemption)
- Small memory pool (addresses reused frequently)
- Long CAS retry loops

#### ABA Problem Solutions

**Solution 1: Tagged Pointers / Version Numbers**

**Idea:** Pack a counter with the pointer. CAS checks both pointer and counter.

**Implementation:**
```c
typedef struct {
    Node* ptr;
    uint64_t counter;
} TaggedPointer;

TaggedPointer head;

// Push
void push(LockFreeStack* stack, int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = value;

    TaggedPointer old_head, new_head;
    do {
        old_head.ptr = __atomic_load_n(&stack->head.ptr, __ATOMIC_SEQ_CST);
        old_head.counter = __atomic_load_n(&stack->head.counter, __ATOMIC_SEQ_CST);

        new_node->next = old_head.ptr;

        new_head.ptr = new_node;
        new_head.counter = old_head.counter + 1;  // Increment counter!

    } while (!__atomic_compare_exchange_n(
        &stack->head,  // CAS both ptr and counter
        &old_head,
        new_head,
        false,
        __ATOMIC_SEQ_CST,
        __ATOMIC_SEQ_CST
    ));
}
```

**Pros:**
- Simple to understand
- Works on platforms with DWCAS (double-width CAS)

**Cons:**
- Requires 128-bit CAS on 64-bit platforms (CMPXCHG16B on x86-64)
- Not portable (not all architectures support DWCAS)
- Counter can theoretically wrap (but 2^64 operations is huge)

**Solution 2: Hazard Pointers**

**Idea:** Threads announce which pointers they're using. Delay freeing until no thread uses them.

**Implementation Sketch:**
```c
// Global hazard pointer array
Node* hazard_pointers[MAX_THREADS];

bool pop(LockFreeStack* stack, int* value) {
    Node* old_head;
    int my_thread_id = get_thread_id();

    do {
        old_head = __atomic_load_n(&stack->head, __ATOMIC_SEQ_CST);
        if (old_head == NULL) return false;

        // Announce: "I'm using this pointer!"
        hazard_pointers[my_thread_id] = old_head;
        __atomic_thread_fence(__ATOMIC_SEQ_CST);

        // Verify head didn't change
        if (old_head != __atomic_load_n(&stack->head, __ATOMIC_SEQ_CST)) {
            continue;  // Retry
        }

        // Now old_head is protected from being freed
        // ... CAS logic ...

    } while (!success);

    // Clear hazard pointer
    hazard_pointers[my_thread_id] = NULL;

    *value = old_head->data;
    retire(old_head);  // Don't free immediately!
    return true;
}

void retire(Node* node) {
    // Add to retired list
    // Free only if no hazard pointer matches
}
```

**Pros:**
- Portable (works on all architectures)
- Correct and well-studied

**Cons:**
- Complex implementation
- Overhead of hazard pointer management
- Deferred deallocation (memory usage)

**Solution 3: Epoch-Based Reclamation**

**Idea:** Track epochs (generations). Only free nodes from old epochs when all threads have advanced.

**Pros:**
- Efficient (low overhead)
- Scalable

**Cons:**
- Complex to implement correctly
- Memory usage (deferred freeing)

**Solution 4: Garbage Collection**

**Idea:** Use a language with automatic memory management (Java, Go, etc.). No manual `free()` means no reuse.

**Pros:**
- Simple application code
- Correct by default

**Cons:**
- Requires GC runtime
- GC pauses (problematic for RT systems!)
- Not applicable to C/C++

#### Exercise 8 Approach

**Note:** Exercise 8 uses a simplified implementation for educational purposes:
- Basic CAS without ABA protection
- Demonstrates algorithm and performance
- **Not production-ready** without ABA solution

**For learning:**
- Focus on algorithm understanding
- Observe performance benefits
- Understand when ABA occurs

**For production:**
- Add hazard pointers or tagged pointers
- Extensive testing
- Consider using vetted libraries (Boost.Lockfree, Folly)

#### Detecting ABA in Practice

**Validation Checks:**
```c
// Add magic numbers
typedef struct Node {
    uint32_t magic;  // Set to 0xDEADBEEF when allocated
    int data;
    struct Node* next;
} Node;

// In pop:
assert(node->magic == 0xDEADBEEF);  // Detect use-after-free
```

**Stress Testing:**
- Many threads
- Small memory pool (forces reuse)
- Long duration
- Use AddressSanitizer (`-fsanitize=address`)

**ThreadSanitizer:**
```bash
gcc -fsanitize=thread -g exercise8.c -o exercise8
./exercise8
# Reports data races and potential issues
```

---

### 8.5 Exercise 9: Comprehensive Performance Analysis

Exercise 9 provides a systematic comparison of synchronization approaches (mutex with PI vs lock-free) under various workloads, contention levels, and priority scenarios.

#### Concept

Real-world systems have diverse characteristics:
- Variable contention (low to high)
- Mixed workloads (read-heavy vs write-heavy)
- Priority-sensitive tasks
- Deadline requirements

Exercise 9 tests both approaches across these dimensions to understand trade-offs.

#### Running the Exercise

```bash
# Build
make exercise9

# Run full test suite
./exercise9

# Run specific test
./exercise9 --test low-contention
./exercise9 --test high-contention
./exercise9 --test mixed-priority
./exercise9 --test deadline-sensitive

# With RT scheduling
sudo chrt -f 80 ./exercise9
```

#### Test Scenarios

**Test 1: Read-Heavy Workload (Low Contention)**
- 90% read operations, 10% write operations
- Simulates: caches, lookup tables, configuration data
- Expected: Lock-free wins moderately (20-30% faster)

**Test 2: Write-Heavy Workload (High Contention)**
- 50% read operations, 50% write operations
- Simulates: counters, statistics, frequently updated shared state
- Expected: Lock-free wins significantly (40-50% faster)

**Test 3: Mixed Priority Tasks**
- 1 high-priority reader (SCHED_FIFO, priority 90)
- 3 low-priority writers (SCHED_FIFO, priority 20)
- Expected: Lock-free eliminates priority inversion

**Test 4: Deadline Sensitivity**
- 4 periodic tasks with 10ms deadlines
- Expected: Lock-free meets all deadlines, mutex misses some

#### Metrics Collected

**Performance Metrics:**
- Throughput (ops/sec)
- Latency (average, min, max, percentiles)
- Execution time

**Synchronization Metrics:**
- CAS retry count/rate
- Lock contention events
- Priority inversion occurrences

**RT Characteristics:**
- Deadline miss count/rate
- Worst-case response time
- Jitter

---

### 8.6 Advanced Topics

**Lock-Free Queue (FIFO):**
More complex than stack - needs careful handling of both head and tail pointers.

**Memory Reclamation Strategies:**
- Hazard pointers (detailed implementation)
- Epoch-based reclamation
- Reference counting

**Double-Width CAS (DWCAS):**
CAS on two adjacent words atomically for tagged pointers.

**Memory Barriers and Fences:**
Compiler barriers vs hardware fences for ensuring visibility.

---

### 8.7 Common Pitfalls and Solutions

**Pitfall 1: ABA Problem**
- Symptom: Intermittent corruption under high load
- Solution: Tagged pointers or hazard pointers

**Pitfall 2: Memory Ordering Bugs**
- Symptom: Works on x86, fails on ARM/PowerPC
- Solution: Use proper memory ordering modes

**Pitfall 3: Livelock**
- Symptom: Threads keep retrying, making no progress
- Solution: Exponential backoff, fallback to locks

**Pitfall 4: Memory Leaks**
- Symptom: Memory usage grows over time
- Solution: Proper reclamation scheme

**Pitfall 5: False Sharing**
- Symptom: Poor scalability despite lock-free design
- Solution: Pad structures to cache line boundaries

---

### 8.8 Best Practices and Design Guidelines

**Design Process:**
1. Start with locks
2. Measure and identify bottlenecks
3. Consider lock-free only if necessary
4. Choose simplest lock-free structure
5. Implement with sequential consistency
6. Test extensively
7. Optimize memory ordering if needed

**Implementation Checklist:**
- [ ] Algorithm proven lock-free
- [ ] ABA protection implemented
- [ ] Memory ordering specified
- [ ] Tested on weak memory models
- [ ] ThreadSanitizer clean
- [ ] Stress tested

---

### 8.9 Week 4 Summary and Assignment

**Key Takeaways:**
✓ CAS is the basic primitive for lock-free algorithms
✓ Progress guarantees: wait-free > lock-free > blocking
✓ Lock-free eliminates priority inversion and deadlock
✓ Trade-off: complexity vs performance/guarantees

**Commands Reference:**
```bash
make week4
make run8              # Lock-free stack
make run9              # Performance comparison
make CFLAGS="-fsanitize=thread" exercise8
perf stat -e cache-misses ./exercise9
```

**Assignment Ideas:**

**Part 1: Implementation**

1. Lock-Free Queue
2. Add Hazard Pointers to Exercise 8
3. Exponential Backoff

**Part 2: Analysis**

1. CAS Retry Analysis
2. Memory Ordering Comparison
3. Cache Performance Analysis

**Part 3: Advanced**

1. Epoch-Based Memory Reclamation
2. Lock-Free Hash Table

**Critical Thinking Questions:**

1. When Lock-Free is Counterproductive
2. ABA Solutions Comparison
3. Memory Models and Real-Time Systems

---

*[Content for remaining sections will be added in subsequent steps]*

---

## Navigation

**[↑ Back to Table of Contents](#table-of-contents)**

---

*Document Version: 1.0*
*Last Updated: 2025-10-12*
*Course: SSATR/IAISC/ICAF 2025*
