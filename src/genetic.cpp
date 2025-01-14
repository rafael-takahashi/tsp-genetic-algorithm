#include "genetic.h"

#include "parameters.h"
#include "path.h"
#include "tsp.h"

#include <algorithm>
#include <random>

using namespace std;

Path generate_random_path(const vector<Node>& node_list, int seed) {
    vector<Node> node_sequence;
    double res_distance = 0;
    vector<int> unvisited;
    for (long unsigned int i = 0; i < node_list.size(); i++)
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

    return Path(res_distance, node_sequence, calculate_fitness(res_distance));
}

vector<Path> generate_population(const vector<Node>& node_list) {
    vector<Path> population;
    
    for (int i = 0; i < POPULATION_SIZE; i++) 
        population.push_back(generate_random_path(node_list, SEED + i));
        
    return population;
}

pair<Path, Path> tournament_selection(vector<Path>& population, int seed) {
    mt19937 gen(seed);

    if (TOURNAMENT_SIZE > int(population.size()))
        throw invalid_argument("Tournament size cannot be larger than population size");

    int parent1_idx = -1;
    int parent2_idx = -1;
    double parent1_fit = -numeric_limits<double>::infinity();
    double parent2_fit = -numeric_limits<double>::infinity();
    
    for (int i = 0; i < TOURNAMENT_SIZE; i++) {
        int index = (gen() + i) % int(population.size());
        if (population[index].fitness > parent1_fit) {
            parent2_fit = parent1_fit;
            parent2_idx = parent1_idx;
            parent1_fit = population[index].fitness;
            parent1_idx = index;
        } else if (population[index].fitness > parent2_fit) {
            parent2_fit = population[index].fitness;
            parent2_idx = index;
        }
    }

    if (parent1_idx != -1 && parent2_idx != -1)
        return make_pair(population[parent1_idx], population[parent2_idx]);
    else
        throw runtime_error("Unable to select two parents");
}

pair<Path, Path> ox_crossover(vector<Node>& parent1_seq, vector<Node>& parent2_seq){
    int size = parent1_seq.size();

    int cut_point1 = rand() % (size/2);
    int cut_point2 = cut_point1 + (rand() % (size/2));

    if (cut_point1 > cut_point2) 
        swap(cut_point1, cut_point2);

    vector<Node> offspring1_seq(size);
    vector<Node> offspring2_seq(size);

    for (int i = cut_point1; i < cut_point2; i++) {
        offspring1_seq[i] = parent1_seq[i];
        offspring2_seq[i] = parent2_seq[i];
    }

    int pointer1 = cut_point2;
    int i = 0;

    while (i < size) {
        bool found = false;
        int node_id = parent2_seq[i].id;

        int j = 0;
        while (j < size && !found) {
            if (offspring1_seq[j].id == node_id)
                found = true;
            j++;
        }

        if (!found) {
            if (pointer1 >= size)
                pointer1 = 0;
            offspring1_seq[pointer1] = parent2_seq[i];
            pointer1++;
        }
        i++;
    }

    int pointer2 = cut_point1;
    i = 0;
    while (i < size) {
        bool found = false;
        int node_id = parent1_seq[i].id;

        int j = 0;
        while (j < size && !found) {
            if (offspring2_seq[j].id == node_id)
                found = true;
            j++;
        }

        if (!found) {
            if (pointer2 >= size)
                pointer2 = 0;
            offspring2_seq[pointer2] = parent1_seq[i];
            pointer2++;
        }
        i++;
    }

    Path offspring1 = create_path_by_sequence(offspring1_seq);
    Path offspring2 = create_path_by_sequence(offspring2_seq);

    return make_pair(offspring1, offspring2);
}

pair<Path, Path> pmx_crossover(vector<Node>& parent1_seq, vector<Node>& parent2_seq) {
    int size = parent1_seq.size();

    int cut_point1 = rand() % size;
    int cut_point2 = rand() % (size - cut_point1);

    vector<Node> offspring1_seq(size);
    vector<Node> offspring2_seq(size);

    for (int i = cut_point1; i < cut_point2; i++) {
        offspring1_seq[i] = parent1_seq[i];
        offspring2_seq[i] = parent2_seq[i]; 
    }
    
    for (int i = 0; i < size; i++) {
        if (i < cut_point1 || i >= cut_point2) {
            /*
                If child 1|2 already has the node, find the node that maps to this position in parent 2|1
                Else just copy the node from parent 2|1
            */
            if (find(offspring1_seq.begin(), offspring1_seq.end(), parent2_seq[i].id) != offspring1_seq.end()) {
                int j = i;
                // Find the node that maps to this position in parent 2
                while (find(offspring1_seq.begin(), offspring1_seq.end(), parent2_seq[j]) != offspring1_seq.end())
                    j = find(parent1_seq.begin(), parent1_seq.end(), parent2_seq[j]) - parent1_seq.begin();
                
                offspring1_seq[i] = parent2_seq[j];
            } 
            else offspring1_seq[i] = parent2_seq[i];
            
            if (find(offspring2_seq.begin(), offspring2_seq.end(), parent1_seq[i].id) != offspring2_seq.end()) {
                int j = i;

                while (find(offspring2_seq.begin(), offspring2_seq.end(), parent1_seq[j]) != offspring2_seq.end())
                    j = find(parent2_seq.begin(), parent2_seq.end(), parent1_seq[j]) - parent2_seq.begin();
                
                offspring2_seq[i] = parent1_seq[j];
            }
            else offspring2_seq[i] = parent1_seq[i];
        }
    }

    Path offspring1 = create_path_by_sequence(offspring1_seq);
    Path offspring2 = create_path_by_sequence(offspring2_seq);

    return make_pair(offspring1, offspring2);
}

void swap_mutation(Path& path) {
    int size = path.size;

    int i = rand() % size;
    int j = rand() % size;

    while (i == j) 
            j = rand() % size;
    

    swap(path.node_sequence[i], path.node_sequence[j]);

    path.distance += calculate_swap_delta(i, j, path.node_sequence, size);
    path.fitness = calculate_fitness(path.distance);
}

pair<int, int> find_two_worst_indexes(vector<Path>& population) {
    int worst_idx = -1;
    int second_worst_idx = -1;
    double worst_fitness = numeric_limits<double>::infinity();
    double second_worst_fitness = numeric_limits<double>::infinity();

    for (long unsigned int i = 0; i < population.size(); i++) {
        if (population[i].fitness < worst_fitness) {
            second_worst_fitness = worst_fitness;
            second_worst_idx = worst_idx;
            worst_fitness = population[i].fitness;
            worst_idx = i;
        } else if (population[i].fitness < second_worst_fitness) {
            second_worst_fitness = population[i].fitness;
            second_worst_idx = i;
        }
    }

    return make_pair(worst_idx, second_worst_idx);
}