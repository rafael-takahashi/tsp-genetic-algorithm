import subprocess
import os

EXECUTABLE = "./tsp_ga"
INSTANCES_DIR = "instances/"
NUM_RUNS = 3

instances = [
    f"{INSTANCES_DIR}u574.tsp",
    f"{INSTANCES_DIR}pr1002.tsp",
    f"{INSTANCES_DIR}fnl4461.tsp",
    f"{INSTANCES_DIR}brd14051.tsp"
]

run_flags = [ ["-s"] ] + [ ["-t", str(n)] for n in [1, 2, 4, 8, 16] ]

os.makedirs("results", exist_ok=True)

for instance in instances:
    for flag in run_flags:
        for _ in range(NUM_RUNS):
            cmd = [EXECUTABLE] + flag + [instance]
            print(f"Running: {' '.join(cmd)}")
            try:
                subprocess.run(cmd, check=True)
            except subprocess.CalledProcessError as e:
                print(f"Error running {cmd}: {e}")
