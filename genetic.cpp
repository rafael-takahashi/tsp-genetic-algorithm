#include "genetic.h"

#include <random>

using namespace std;

Path generate_random_path(std::vector<Node>& node_list, unsigned int seed) {
    vector<Node> node_sequence;
    double res_distance = 0;
    vector<int> unvisited;
    for (int i = 0; i < node_list.size(); i++)
        unvisited.push_back(i);
    mt19937 gen(seed);

    int random_index = gen() % unvisited.size();
    int current = unvisited[random_index];
    node_sequence.push_back(node_list[current]);

    unvisited.erase(unvisited.begin() + random_index);

    while (!unvisited.empty()) {
        random_index = gen() % unvisited.size();
        int next = unvisited[random_index];
        node_sequence.push_back(node_list[next]);

        double distance = calculate_distance(node_list[current], node_list[next]);
        res_distance += distance;

        unvisited.erase(unvisited.begin() + random_index);

        current = next;
    }

    return Path(res_distance, node_sequence);
}


double fitness_evaluation(Path path){
    return 1/path.distance;
}

std::vector<Node> tournament_selection(const std::vector<std::vector<Node>>& population, const std::vector<double>& fitnesses, int tournament_size = 3){
    int best_index = -1;
    double best_fitness = -1.0;

    for(int i =0; i < tournament_size; i++){
        int index = rand() % population.size();
        if(fitnesses[index] > best_fitness){
            best_fitness = fitnesses[index];
            best_index = index;
        }
    }

    return population[best_index];
}