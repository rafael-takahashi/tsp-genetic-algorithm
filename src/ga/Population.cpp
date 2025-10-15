#include <algorithm>
#include <queue>
#include <vector>

#include "ga/Population.h"
#include "tsp/Node.h"

using namespace std;

Path generate_random_path(const vector<Node>& node_list, mt19937& gen) {
    size_t node_list_length = node_list.size();

    vector<int> indexes(node_list_length);
    iota(indexes.begin(), indexes.end(), 0);

    shuffle(indexes.begin(), indexes.end(), gen);

    vector<Node> node_sequence(node_list_length);
    double res_distance = 0;

    for (size_t i = 0; i < node_list_length; i++) {
        node_sequence[i] = node_list[indexes[i]];
        if (i > 0) {
            res_distance += calculate_distance(
                node_list[indexes[i-1]], node_list[indexes[i]]
            );
        }
    }

    return Path(res_distance, node_sequence, calculate_fitness(res_distance));
}

vector<Path> generate_population(const vector<Node>& node_list, mt19937& gen) {
    vector<Path> population(node_list.size());
    
    for (size_t i = 0; i < node_list.size(); i++) 
        population[i] = generate_random_path(node_list, gen);
        
    return population;
}

struct CompareFitness {
    bool operator()(const pair<int, double>& a, const pair<int, double>& b) {
        return a.second < b.second;
    }
};

vector<int> get_elite_indexes(const vector<Path>& population, size_t elite_size) {
    priority_queue<pair<int, double>,
                    vector<pair<int, double>>,
                    CompareFitness> heap;

    for (size_t i = 0; i < population.size(); i++) {
        double fit = population[i].fitness;
        if (heap.size() < elite_size)
            heap.push({static_cast<int>(i), fit});
        else if (fit < heap.top().second) {
            heap.pop();
            heap.push({static_cast<int>(i), fit});
        }
    }

    vector<int> elite_indexes;
    elite_indexes.reserve(elite_size);
    while (!heap.empty()) {
        elite_indexes.push_back(heap.top().first);
        heap.pop();
    }

    return elite_indexes;
}