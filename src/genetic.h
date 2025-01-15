#pragma once

#include <random>
#include <vector>

#include "node.h"
#include "path.h"

Path generate_random_path(const std::vector<Node>& node_list, std::mt19937& gen);
std::vector<Path> generate_population(const std::vector<Node>& node_list, std::mt19937& gen);

std::pair<Path, Path> tournament_selection(std::vector<Path>& population, std::mt19937& gen);

std::pair<Path, Path> order_crossover(std::vector<Node>& parent1_seq, std::vector<Node>& parent2_seq, std::mt19937& gen);
std::pair<Path, Path> partially_mapped_crossover(std::vector<Node>& parent1_seq, std::vector<Node>& parent2_seq, std::mt19937& gen);

void swap_mutation(Path& path, std::mt19937& gen);

std::pair<int, int> find_two_worst_indexes(std::vector<Path>& population);