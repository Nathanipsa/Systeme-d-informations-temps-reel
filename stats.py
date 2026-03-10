import subprocess
import time
from tqdm import tqdm

time_1 = 0
WCET = 0

security_factor = 1.3


for i in tqdm(range(10000), desc="Calculating WCET"):
    start_time = time.time()
    subprocess.run(["make", "-f", "MakeFile"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    end_time = time.time()

    time_1 = end_time - start_time
    
    if time_1 > WCET:
        WCET = time_1


print("Worst Case Execution Time (WCET) : ", WCET)
print("Secure Execution Time (SET) : ", WCET * security_factor)