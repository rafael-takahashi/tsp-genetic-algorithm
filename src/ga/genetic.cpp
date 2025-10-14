#include "ga/genetic.h"
#include "ga/parameters.h"
#include "ga/path.h"
#include "tsp/tsp.h"
#include "parallel/ThreadPool.h"

#include <algorithm>
#include <random>
#include <queue>

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
    vector<Path> population(POPULATION_SIZE);
    
    for (size_t i = 0; i < POPULATION_SIZE; i++) 
        population[i] = generate_random_path(node_list, gen);
        
    return population;
}

pair<Path, Path> tournament_selection(vector<Path>& population, mt19937& gen) {
    int parent1_idx = -1;
    int parent2_idx = -1;
    double parent1_fit = -numeric_limits<double>::infinity();
    double parent2_fit = -numeric_limits<double>::infinity();
    
    for (size_t i = 0; i < TOURNAMENT_SIZE; i++) {
        int index = gen() % int(population.size());
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

pair<Path, Path> order_crossover(vector<Node>& parent1_seq, vector<Node>& parent2_seq, mt19937& gen) {
    int size = parent1_seq.size();

    int cut_point1 = gen() % (size/2);
    uniform_int_distribution<int> dist_cut2(cut_point1, size - 1);
    int cut_point2 = dist_cut2(gen);

    if (cut_point1 > cut_point2) 
        swap(cut_point1, cut_point2);

    Node sentinel_node;
    sentinel_node.id = -1;

    vector<Node> offspring1_seq(size, sentinel_node);
    vector<Node> offspring2_seq(size, sentinel_node);

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

    int pointer2 = cut_point2; 
    i = 0;
    while (i < size) {
        bool found = false;
        int node_id = parent1_seq[i].id;
        int j = 0;
        while (j < size && !found) {
            if (offspring2_seq[j].id == node_id) {
                found = true;
            }
            j++;
        }

        if (!found) {
            pointer2 = pointer2 % size;
            while (offspring2_seq[pointer2].id != -1) {
                pointer2 = (pointer2 + 1) % size;
            }
            offspring2_seq[pointer2] = parent1_seq[i];
            pointer2++;
        }
        i++;
    }

    Path offspring1 = create_path_by_sequence(offspring1_seq);
    Path offspring2 = create_path_by_sequence(offspring2_seq);

    return make_pair(offspring1, offspring2);
}

pair<Path, Path> partially_mapped_crossover(vector<Node>& parent1_seq, vector<Node>& parent2_seq, mt19937& gen) {
    int size = parent1_seq.size();

    int cut_point1 = gen() % size;
    int cut_point2 = cut_point1 + (gen() % (size - cut_point1));

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

void swap_mutation(Path& path, mt19937& gen) {
    int size = path.size;

    int i = 1 + gen() % (size - 2);
    int j = i + (gen() % (size - i));

    if (i == j || j == size - 1) return;

    path.distance += calculate_swap_delta(i, j, path.node_sequence, size);
    path.fitness = calculate_fitness(path.distance);
    
    swap(path.node_sequence[i], path.node_sequence[j]);
}

struct CompareFitness {
    bool operator()(const pair<int, double>& a, const pair<int, double>& b) {
        return a.second < b.second;
    }
};

vector<int> get_elite_indexes(const vector<Path>& population) {
    priority_queue<pair<int, double>,
                    vector<pair<int, double>>,
                    CompareFitness> heap;

    for (size_t i = 0; i < population.size(); i++) {
        double fit = population[i].fitness;
        if (heap.size() < ELITE_SIZE)
            heap.push({static_cast<int>(i), fit});
        else if (fit < heap.top().second) {
            heap.pop();
            heap.push({static_cast<int>(i), fit});
        }
    }

    vector<int> elite_indexes;
    elite_indexes.reserve(ELITE_SIZE);
    while (!heap.empty()) {
        elite_indexes.push_back(heap.top().first);
        heap.pop();
    }

    return elite_indexes;
}

int genetic_algorithm(std::string file_path) {
    vector<Node> node_list = tsp_to_vector(file_path);

    mt19937 gen(SEED);
    uniform_real_distribution<> prob_dist(0.0, 1.0);

    vector<Path> population = generate_population(node_list, gen);

    cout << "Initial population generated:\n";
    double average = 0.0;
    for (auto path : population) 
        average += path.distance;
    average /= POPULATION_SIZE;
    cout << fixed << setprecision(15) << "Average distance: " << average << "\n";

    int generation = 0;
    double new_average = 0.0;

    while (generation != MAX_GENERATIONS) {
        vector<Path> new_population;
        new_population.reserve(POPULATION_SIZE);

        vector<int> elite_indexes = get_elite_indexes(population);
        for (int idx : elite_indexes)
            new_population.push_back(population[idx]);

        while (new_population.size() < POPULATION_SIZE) {
            auto [parent1, parent2] = tournament_selection(population, gen);

            auto [offspring1, offspring2] =
                order_crossover(parent1.node_sequence, parent2.node_sequence, gen);

            if (prob_dist(gen) < MUTATION_RATE) swap_mutation(offspring1, gen);
            if (prob_dist(gen) < MUTATION_RATE) swap_mutation(offspring2, gen);

            if (prob_dist(gen) < LOCAL_OPT_RATE) offspring1 = two_opt(offspring1);
            if (prob_dist(gen) < LOCAL_OPT_RATE) offspring2 = two_opt(offspring2);

            new_population.push_back(offspring1);
            if (new_population.size() < POPULATION_SIZE)
                new_population.push_back(offspring2);
        }

        population = move(new_population);
        
        cout << generation + 1 << "º Generation average: ";
        new_average = 0.0;
        for (auto path : population) 
            new_average += path.distance;
        new_average /= POPULATION_SIZE;
        cout << fixed << setprecision(15) << new_average << "\n";

        generation++;
    }

    Path best_path = population[0];
    for (auto path : population)
        if (best_path.fitness < path.fitness) 
            best_path = path;

    return 0;
}

int parallel_genetic_algorithm(string file_path) {
    vector<Node> node_list = tsp_to_vector(file_path);

    mt19937 gen(SEED);
    uniform_real_distribution<> prob_dist(0.0, 1.0);

    vector<Path> population = generate_population(node_list, gen);

    cout << "Initial population generated:\n";
    double average = 0.0;
    for (auto path : population) 
        average += path.distance;
    average /= POPULATION_SIZE;
    cout << fixed << setprecision(15) << "Average distance: " << average << "\n";

    int generation = 0;
    double new_average = 0.0;

    ThreadPool pool;
    
    while (generation != MAX_GENERATIONS) {
        vector<Path> new_population;
        new_population.reserve(POPULATION_SIZE);
        
        vector<int> elite_indexes = get_elite_indexes(population);
        for (int idx : elite_indexes)
        new_population.push_back(population[idx]);
        
        vector<future<vector<Path>>> futures;
        futures.reserve(MAX_THREADS);

        for (int t = 0; t < MAX_THREADS; t++) {
            futures.push_back(pool.enqueue([&](mt19937& generator, int start, int end) {
                int segment_size = end - start;
                uniform_real_distribution<double> prob_dist(0.0, 1.0);
                vector<Path> partial_population;
                partial_population.reserve(segment_size);
    
                while ((int)partial_population.size() < segment_size) {
                        auto [parent1, parent2] = tournament_selection(population, generator);

                        auto [offspring1, offspring2] =
                            order_crossover(parent1.node_sequence, parent2.node_sequence, generator);

                        if (prob_dist(generator) < MUTATION_RATE) swap_mutation(offspring1, generator);
                        if (prob_dist(generator) < MUTATION_RATE) swap_mutation(offspring2, generator);

                        if (prob_dist(generator) < LOCAL_OPT_RATE) offspring1 = two_opt(offspring1);
                        if (prob_dist(generator) < LOCAL_OPT_RATE) offspring2 = two_opt(offspring2);

                        partial_population.push_back(move(offspring1));
                        if ((int)partial_population.size() < segment_size)
                            partial_population.push_back(move(offspring2));
                }

                return partial_population;
            }));
        }
        
        for (auto& fut : futures) {
            auto part = fut.get();
            new_population.insert(
                new_population.end(),
                make_move_iterator(part.begin()),
                make_move_iterator(part.end())
            );
        }

        population = move(new_population);
        
        cout << generation + 1 << "º Generation average: ";
        new_average = 0.0;
        for (auto path : population) 
            new_average += path.distance;
        new_average /= POPULATION_SIZE;
        cout << fixed << setprecision(15) << new_average << "\n";

        generation++;
    }

    Path best_path = population[0];
    for (auto path : population)
        if (best_path.fitness < path.fitness) 
            best_path = path;

    return 0;
}
