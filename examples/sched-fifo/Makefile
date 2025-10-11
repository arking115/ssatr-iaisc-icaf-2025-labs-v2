# Makefile for SCHED_FIFO Demonstration Example

CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread
LDFLAGS = -pthread

# Target executable
TARGET = sched_fifo_demo

# Source file
SRC = sched_fifo_demo.c

.PHONY: all clean run run-normal help

# Default target
all: $(TARGET)
	@echo "✓ SCHED_FIFO demo compiled successfully"
	@echo "Run with: make run (or ./$(TARGET))"

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Run with RT scheduling (requires --privileged)
run: $(TARGET)
	@echo "=== Running SCHED_FIFO Demo (RT mode) ==="
	@echo "Note: Requires --privileged flag in Docker for RT scheduling"
	@echo ""
	./$(TARGET)

# Run with normal scheduling (for comparison)
run-normal: $(TARGET)
	@echo "=== Running SCHED_FIFO Demo (Normal mode) ==="
	@echo "Using SCHED_OTHER for comparison"
	@echo ""
	./$(TARGET) --normal

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o
	@echo "✓ Cleaned build artifacts"

# Help target
help:
	@echo "SCHED_FIFO Demonstration - Makefile"
	@echo ""
	@echo "This example demonstrates SCHED_FIFO real-time scheduling behavior."
	@echo ""
	@echo "Build targets:"
	@echo "  make all          - Build the demo (default)"
	@echo "  make $(TARGET)    - Build the executable"
	@echo ""
	@echo "Run targets:"
	@echo "  make run          - Run with SCHED_FIFO (requires --privileged)"
	@echo "  make run-normal   - Run with SCHED_OTHER (no special privileges)"
	@echo ""
	@echo "Direct execution:"
	@echo "  ./$(TARGET)              - Use SCHED_FIFO (RT mode)"
	@echo "  ./$(TARGET) --normal     - Use SCHED_OTHER (normal mode)"
	@echo "  ./$(TARGET) --help       - Show help message"
	@echo ""
	@echo "Utility targets:"
	@echo "  make clean        - Remove compiled binaries"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - For RT scheduling: Run container with --privileged flag"
	@echo "    docker-compose run --rm --privileged rt-lab"
	@echo "  - Or on Linux: sudo ./$(TARGET) or use setcap"
	@echo ""
	@echo "What this demo shows:"
	@echo "  • Priority-based execution with SCHED_FIFO"
	@echo "  • Higher priority threads preempt lower priority"
	@echo "  • No time slicing - threads run to completion"
	@echo "  • Deterministic scheduling behavior"
	@echo ""
	@echo "Experiments to try:"
	@echo "  1. Compare RT vs normal:  make run && make run-normal"
	@echo "  2. Under load:            stress-ng --cpu 4 & make run"
	@echo "  3. Check scheduling:      ps -eLo pid,class,rtprio,comm | grep sched_fifo"
