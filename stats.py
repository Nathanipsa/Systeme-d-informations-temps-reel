"""
Worst Case Execution Time (WCET) analysis for final.c compilation
Measures compilation time over 1000 iterations and calculates statistics
"""

import subprocess
import time
from tqdm import tqdm

SECURITY_FACTOR = 1.3
MAX_ITERATIONS = 1000

execution_times = []
wcet = 0

# Measure compilation time for multiple iterations
for _ in tqdm(range(MAX_ITERATIONS), desc="Calculating WCET"):
    start_time = time.time()
    subprocess.run(["gcc", "-c", "final.c"],
                   stdout=subprocess.DEVNULL,
                   stderr=subprocess.DEVNULL)
    end_time = time.time()

    elapsed_time = end_time - start_time

    if elapsed_time > wcet:
        wcet = elapsed_time

    execution_times.append(elapsed_time)

# Calculate quartiles
sorted_times = sorted(execution_times)
q1 = sorted_times[int(0.25 * len(execution_times))]
q2 = sorted_times[int(0.50 * len(execution_times))]
q3 = sorted_times[int(0.75 * len(execution_times))]
set_time = wcet * SECURITY_FACTOR

# Display results
print("\n" + "=" * 50)
print("WCET Analysis Results")
print("=" * 50)
print(f"Worst Case Execution Time (WCET): {wcet:.6f} seconds")
print(f"Secure Execution Time (SET):      {set_time:.6f} seconds")
print(f"First Quartile (Q1):              {q1:.6f} seconds")
print(f"Median (Q2):                      {q2:.6f} seconds")
print(f"Third Quartile (Q3):              {q3:.6f} seconds")
print("=" * 50)