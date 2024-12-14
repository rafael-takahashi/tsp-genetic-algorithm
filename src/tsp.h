#pragma once

#include <vector>

#include "node.h"

struct Path {
    double distance;
    std::vector<Node> node_sequence;
    size_t size;

    Path(double distance, std::vector<Node>& sequence)
        : distance(distance), 
          node_sequence(sequence), 
          size(sequence.size()) {}
};

std::vector<Node> tsp_to_vector(std::string& file_path);

Path nearest_neighbor(std::vector<Node>& node_list);
Path farthest_insertion(std::vector<Node>& node_list);

Path two_opt(Path& path);
Path pair_swap(Path& path);