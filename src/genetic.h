#pragma once

#include <vector>

#include "node.h"
#include "path.h"

Path generate_random_path(const std::vector<Node>& node_list, int seed);
std::vector<Path> generate_population(const std::vector<Node>& node_list);

std::pair<Path, Path> tournament_selection(std::vector<Path>& population, int seed);

std::pair<Path, Path> ox_crossover(std::vector<Node>& parent1_seq, std::vector<Node>& parent2_seq);
std::pair<Path, Path> pmx_crossover(std::vector<Node>& parent1_seq, std::vector<Node>& parent2_seq);

void swap_mutation(Path& path);

std::pair<int, int> find_two_worst_indexes(std::vector<Path>& population);