#pragma once

#include <random>
#include <vector>

#include "ga/Path.h"
#include "tsp/Node.h"

Path generate_random_path(
    const std::vector<Node>& node_list,
    std::mt19937& gen
);

std::vector<Path> generate_population(
    const std::vector<Node>& node_list,
    std::mt19937& gen
);

std::vector<int> get_elite_indexes(
    const std::vector<Path>& population,
    size_t elite_size
);

double get_average_distance(const std::vector<Path>& population);
Path get_best_path(const std::vector<Path>& population);
