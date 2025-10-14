#pragma once

#include <random>
#include <string>
#include <vector>

#include "tsp/node.h"
#include "path.h"

int genetic_algorithm(std::string file_path);

int parallel_genetic_algorithm(std::string file_path);
