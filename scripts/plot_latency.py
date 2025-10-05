#!/usr/bin/env python3
"""
Latency Visualization Script

Reads latency measurement data and creates visualization plots.
"""

import sys
import matplotlib.pyplot as plt
import numpy as np

def plot_latency(filename):
    """Plot latency measurements from file."""

    # Read data
    iterations = []
    deviations = []

    try:
        with open(filename, 'r') as f:
            for line in f:
                line = line.strip()
                if line.startswith('#') or not line:
                    continue
                parts = line.split()
                if len(parts) >= 2:
                    iterations.append(int(parts[0]))
                    deviations.append(float(parts[1]) / 1000.0)  # Convert to microseconds
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found")
        sys.exit(1)
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)

    if not deviations:
        print("Error: No data found in file")
        sys.exit(1)

    # Calculate statistics
    deviations_array = np.array(deviations)
    avg = np.mean(deviations_array)
    min_val = np.min(deviations_array)
    max_val = np.max(deviations_array)
    jitter = max_val - min_val
    std = np.std(deviations_array)

    # Create figure with subplots
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('Latency Measurement Analysis', fontsize=16, fontweight='bold')

    # 1. Time series plot
    axes[0, 0].plot(iterations, deviations, linewidth=0.5, alpha=0.7)
    axes[0, 0].axhline(y=avg, color='r', linestyle='--', label=f'Average: {avg:.2f} µs')
    axes[0, 0].set_xlabel('Iteration')
    axes[0, 0].set_ylabel('Latency Deviation (µs)')
    axes[0, 0].set_title('Latency Over Time')
    axes[0, 0].legend()
    axes[0, 0].grid(True, alpha=0.3)

    # 2. Histogram
    axes[0, 1].hist(deviations, bins=50, edgecolor='black', alpha=0.7)
    axes[0, 1].axvline(x=avg, color='r', linestyle='--', label=f'Average: {avg:.2f} µs')
    axes[0, 1].set_xlabel('Latency Deviation (µs)')
    axes[0, 1].set_ylabel('Frequency')
    axes[0, 1].set_title('Latency Distribution')
    axes[0, 1].legend()
    axes[0, 1].grid(True, alpha=0.3)

    # 3. Box plot
    axes[1, 0].boxplot(deviations, vert=True)
    axes[1, 0].set_ylabel('Latency Deviation (µs)')
    axes[1, 0].set_title('Latency Statistics (Box Plot)')
    axes[1, 0].grid(True, alpha=0.3)

    # 4. Statistics summary
    axes[1, 1].axis('off')
    stats_text = f"""
    LATENCY STATISTICS
    ══════════════════════════════

    Average:     {avg:.2f} µs
    Minimum:     {min_val:.2f} µs
    Maximum:     {max_val:.2f} µs
    Jitter:      {jitter:.2f} µs
    Std Dev:     {std:.2f} µs

    Samples:     {len(deviations)}

    ══════════════════════════════

    Jitter Assessment:
    """

    if jitter < 100:
        stats_text += "  ✓ Low jitter\n    Good for soft RT"
    elif jitter < 500:
        stats_text += "  ⚠ Moderate jitter\n    Acceptable for some RT"
    else:
        stats_text += "  ✗ High jitter\n    Not suitable for hard RT"

    axes[1, 1].text(0.1, 0.5, stats_text, fontsize=11, family='monospace',
                    verticalalignment='center')

    plt.tight_layout()

    # Save figure
    output_file = filename.replace('.txt', '.png')
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    print(f"✓ Plot saved to: {output_file}")

    # Try to display (may not work in headless environments)
    try:
        plt.show()
    except:
        print("  (Display not available - plot saved to file)")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 plot_latency.py <latency_data_file>")
        print("Example: python3 plot_latency.py results/latency_raw.txt")
        sys.exit(1)

    plot_latency(sys.argv[1])
