#include <chrono>
#include <fstream>
#include <iomanip>

#include "tsp.h"
#include "genetic.h"
#include "parameters.h"
#include "ThreadPool.h"

using namespace std;

int main() {
     string file_path = "instances/u574.tsp";

     vector<Node> node_list = tsp_to_vector(file_path);

     mt19937 gen(SEED);
     uniform_real_distribution<> prob_dist(0.0, 1.0);

     auto start = chrono::high_resolution_clock::now();
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
     
     int total_to_generate = POPULATION_SIZE - ELITE_SIZE;
     int segment_size = (total_to_generate) / MAX_THREADS;
     int remainder = (total_to_generate) % MAX_THREADS;

     while (generation != MAX_GENERATIONS) {
          vector<Path> new_population;
          new_population.reserve(POPULATION_SIZE);
          
          vector<int> elite_indexes = get_elite_indexes(population);
          for (int idx : elite_indexes)
          new_population.push_back(population[idx]);
          
          vector<future<vector<Path>>> futures;
          futures.reserve(MAX_THREADS);

          for (int t = 0; t < MAX_THREADS; t++) {
               int this_segment = segment_size + (t < remainder ? 1 : 0);

               futures.push_back(move(pool.enqueue([&, this_segment](mt19937& rng) {
                    vector<Path> partial_population;
                    partial_population.reserve(this_segment);

                    uniform_real_distribution<double> prob_dist(0.0, 1.0);

                    while ((int)partial_population.size() < this_segment) {
                         auto [parent1, parent2] = tournament_selection(population, rng);

                         auto [offspring1, offspring2] =
                              order_crossover(parent1.node_sequence, parent2.node_sequence, rng);

                         if (prob_dist(rng) < MUTATION_RATE) swap_mutation(offspring1, rng);
                         if (prob_dist(rng) < MUTATION_RATE) swap_mutation(offspring2, rng);

                         if (prob_dist(rng) < LOCAL_OPT_RATE) offspring1 = two_opt(offspring1);
                         if (prob_dist(rng) < LOCAL_OPT_RATE) offspring2 = two_opt(offspring2);

                         partial_population.push_back(move(offspring1));
                         if ((int)partial_population.size() < this_segment)
                              partial_population.push_back(move(offspring2));
                    }

                    return partial_population;
               })));
          }

          for (auto& fut : futures) {
               auto part = fut.get();
               new_population.insert(new_population.end(),
                                   make_move_iterator(part.begin()),
                                   make_move_iterator(part.end()));
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

     auto elapsed = chrono::high_resolution_clock::now();

     cout << "Distance: " << best_path.distance << "\n";
     cout << "Fitness: " << best_path.fitness << "\n\n";
     cout << "Execution time: "
     << chrono::duration_cast<chrono::milliseconds>(elapsed - start).count() 
     << " ms\n";

    return 0;
}
