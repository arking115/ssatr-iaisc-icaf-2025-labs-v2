# Testing Your RT Systems Setup

Quick guide to verify your Docker environment is configured correctly.

## Step 1: Build the Container

```bash
docker-compose build
```

**Expected output:**
- Downloads Ubuntu 22.04 image
- Installs RT tools (rt-tests, stress-ng, etc.)
- Installs Python and matplotlib
- Completes without errors

## Step 2: Run the Container

```bash
docker-compose run --rm rt-lab
```

**Expected:**
- Starts container with privileged mode
- Drops you into `/workspace` directory
- Prompt shows: `root@<container-id>:/workspace#`

## Step 3: Verify Environment

Inside the container, run these checks:

```bash
# Check RT tools are installed
which cyclictest
which stress-ng

# Check Python and matplotlib
python3 --version
python3 -c "import matplotlib; print('matplotlib OK')"

# Check directory structure
ls -la
# Should see: exercises/, scripts/, results/, Makefile
```

## Step 4: Build Exercise 1

```bash
make exercise1
```

**Expected output:**
```
gcc -Wall -Wextra -O2 -pthread -lrt -o exercise1 exercises/exercise1.c -pthread -lrt
✓ Exercise compiled successfully
```

## Step 5: Run Exercise 1

```bash
./exercise1
```

**Expected output:**
```
=== Exercise 1: Basic Latency Measurement ===
Target sleep interval: 1000000 ns (1.000 ms)
Running 1000 iterations...

Progress: 1000/1000 iterations completed

=== Results ===
Average latency: 45230 ns (45.230 µs)
Min deviation:   12450 ns (12.450 µs)
Max deviation:   234120 ns (234.120 µs)
Jitter:          221670 ns (221.670 µs)

Results saved to: results/latency_raw.txt
...
```

**Note:** Actual values will vary based on your system.

## Step 6: Verify Results File

```bash
ls -la results/
cat results/latency_raw.txt | head -10
```

**Expected:**
- File `latency_raw.txt` exists
- Contains measurements in format: `<iteration> <deviation_ns>`

## Step 7: Test Visualization (Optional)

```bash
python3 scripts/plot_latency.py results/latency_raw.txt
```

**Expected:**
- Creates `results/latency_raw.png`
- May show "Display not available" - this is OK
- Plot file should be saved successfully

## Step 8: Test Stress Testing

```bash
make stress-test
```

**Expected:**
- Runs baseline measurement
- Starts stress-ng with CPU and I/O load
- Runs measurement under load
- Saves both results
- Shows comparison between baseline and stressed

## Step 9: Verify RT Scheduling Access

```bash
# Check scheduling policies available
chrt -m
```

**Expected output should include:**
```
SCHED_OTHER min/max priority    : 0/0
SCHED_FIFO min/max priority     : 1/99
SCHED_RR min/max priority       : 1/99
```

If you see this, your container has proper RT scheduling access.

## Troubleshooting

### Issue: "Operation not permitted" when setting RT priority

**Solution:**
```bash
# Exit container and restart with explicit privileged flag
exit
docker-compose run --rm --privileged rt-lab
```

Or modify `docker-compose.yml` to ensure `privileged: true` is set.

### Issue: "matplotlib not found"

**Solution (inside container):**
```bash
pip3 install matplotlib numpy
```

### Issue: Cannot create results file

**Check:**
```bash
ls -ld results/
# Should show: drwxr-xr-x ... results/

# Fix permissions if needed:
chmod 755 results/
```

### Issue: Compilation errors

**Check file exists:**
```bash
ls -la exercises/exercise1.c
```

**Try manual compilation:**
```bash
gcc -Wall -Wextra -O2 -pthread exercises/exercise1.c -o exercise1 -lrt
```

## Success Criteria

✓ Container builds without errors
✓ Exercise compiles successfully
✓ Exercise runs and produces output
✓ Results file is created
✓ RT scheduling policies are available
✓ Stress testing works

If all checks pass, your environment is ready for RT systems development!

## Next Steps

Once your setup is verified:
1. Review `rt_week1_guide.md` for detailed exercise explanations
2. Run all exercises: `make run-all`
3. Experiment with different configurations
4. Complete the week 1 assignment

## Quick Test Script

Run this complete test sequence:

```bash
# Build
docker-compose build

# Test
docker-compose run --rm rt-lab bash -c "
  echo '=== Environment Check ===' &&
  which cyclictest stress-ng python3 &&
  echo -e '\n=== Build Check ===' &&
  make exercise1 &&
  echo -e '\n=== Run Check ===' &&
  ./exercise1 &&
  echo -e '\n=== Results Check ===' &&
  ls -la results/latency_raw.txt &&
  echo -e '\n=== RT Scheduling Check ===' &&
  chrt -m &&
  echo -e '\n✓ All checks passed!'
"
```

If this completes without errors, you're all set!
