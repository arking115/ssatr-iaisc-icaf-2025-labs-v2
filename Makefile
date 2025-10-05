# Makefile for Real-Time Systems Lab - Weeks 1, 2 & 3

CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread -lrt
LDFLAGS = -pthread -lrt

# Source directories
EXERCISE_DIR = exercises
SCRIPT_DIR = scripts
RESULT_DIR = results

# Executables
EXERCISE1 = exercise1
EXERCISE2 = exercise2
EXERCISE3 = exercise3
EXERCISE4 = exercise4
EXERCISE5 = exercise5
EXERCISE6 = exercise6
EXERCISE7 = exercise7

# Source files
EXERCISE1_SRC = $(EXERCISE_DIR)/exercise1.c
EXERCISE2_SRC = $(EXERCISE_DIR)/exercise2.c
EXERCISE3_SRC = $(EXERCISE_DIR)/exercise3.c
EXERCISE4_SRC = $(EXERCISE_DIR)/exercise4.c
EXERCISE5_SRC = $(EXERCISE_DIR)/exercise5.c
EXERCISE6_SRC = $(EXERCISE_DIR)/exercise6.c
EXERCISE7_SRC = $(EXERCISE_DIR)/exercise7.c

# Targets
.PHONY: all week1 week2 week3 clean run-all run1 run2 run3 run4 run5 run6 run7 stress-test help

all: $(EXERCISE1) $(EXERCISE2) $(EXERCISE3) $(EXERCISE4) $(EXERCISE5) $(EXERCISE6) $(EXERCISE7)
	@echo "✓ All exercises compiled successfully"

week1: $(EXERCISE1) $(EXERCISE2) $(EXERCISE3)
	@echo "✓ Week 1 exercises compiled successfully"

week2: $(EXERCISE4) $(EXERCISE5)
	@echo "✓ Week 2 exercises compiled successfully"

week3: $(EXERCISE6) $(EXERCISE7)
	@echo "✓ Week 3 exercises compiled successfully"

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

# Run exercises
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

run-all: run1 run2 run3 run4 run5 run6 run7

run-week1: run1 run2 run3

run-week2: run4 run5

run-week3: run6 run7

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
	rm -f $(EXERCISE1) $(EXERCISE2) $(EXERCISE3) $(EXERCISE4) $(EXERCISE5) $(EXERCISE6) $(EXERCISE7)
	rm -f *.o
	@echo "✓ Cleaned build artifacts"

clean-results:
	rm -f $(RESULT_DIR)/*.txt $(RESULT_DIR)/*.png
	@echo "✓ Cleaned results"

clean-all: clean clean-results

# Help target
help:
	@echo "Real-Time Systems Lab - Weeks 1, 2 & 3 Makefile"
	@echo ""
	@echo "Build targets:"
	@echo "  make all          - Build all exercises (Week 1, 2 & 3)"
	@echo "  make week1        - Build Week 1 exercises (1-3)"
	@echo "  make week2        - Build Week 2 exercises (4-5)"
	@echo "  make week3        - Build Week 3 exercises (6-7)"
	@echo "  make exercise1    - Build Exercise 1 (latency measurement)"
	@echo "  make exercise2    - Build Exercise 2 (scheduling policies)"
	@echo "  make exercise3    - Build Exercise 3 (periodic tasks)"
	@echo "  make exercise4    - Build Exercise 4 (priority inversion)"
	@echo "  make exercise5    - Build Exercise 5 (priority inheritance)"
	@echo "  make exercise6    - Build Exercise 6 (deadlock demonstration)"
	@echo "  make exercise7    - Build Exercise 7 (deadlock prevention)"
	@echo ""
	@echo "Run targets:"
	@echo "  make run1         - Run Exercise 1 with visualization"
	@echo "  make run2         - Run Exercise 2"
	@echo "  make run3         - Run Exercise 3 (both modes)"
	@echo "  make run4         - Run Exercise 4 (priority inversion demo)"
	@echo "  make run5         - Run Exercise 5 (priority inheritance)"
	@echo "  make run6         - Run Exercise 6 (deadlock demo)"
	@echo "  make run7         - Run Exercise 7 (deadlock prevention)"
	@echo "  make run-all      - Run all exercises"
	@echo "  make run-week1    - Run Week 1 exercises"
	@echo "  make run-week2    - Run Week 2 exercises"
	@echo "  make run-week3    - Run Week 3 exercises"
	@echo ""
	@echo "Other targets:"
	@echo "  make stress-test  - Run latency test under system load"
	@echo "  make clean        - Remove compiled binaries"
	@echo "  make clean-results - Remove result files"
	@echo "  make clean-all    - Clean everything"
	@echo "  make help         - Show this help message"
