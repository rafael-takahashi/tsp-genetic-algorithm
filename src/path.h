#pragma once

#include <optional>
#include <vector>

#include "node.h"

struct Path {
    double distance = 0.0;
    std::vector<Node> node_sequence = {};
    size_t size = 0;
    std::optional<double> fitness = std::nullopt;

    Path() = default;

    Path(double distance, std::vector<Node>& sequence, std::optional<double> fitness = std::nullopt)
        : distance(distance), 
          node_sequence(sequence), 
          size(sequence.size()),
          fitness(fitness) {}
};