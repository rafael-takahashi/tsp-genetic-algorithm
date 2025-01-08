#include "genetic.h"

#include <random>
#include <algorithm>

using namespace std;

Path generate_random_path(vector<Node>& node_list, unsigned int seed) {
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

vector<Path> generate_population(vector<Node>& node_list, int size) {
    vector<Path> population;
    
    for (int i = 0; i < size; i++) 
        population.push_back(generate_random_path(node_list, i));
        
    return population;
}

double fitness_evaluation(Path path) {
    return 1.0 / path.distance;
}

vector<Node> tournament_selection(const vector<vector<Node>>& population, const vector<double>& fitnesses, int tournament_size) {
    int best_index = -1;
    double best_fitness = -1.0;

    for(int i =0; i < tournament_size; i++) {
        int index = rand() % population.size();
        if(fitnesses[index] > best_fitness) {
            best_fitness = fitnesses[index];
            best_index = index;
        }
    }

    return population[best_index];
}

vector<int> ox_crossover(const vector<int>& parent1, const vector<int>& parent2){
    int size = parent1.size();

    int cut_point1 = rand() % size;
    int cut_point2 = rand() % (size - cut_point1);

    cout << "Cut point 1: " << cut_point1 << endl;
    cout << "Cut point 2: " << cut_point2 << endl;

    vector<int> child1(size, -1);
    vector<int> child2(size, -1);

    for(int i = cut_point1; i < cut_point2; i++){
        child1[i] = parent1[i];
        child2[i] = parent2[i];
    }

    int pointer1 = cut_point2;
    for(int i = 0; i < size; i++){
        if((find(child1.begin(), child1.end(), parent2[i]) == child1.end())){
            if(pointer1 >= size){
                pointer1 = 0;
            }
            child1[pointer1] = parent2[i];
            pointer1++;
        }
    }

    int pointer2 = cut_point2;
    for(int i = 0; i < size; i++){
        if((find(child2.begin(), child2.end(), parent1[i]) == child2.end())){
            if(pointer2 >= size){
                pointer2 = 0;
            }
            child2[pointer1] = parent1[i];
            pointer2++;
        }
    }

    return child1, child2;
}