#pragma once

#include <vector>

#include "node.h"
#include "path.h"

Path generate_random_path(std::vector<Node>& node_list, int seed);
std::vector<Path> generate_population(std::vector<Node>& node_list);

std::pair<Path, Path> tournament_selection(std::vector<Path>& population, long unsigned int tournament_size, int seed);

std::pair<Path, Path> ox_crossover(std::vector<Node>& parent1_seq, std::vector<Node>& parent2_seq);
std::pair<Path, Path> pmx_crossover(std::vector<Node>& parent1_seq, std::vector<Node>& parent2_seq);

void mutation(std::vector<Path>& population, float mutation_rate);

std::vector<Path> elitism(std::vector<Path>& population, std::vector<Path>& offspring);