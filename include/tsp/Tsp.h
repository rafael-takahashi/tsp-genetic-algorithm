#pragma once

#include <vector>

#include "tsp/Node.h"
#include "ga/Path.h"

std::vector<Node> tsp_to_vector(std::string& file_path);

Path nearest_neighbor(const std::vector<Node>& node_list);
Path farthest_insertion(const std::vector<Node>& node_list);

Path two_opt(Path& path);
double calculate_swap_delta(int i, int j, std::vector<Node>& node_sequence, int path_size);
Path pair_swap(Path& path);