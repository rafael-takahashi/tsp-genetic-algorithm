import subprocess
import os

EXECUTABLE = "./tsp_ga"
INSTANCES_DIR = "instances/"
NUM_RUNS = 3

instances = [
    f"{INSTANCES_DIR}a280.tsp",
    f"{INSTANCES_DIR}u574.tsp",
    f"{INSTANCES_DIR}pr1002.tsp",
    f"{INSTANCES_DIR}fnl4461.tsp",
    f"{INSTANCES_DIR}rl5934.tsp",
]

run_flags = [["-s"]] + [["-t", str(n)] for n in [1, 2, 4, 8, 16]]

os.makedirs("results", exist_ok=True)

def flag_to_threads(flag):
    return int(flag[1]) if len(flag) > 1 else 1

for instance in instances:
    instance_name = os.path.splitext(os.path.basename(instance))[0]

    for flag in run_flags:
        num_threads = flag_to_threads(flag)

        for run_id in range(1, NUM_RUNS + 1):
            cmd = ["nice", "-n", "10", EXECUTABLE] + flag + [instance]

            print("\n" + "-"*60)
            print(f"[Run {run_id}/{NUM_RUNS}] Instance: {instance_name}")
            print(f"Mode: {'sequential' if num_threads == 1 and flag[0] == '-s' else 'parallel'} | Threads: {num_threads}")
            print(f"Command: {' '.join(cmd)}")
            print("-"*60)

            try:
                subprocess.run(cmd, check=True)
            except subprocess.CalledProcessError as e:
                print(f"Error running {cmd}: {e}")
