#pragma once

#include <vector>

#include "node.h"
#include "path.h"

Path generate_random_path(std::vector<Node>& node_list, int seed);
std::vector<Path> generate_population(std::vector<Node>& node_list);

double calculate_fitness(double distance);
double fitness_evaluation(Path path);
std::pair<Path, Path> tournament_selection(std::vector<Path>& population, long unsigned int tournament_size, int seed);
std::vector<Path> ox_crossover(const std::vector<Path>& parents);
std::vector<Path> pmx_crossover(std::vector<Node>& parent1_seq, std::vector<Node>& parent2_seq);