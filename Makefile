# Makefile for Real-Time Systems Lab - Weeks 1, 2, 3 & 4

CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread -lrt
LDFLAGS = -pthread -lrt

# Source directories
EXERCISE_DIR = exercises
SCRIPT_DIR = scripts
RESULT_DIR = results

# Executables - Introductory exercises (Week 0)
EXERCISE0A = exercise0a_time_basics
EXERCISE0B = exercise0b_threading
EXERCISE0C = exercise0c_file_io
EXERCISE0D = exercise0d_busywait
EXERCISE0E = exercise0e_scheduling
EXERCISE0F = exercise0f_statistics

# Executables - Main exercises
EXERCISE1 = exercise1
EXERCISE2 = exercise2
EXERCISE3 = exercise3
EXERCISE4 = exercise4
EXERCISE5 = exercise5
EXERCISE6 = exercise6
EXERCISE7 = exercise7
EXERCISE8 = exercise8
EXERCISE8_ABA = exercise8_aba
EXERCISE9 = exercise9

# Source files - Introductory exercises
EXERCISE0A_SRC = $(EXERCISE_DIR)/exercise0a_time_basics.c
EXERCISE0B_SRC = $(EXERCISE_DIR)/exercise0b_threading.c
EXERCISE0C_SRC = $(EXERCISE_DIR)/exercise0c_file_io.c
EXERCISE0D_SRC = $(EXERCISE_DIR)/exercise0d_busywait.c
EXERCISE0E_SRC = $(EXERCISE_DIR)/exercise0e_scheduling.c
EXERCISE0F_SRC = $(EXERCISE_DIR)/exercise0f_statistics.c

# Source files - Main exercises
EXERCISE1_SRC = $(EXERCISE_DIR)/exercise1.c
EXERCISE2_SRC = $(EXERCISE_DIR)/exercise2.c
EXERCISE3_SRC = $(EXERCISE_DIR)/exercise3.c
EXERCISE4_SRC = $(EXERCISE_DIR)/exercise4.c
EXERCISE5_SRC = $(EXERCISE_DIR)/exercise5.c
EXERCISE6_SRC = $(EXERCISE_DIR)/exercise6.c
EXERCISE7_SRC = $(EXERCISE_DIR)/exercise7.c
EXERCISE8_SRC = $(EXERCISE_DIR)/exercise8.c
EXERCISE8_ABA_SRC = $(EXERCISE_DIR)/exercise8_aba.c
EXERCISE9_SRC = $(EXERCISE_DIR)/exercise9.c

# Targets
.PHONY: all week0 week1 week2 week3 week4 clean run-all run0a run0b run0c run0d run0e run0f run-week0 run1 run2 run3 run4 run5 run6 run7 run8 run8-aba run9 stress-test help

all: $(EXERCISE0A) $(EXERCISE0B) $(EXERCISE0C) $(EXERCISE0D) $(EXERCISE0E) $(EXERCISE0F) $(EXERCISE1) $(EXERCISE2) $(EXERCISE3) $(EXERCISE4) $(EXERCISE5) $(EXERCISE6) $(EXERCISE7) $(EXERCISE8) $(EXERCISE8_ABA) $(EXERCISE9)
	@echo "✓ All exercises compiled successfully (including Week 0 introductory exercises)"

week0: $(EXERCISE0A) $(EXERCISE0B) $(EXERCISE0C) $(EXERCISE0D) $(EXERCISE0E) $(EXERCISE0F)
	@echo "✓ Week 0 (introductory) exercises compiled successfully"

week1: $(EXERCISE1) $(EXERCISE2) $(EXERCISE3)
	@echo "✓ Week 1 exercises compiled successfully"

week2: $(EXERCISE4) $(EXERCISE5)
	@echo "✓ Week 2 exercises compiled successfully"

week3: $(EXERCISE6) $(EXERCISE7)
	@echo "✓ Week 3 exercises compiled successfully"

week4: $(EXERCISE8) $(EXERCISE8_ABA) $(EXERCISE9)
	@echo "✓ Week 4 exercises compiled successfully"

# Introductory exercises (Week 0)
$(EXERCISE0A): $(EXERCISE0A_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE0B): $(EXERCISE0B_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE0C): $(EXERCISE0C_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE0D): $(EXERCISE0D_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE0E): $(EXERCISE0E_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE0F): $(EXERCISE0F_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Main exercises
$(EXERCISE1): $(EXERCISE1_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE2): $(EXERCISE2_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE3): $(EXERCISE3_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE4): $(EXERCISE4_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE5): $(EXERCISE5_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE6): $(EXERCISE6_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE7): $(EXERCISE7_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE8): $(EXERCISE8_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE8_ABA): $(EXERCISE8_ABA_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(EXERCISE9): $(EXERCISE9_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Run introductory exercises (Week 0)
run0a: $(EXERCISE0A)
	@echo "=== Running Exercise 0A: Time Basics ==="
	./$(EXERCISE0A)

run0b: $(EXERCISE0B)
	@echo "=== Running Exercise 0B: Threading ==="
	./$(EXERCISE0B)

run0c: $(EXERCISE0C)
	@echo "=== Running Exercise 0C: File I/O ==="
	./$(EXERCISE0C)

run0d: $(EXERCISE0D)
	@echo "=== Running Exercise 0D: Busy-Wait ==="
	./$(EXERCISE0D)

run0e: $(EXERCISE0E)
	@echo "=== Running Exercise 0E: Scheduling ==="
	./$(EXERCISE0E)

run0f: $(EXERCISE0F)
	@echo "=== Running Exercise 0F: Statistics ==="
	./$(EXERCISE0F)

run-week0: run0a run0b run0c run0d run0e run0f

# Run main exercises
run1: $(EXERCISE1)
	@echo "=== Running Exercise 1: Latency Measurement ==="
	./$(EXERCISE1)
	@if [ -f $(RESULT_DIR)/latency_raw.txt ]; then \
		echo "=== Generating visualization ==="; \
		python3 $(SCRIPT_DIR)/plot_latency.py $(RESULT_DIR)/latency_raw.txt; \
	fi

run2: $(EXERCISE2)
	@echo "=== Running Exercise 2: Scheduling Policies ==="
	./$(EXERCISE2)

run3: $(EXERCISE3)
	@echo "=== Running Exercise 3: Periodic Tasks (Normal) ==="
	./$(EXERCISE3)
	@echo ""
	@echo "=== Running Exercise 3: Periodic Tasks (RT) ==="
	./$(EXERCISE3) --rt

run4: $(EXERCISE4)
	@echo "=== Running Exercise 4: Priority Inversion Demo ==="
	./$(EXERCISE4)

run5: $(EXERCISE5)
	@echo "=== Running Exercise 5: Priority Inheritance Solution ==="
	./$(EXERCISE5)

run6: $(EXERCISE6)
	@echo "=== Running Exercise 6: Deadlock Demonstration ==="
	./$(EXERCISE6)

run7: $(EXERCISE7)
	@echo "=== Running Exercise 7: Deadlock Prevention ==="
	./$(EXERCISE7)

run8: $(EXERCISE8)
	@echo "=== Running Exercise 8: Lock-Free Stack (Safe - Phased) ==="
	./$(EXERCISE8)

run8-aba: $(EXERCISE8_ABA)
	@echo "=== Running Exercise 8 ABA: Lock-Free Stack with ABA Demo ==="
	./$(EXERCISE8_ABA)

run9: $(EXERCISE9)
	@echo "=== Running Exercise 9: Synchronization Performance ==="
	./$(EXERCISE9)

run-all: run1 run2 run3 run4 run5 run6 run7 run8 run9

run-week1: run1 run2 run3

run-week2: run4 run5

run-week3: run6 run7

run-week4: run8 run8-aba run9

# Stress testing
stress-test: $(EXERCISE1)
	@echo "=== Baseline measurement (no load) ==="
	./$(EXERCISE1)
	@mv $(RESULT_DIR)/latency_raw.txt $(RESULT_DIR)/latency_baseline.txt
	@echo ""
	@echo "=== Starting stress test (CPU + I/O load) ==="
	@echo "This will take about 10 seconds..."
	stress-ng --cpu 4 --io 2 --timeout 10s &
	@sleep 1
	./$(EXERCISE1)
	@mv $(RESULT_DIR)/latency_raw.txt $(RESULT_DIR)/latency_stressed.txt
	@echo ""
	@echo "=== Results saved ==="
	@echo "Baseline: $(RESULT_DIR)/latency_baseline.txt"
	@echo "Stressed: $(RESULT_DIR)/latency_stressed.txt"

# Clean build artifacts
clean:
	rm -f $(EXERCISE0A) $(EXERCISE0B) $(EXERCISE0C) $(EXERCISE0D) $(EXERCISE0E) $(EXERCISE0F)
	rm -f $(EXERCISE1) $(EXERCISE2) $(EXERCISE3) $(EXERCISE4) $(EXERCISE5) $(EXERCISE6) $(EXERCISE7) $(EXERCISE8) $(EXERCISE8_ABA) $(EXERCISE9)
	rm -f *.o
	@echo "✓ Cleaned build artifacts"

clean-results:
	rm -f $(RESULT_DIR)/*.txt $(RESULT_DIR)/*.png
	@echo "✓ Cleaned results"

clean-all: clean clean-results

# Help target
help:
	@echo "Real-Time Systems Lab - Week 0 (Intro) + Weeks 1-4 Makefile"
	@echo ""
	@echo "=== WEEK 0: INTRODUCTORY EXERCISES ==="
	@echo "Build targets:"
	@echo "  make week0                - Build all Week 0 exercises"
	@echo "  make exercise0a_time_basics    - Time measurement basics"
	@echo "  make exercise0b_threading      - POSIX threads introduction"
	@echo "  make exercise0c_file_io        - File I/O and structs"
	@echo "  make exercise0d_busywait       - Command-line args and busy-wait"
	@echo "  make exercise0e_scheduling     - Scheduling policies intro"
	@echo "  make exercise0f_statistics     - Statistical analysis"
	@echo ""
	@echo "Run targets (Week 0):"
	@echo "  make run0a        - Run Exercise 0A (time basics)"
	@echo "  make run0b        - Run Exercise 0B (threading)"
	@echo "  make run0c        - Run Exercise 0C (file I/O)"
	@echo "  make run0d        - Run Exercise 0D (busy-wait)"
	@echo "  make run0e        - Run Exercise 0E (scheduling)"
	@echo "  make run0f        - Run Exercise 0F (statistics)"
	@echo "  make run-week0    - Run all Week 0 exercises"
	@echo ""
	@echo "=== MAIN EXERCISES ==="
	@echo "Build targets:"
	@echo "  make all          - Build all exercises (Week 0-4)"
	@echo "  make week1        - Build Week 1 exercises (1-3)"
	@echo "  make week2        - Build Week 2 exercises (4-5)"
	@echo "  make week3        - Build Week 3 exercises (6-7)"
	@echo "  make week4        - Build Week 4 exercises (8-9)"
	@echo "  make exercise1    - Build Exercise 1 (latency measurement)"
	@echo "  make exercise2    - Build Exercise 2 (scheduling policies)"
	@echo "  make exercise3    - Build Exercise 3 (periodic tasks)"
	@echo "  make exercise4    - Build Exercise 4 (priority inversion)"
	@echo "  make exercise5    - Build Exercise 5 (priority inheritance)"
	@echo "  make exercise6    - Build Exercise 6 (deadlock demonstration)"
	@echo "  make exercise7    - Build Exercise 7 (deadlock prevention)"
	@echo "  make exercise8    - Build Exercise 8 (lock-free stack - safe)"
	@echo "  make exercise8_aba - Build Exercise 8 ABA (demonstrates ABA problem)"
	@echo "  make exercise9    - Build Exercise 9 (performance analysis)"
	@echo ""
	@echo "Run targets (Main exercises):"
	@echo "  make run1         - Run Exercise 1 with visualization"
	@echo "  make run2         - Run Exercise 2"
	@echo "  make run3         - Run Exercise 3 (both modes)"
	@echo "  make run4         - Run Exercise 4 (priority inversion demo)"
	@echo "  make run5         - Run Exercise 5 (priority inheritance)"
	@echo "  make run6         - Run Exercise 6 (deadlock demo)"
	@echo "  make run7         - Run Exercise 7 (deadlock prevention)"
	@echo "  make run8         - Run Exercise 8 (lock-free stack - safe)"
	@echo "  make run8-aba     - Run Exercise 8 ABA (demonstrates ABA problem)"
	@echo "  make run9         - Run Exercise 9 (performance analysis)"
	@echo "  make run-all      - Run all exercises"
	@echo "  make run-week1    - Run Week 1 exercises"
	@echo "  make run-week2    - Run Week 2 exercises"
	@echo "  make run-week3    - Run Week 3 exercises"
	@echo "  make run-week4    - Run Week 4 exercises"
	@echo ""
	@echo "Other targets:"
	@echo "  make stress-test  - Run latency test under system load"
	@echo "  make clean        - Remove compiled binaries"
	@echo "  make clean-results - Remove result files"
	@echo "  make clean-all    - Clean everything"
	@echo "  make help         - Show this help message"
