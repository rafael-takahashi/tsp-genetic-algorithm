#pragma once

#include <random>
#include <utility>
#include <vector>

#include "ga/Path.h"
#include "tsp/Node.h"

std::pair<Path, Path> tournament_selection(
    std::vector<Path>& population,
    std::mt19937& gen,
    size_t tournament_size
);

std::pair<Path, Path> order_crossover(
    std::vector<Node>& parent1_seq,
    std::vector<Node>& parent2_seq,
    std::mt19937& gen
);

std::pair<Path, Path> partially_mapped_crossover(
    std::vector<Node>& parent1_seq,
    std::vector<Node>& parent2_seq,
    std::mt19937& gen
);

void swap_mutation(Path& path, std::mt19937& gen);
