#pragma once

#include <vector>
#include "node.h"

struct Path {
    double distance;
    std::vector<Node> node_sequence;
    size_t size;

    Path(double distance, const std::vector<Node>& sequence)
        : distance(distance), 
          node_sequence(sequence), 
          size(sequence.size()) {}
};

std::vector<Node> tsp_to_vector(const std::string& file_path);
Path nearest_neighbor(const std::vector<Node>& node_list);
Path two_opt(const Path& path);