#include "path.h"

double calculate_fitness(double distance) {
    return 1.0 / distance;
}

Path create_path_by_sequence(std::vector<Node>& sequence){
    int size = sequence.size();
    double distance = 0.0;

    for (int i = 0; i < size - 1; i++){
        distance += calculate_distance(sequence[i], sequence[i+1]);
    }
    distance += calculate_distance(sequence[size-1], sequence[0]);

    double fitness = calculate_fitness(distance);

    Path path(distance, sequence, fitness);

    return path;
}