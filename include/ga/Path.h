#pragma once

#include <vector>

#include "tsp/Node.h"

struct Path {
    double distance = 0.0;
    std::vector<Node> node_sequence = {};
    size_t size = 0;
    double fitness = 0.0;

    Path() = default;

    Path(double distance, std::vector<Node>& sequence, double fitness)
        : distance(distance), 
          node_sequence(sequence), 
          size(sequence.size()),
          fitness(fitness) {}
};

double calculate_fitness(double distance);
Path create_path_by_sequence(std::vector<Node>& sequence);