#pragma once

#include <vector>
#include "node.h"

struct TspResult {
    double traveled_distance;
    std::vector<Node> node_sequence;
};

std::vector<Node> tsp_to_vector(const std::string& file_path);
TspResult nearest_neighbor(const std::vector<Node>& node_list);
double calculate_distance(double x1, double y1, double x2, double y2);