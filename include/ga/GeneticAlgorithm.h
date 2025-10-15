#pragma once

#include <random>
#include <string>
#include <vector>

#include "tsp/Node.h"
#include "Parameters.h"
#include "Path.h"

int genetic_algorithm(std::string file_path, GAParameters params);
int parallel_genetic_algorithm(std::string file_path, GAParameters params, int num_threads);
