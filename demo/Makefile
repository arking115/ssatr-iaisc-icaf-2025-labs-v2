# Makefile for 3-Thread RT Demo Application

CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread -lm
LDFLAGS = -pthread -lrt -lm

# Executable
TARGET = sample_3threads

# Source file
SRC = sample_3threads.c

.PHONY: all clean run run-rt run-long run-stressed help

all: $(TARGET)
	@echo "✓ Demo application compiled successfully"
	@echo "  Run 'make run' or 'make run-rt' to execute"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Run with normal scheduling
run: $(TARGET)
	@echo "=== Running with SCHED_OTHER (normal scheduling) ==="
	./$(TARGET)

# Run with RT scheduling
run-rt: $(TARGET)
	@echo "=== Running with SCHED_FIFO (real-time scheduling) ==="
	@echo "Note: Requires Docker container with --privileged flag"
	./$(TARGET) --rt

# Run longer duration
run-long: $(TARGET)
	@echo "=== Running 10-second test with RT scheduling ==="
	./$(TARGET) --rt --duration 10

# Run under system stress
run-stressed: $(TARGET)
	@echo "=== Running under CPU stress ==="
	@echo "Starting stress-ng in background..."
	@stress-ng --cpu 4 --timeout 10s &
	@sleep 1
	@echo "Running application with RT scheduling under load..."
	./$(TARGET) --rt --duration 8
	@echo ""
	@echo "Compare with normal run to see RT scheduling benefits"

# Compare normal vs RT
compare: $(TARGET)
	@echo "=== Comparison: Normal vs RT Scheduling ==="
	@echo ""
	@echo "1. Normal Scheduling:"
	@echo "--------------------"
	./$(TARGET) --duration 3
	@echo ""
	@echo "2. RT Scheduling:"
	@echo "--------------------"
	./$(TARGET) --rt --duration 3
	@echo ""
	@echo "Notice the difference in latency and jitter!"

clean:
	rm -f $(TARGET) *.o
	@echo "✓ Cleaned demo build artifacts"

help:
	@echo "3-Thread RT Demo Application - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make all          - Build the demo application"
	@echo "  make run          - Run with normal scheduling"
	@echo "  make run-rt       - Run with RT scheduling (needs --privileged)"
	@echo "  make run-long     - Run 10-second test with RT"
	@echo "  make run-stressed - Run under CPU stress"
	@echo "  make compare      - Compare normal vs RT scheduling"
	@echo "  make clean        - Remove compiled binary"
	@echo "  make help         - Show this help"
	@echo ""
	@echo "Manual execution:"
	@echo "  ./sample_3threads                    - Normal scheduling"
	@echo "  ./sample_3threads --rt               - RT scheduling"
	@echo "  ./sample_3threads --rt --duration 10 - RT for 10 seconds"
	@echo ""
	@echo "Docker usage:"
	@echo "  docker-compose run --rm --privileged rt-lab"
	@echo "  cd demo"
	@echo "  make run-rt"
