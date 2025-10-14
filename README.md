# Genetic Algorithm for the Traveling Salesman Problem

A parallel implementation of a generational genetic algorithm to solve the Traveling Salesman Problem (TSP), where the goal is to find the shortest path that visits all cities exactly once and returns to the starting point.

## Features

- **Parallel execution** using thread pools for improved performance
- **Sequential mode** available for comparison and benchmarking
- TSPLIB format instances
- Result visualization with Python scripts

## Project Structure
```
tsp-genetic-algorithm/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── .gitignore             # Git ignore rules
├── main.cpp               # Entry point
├── include/               # Header files
│   ├── tsp/              # TSP-specific components
│   ├── path/             # Path/tour representation
│   ├── ga/               # Genetic algorithm logic
│   └── parallel/         # Parallelization (ThreadPool)
├── src/                  # Implementation files
│   ├── tsp/
│   ├── path/
│   ├── ga/
│   └── parallel/
├── instances/            # TSPLIB problem instances (.tsp files)
├── results/              # Output directory for results
└── scripts/              # Python plotting and analysis scripts
```

## Requirements

- **C++17** compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- **CMake 3.15** or higher
- *(Optional)* Python 3.x for visualization scripts

## Building the Project

### Linux/macOS
```bash
# Clone the repository
git clone <repository-url>
cd tsp-genetic-algorithm

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make -j

# Executable will be at: ./tsp_ga
```

### Windows (Visual Studio)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Build Types
```bash
# Debug build (with debugging symbols, no optimization)
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Release build (with optimizations)
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Usage

### Basic Usage
```bash
# Run with default settings (parallel mode, default instance)
./tsp_ga

# Specify a TSP instance
./tsp_ga instances/berlin52.tsp

# Run in sequential mode
./tsp_ga -s instances/berlin52.tsp
./tsp_ga --sequential instances/eil51.tsp
```

## TSP Instances

This project uses TSPLIB format instances. Download instances from [TSPLIB](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/) and place them in the `instances/` directory.

## Algorithm Parameters

Configure genetic algorithm parameters in the source code:
- Population size
- Tournament size
- Mutation rate
- Local Optimization (2-opt) rate
- Elite size
- Number of generations

## Visualization

Python scripts for analyzing and plotting results are in the `scripts/` directory:
```bash
cd scripts

# Install dependencies
pip install -r requirements.txt

# Plot convergence
python plot_convergence.py ../results/stats.csv

# Visualize best tour
python plot_tour.py ../results/best_tour.txt
```

## Results

Results are saved in the `results/` directory:
- Best tour found
- Fitness statistics per generation
- Execution time comparisons (sequential vs parallel)

## Development

### Cleaning Build Files
```bash
# From build directory
make clean

# Complete rebuild
cd ..
rm -rf build/
mkdir build && cd build
cmake .. && make -j
```

### Running Tests
```bash
# If you add tests
cd build
ctest
```

## Performance Comparison

Compare sequential vs parallel performance:
```bash
# Sequential
time ./tsp_ga -s instances/u574.tsp

# Parallel
time ./tsp_ga instances/u574.tsp
```

## References

- TSPLIB: http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/
- Thread Pool Pattern: https://www.geeksforgeeks.org/cpp/thread-pool-in-cpp/
