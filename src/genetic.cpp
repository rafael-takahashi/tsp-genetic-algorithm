#include "genetic.h"
#include "parameters.h"
#include "path.h"

#include <random>
#include <algorithm>

using namespace std;

Path generate_random_path(vector<Node>& node_list, int seed) {
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

vector<Path> generate_population(vector<Node>& node_list) {
    vector<Path> population;
    
    for (int i = 0; i < POPULATION_SIZE; i++) 
        population.push_back(generate_random_path(node_list, SEED + i));
        
    return population;
}

pair<Path, Path> tournament_selection(vector<Path>& population, long unsigned int tournament_size, int seed) {
    mt19937 gen(seed);

    if (tournament_size > population.size())
        throw invalid_argument("Tournament size cannot be larger than population size");

    int parent1_idx = -1;
    int parent2_idx = -1;
    double parent1_fit = -numeric_limits<double>::infinity();
    double parent2_fit = -numeric_limits<double>::infinity();
    
    for (long unsigned int i = 0; i < tournament_size; i++) {
        int index = (gen() + i) % population.size();
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

    cout << parent1_idx << '\n';
    cout << parent2_idx << '\n';

    if (parent1_idx != -1 && parent2_idx != -1)
        return make_pair(population[parent1_idx], population[parent2_idx]);
    else
        throw runtime_error("Unable to select two parents");
}

//TODO: utilize make_pair to optimally return two chromosomes
vector<Path> ox_crossover(const vector<Path>& parents){
    vector<Path> childrens;

    int size = parents[0].size;

    int cut_point1 = rand() % (size/2);
    int cut_point2 = cut_point1 + (rand() % (size/2));

    if (cut_point1 > cut_point2) {
        swap(cut_point1, cut_point2);
    }

    cout << "Cut point 1: " << cut_point1 << endl;
    cout << "Cut point 2: " << cut_point2 << endl;

    vector<Node> child1_sequence;
    vector<Node> child2_sequence;

    for(int i = cut_point1; i < cut_point2; i++){
        child1_sequence[i] = parents[0].node_sequence[i];
        child2_sequence[i] = parents[1].node_sequence[i];
    }

    int pointer1 = cut_point2;
    int i = 0;
    while (i < size) {
        bool found = false;
        int node_id = parents[1].node_sequence[i].id;

        int j = 0;
        while (j < size && !found) {
            if (child1_sequence[j].id == node_id) {
                found = true;
            }
            j++;
        }

        if (!found) {
            if (pointer1 >= size) {
                pointer1 = 0;
            }
            child1_sequence[pointer1] = parents[1].node_sequence[i];
            pointer1++;
        }

        i++;
    }

    int pointer2 = cut_point1;
    i = 0;
    while (i < size) {
        bool found = false;
        int node_id = parents[0].node_sequence[i].id;

        int j = 0;
        while (j < size && !found) {
            if (child2_sequence[j].id == node_id) {
                found = true;
            }
            j++;
        }

        if (!found) {
            if (pointer2 >= size) {
                pointer2 = 0;
            }
            child2_sequence[pointer2] = parents[0].node_sequence[i];
            pointer2++;
        }

        i++;
    }

    Path child1 = create_path_by_sequence(child1_sequence);
    Path child2 = create_path_by_sequence(child2_sequence);

    childrens.push_back(child1);
    childrens.push_back(child2);

    return childrens;
}

//TODO: utilize make_pair to optimally return two chromosomes
vector<Path> pmx_crossover(
    vector<Node>& parent1_seq, vector<Node>& parent2_seq
) {
    vector<Path> children;

    int size = parent1_seq.size();

    int cut_point1 = rand() % size;
    int cut_point2 = rand() % (size - cut_point1);

    vector<Node> child1(size);
    vector<Node> child2(size);

    for (int i = cut_point1; i < cut_point2; i++) {
        child1[i] = parent1_seq[i];
        child2[i] = parent2_seq[i]; 
    }
    
    for (int i = 0; i < size; i++) {
        if (i < cut_point1 || i >= cut_point2) {
            /*
                If child 1|2 already has the node, find the node that maps to this position in parent 2|1
                Else just copy the node from parent 2|1
            */
            if (find(child1.begin(), child1.end(), parent2_seq[i].id) != child1.end()) {
                int j = i;
                // Find the node that maps to this position in parent 2
                while (find(child1.begin(), child1.end(), parent2_seq[j]) != child1.end())
                    j = find(parent1_seq.begin(), parent1_seq.end(), parent2_seq[j]) - parent1_seq.begin();
                child1[i] = parent2_seq[j];
            } else 
                child1[i] = parent2_seq[i];
            

            if (find(child2.begin(), child2.end(), parent1_seq[i].id) != child2.end()) {
                int j = i;
                while (find(child2.begin(), child2.end(), parent1_seq[j]) != child2.end())
                    j = find(parent2_seq.begin(), parent2_seq.end(), parent1_seq[j]) - parent2_seq.begin();
                child2[i] = parent1_seq[j];
            } else 
                child2[i] = parent1_seq[i];
        }
    }

    Path child1_path = create_path_by_sequence(child1);
    Path child2_path = create_path_by_sequence(child2);

    children.push_back(child1_path);
    children.push_back(child2_path);

    return children;
}