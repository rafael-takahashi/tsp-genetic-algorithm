#pragma once

#include <vector>

#include "node.h"
#include "path.h"

Path generate_random_path(std::vector<Node>& node_list);
std::vector<Path> generate_population(std::vector<Node>& node_list);

double calculate_fitness(double distance);
double fitness_evaluation(Path path);
std::vector<Path> tournament_selection(const std::vector<Path>& population, const std::vector<double>& fitnesses, int tournament_size = 3);
//vector<Path> ox_crossover(const vector<Path>& parents);