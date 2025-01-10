#pragma once

#include <vector>

#include "node.h"
#include "path.h"

std::vector<Node> tsp_to_vector(std::string& file_path);

Path nearest_neighbor(std::vector<Node>& node_list);
Path farthest_insertion(std::vector<Node>& node_list);

Path two_opt(Path& path);
Path pair_swap(Path& path);