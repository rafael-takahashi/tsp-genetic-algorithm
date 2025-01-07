#pragma once

#include <vector>

#include "tsp.h"

Path generate_random_path(std::vector<Node>& node_list, unsigned int seed);
std::vector<Path> generate_population(int size);