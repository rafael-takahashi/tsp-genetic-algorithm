# Genetic Algorithm for the Traveling Salesman Problem

This repository contains the implementation of a genetic algorithm to solve the Traveling Salesman Problem (TSP), where the goal is to find the shortest path that visits all cities in a list, visiting each one exactly once and returning to the starting point.

## How to Use the Code

### Instances
To choose your instance, modify the `file_path` variable in `src/main.cpp`. The selected instance should be present in the `instances/` directory. You should use .tsp instances, which can be found at [TSPLIB](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/).

### Parameters
You can modify the parameters used by the genetic algorithm in `src/path.cpp`.

### Compilation
In the terminal, compile the code by executing the following command:

```
g++ src/*.cpp
```
