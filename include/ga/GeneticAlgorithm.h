#pragma once

#include <random>
#include <string>
#include <vector>

#include "tsp/Node.h"
#include "Parameters.h"
#include "Path.h"

Path genetic_algorithm(
    std::vector<Node>& node_list,
    GAParameters& params,
    std::mt19937& gen,
    std::uniform_real_distribution<>& prob_dist
);

Path parallel_genetic_algorithm(
    std::vector<Node>& node_list,
    GAParameters& params,
    std::mt19937& gen,
    int num_threads
);
