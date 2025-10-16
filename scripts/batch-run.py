import subprocess
import os

EXECUTABLE = "./tsp_ga"

instances = [
    "instances/u574.tsp",
    # "instances/pr1002.tsp",
    # "instances/fnl4461.tsp",
    # "instances/brd14051.tsp"
]

run_flags = [ ["-s"] ] + [ ["-t", str(n)] for n in [1, 2, 4, 8, 16] ]

os.makedirs("results", exist_ok=True)

for instance in instances:
    for flag in run_flags:
        cmd = [EXECUTABLE] + flag + [instance]
        print(f"Running: {' '.join(cmd)}")
        try:
            subprocess.run(cmd, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error running {cmd}: {e}")
