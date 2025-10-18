#include <algorithm>
#include <queue>

#include "ga/GeneticAlgorithm.h"
#include "ga/Population.h"
#include "ga/Operators.h"
#include "parallel/ThreadPool.h"
#include "tsp/Tsp.h"

using namespace std;

Path genetic_algorithm(
    vector<Node>& node_list,
    GAParameters& params,
    mt19937& gen,
    uniform_real_distribution<>& prob_dist
) {
    vector<Path> population = generate_population(node_list, gen);

    // cout << "Initial population generated:\n";
    // double average = get_average_distance(population);
    // cout << fixed << setprecision(15) << "Average distance: " << average << "\n";

    int generation = 0;

    while (generation != params.max_generations) {
        vector<Path> new_population;
        new_population.reserve(params.population_size);

        vector<int> elite_indexes = get_elite_indexes(population, params.elite_size);
        for (int idx : elite_indexes)
            new_population.push_back(population[idx]);

        while (new_population.size() < params.population_size) {
            auto [parent1, parent2] = tournament_selection(population, gen, params.tournament_size);

            auto [offspring1, offspring2] =
                order_crossover(parent1.node_sequence, parent2.node_sequence, gen);

            if (prob_dist(gen) < params.mutation_rate) swap_mutation(offspring1, gen);
            if (prob_dist(gen) < params.mutation_rate) swap_mutation(offspring2, gen);

            if (prob_dist(gen) < params.local_opt_rate) offspring1 = two_opt(offspring1);
            if (prob_dist(gen) < params.local_opt_rate) offspring2 = two_opt(offspring2);

            new_population.push_back(offspring1);
            if (new_population.size() < params.population_size)
                new_population.push_back(offspring2);
        }

        population = move(new_population);
        
        // cout << generation + 1 << "º Generation average: ";
        // average = get_average_distance(population);
        // cout << fixed << setprecision(15) << average << "\n";

        generation++;
    }

    return get_best_path(population);
}

Path parallel_genetic_algorithm(
    vector<Node>& node_list,
    GAParameters& params,
    mt19937& gen,
    int num_threads
) {
    vector<Path> population = generate_population(node_list, gen);

    // cout << "Initial population generated:\n";
    // double average = get_average_distance(population);
    // cout << fixed << setprecision(15) << "Average distance: " << average << "\n";

    int generation = 0;

    ThreadPool pool(params.population_size - params.elite_size, num_threads, gen());
    
    while (generation != params.max_generations) {
        vector<Path> new_population;
        new_population.reserve(params.population_size);
        
        vector<int> elite_indexes = get_elite_indexes(population, params.elite_size);
        for (int idx : elite_indexes)
        new_population.push_back(population[idx]);
        
        vector<future<vector<Path>>> futures;
        futures.reserve(num_threads);

        for (int t = 0; t < num_threads; t++) {
            futures.push_back(pool.enqueue([&](mt19937& generator, int start, int end) {
                int segment_size = end - start;
                uniform_real_distribution<double> prob_dist(0.0, 1.0);
                vector<Path> partial_population;
                partial_population.reserve(segment_size);
    
                while ((int)partial_population.size() < segment_size) {
                        auto [parent1, parent2] = tournament_selection(population, generator, params.tournament_size);

                        auto [offspring1, offspring2] =
                            order_crossover(parent1.node_sequence, parent2.node_sequence, generator);

                        if (prob_dist(generator) < params.mutation_rate) swap_mutation(offspring1, generator);
                        if (prob_dist(generator) < params.mutation_rate) swap_mutation(offspring2, generator);

                        if (prob_dist(generator) < params.local_opt_rate) offspring1 = two_opt(offspring1);
                        if (prob_dist(generator) < params.local_opt_rate) offspring2 = two_opt(offspring2);

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
        
        // cout << generation + 1 << "º Generation average: ";
        // average = get_average_distance(population);
        // cout << fixed << setprecision(15) << average << "\n";

        generation++;
    }

    return get_best_path(population);
}
