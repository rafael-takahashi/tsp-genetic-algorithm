#pragma once

#include <vector>

#include "tsp.h"

Path generate_random_path(std::vector<Node>& node_list, unsigned int seed);
std::vector<Path> generate_population(int size);
double fitness_evaluation(Path path);
std::vector<Node> tournament_selection(const std::vector<std::vector<Node>>& population, const std::vector<double>& fitnesses, int tournament_size = 3);