#pragma once

#include <vector>
#include "node.h"

struct TspResult {
    double traveled_distance;
    std::vector<Node> node_sequence;
};

std::vector<Node> tsp_to_vector(const std::string& file_path);
TspResult nearest_neighbor(const std::vector<Node>& node_list);
TspResult two_opt(const std::vector<Node>& path);
double calculate_total_distance(const std::vector<Node>& node_list);