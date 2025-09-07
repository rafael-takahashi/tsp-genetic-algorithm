#include <chrono>
#include <fstream>
#include <iomanip>

#include "tsp.h"
#include "genetic.h"
#include "parameters.h"

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
     int iterations_without_improvement = 0;
     double new_average = 0.0;

     while (generation != MAX_GENERATIONS) {
          vector<Path> new_population;
          new_population.reserve(POPULATION_SIZE);

          vector<int> elite_indexes = get_elite_indexes(population);
          for (int idx : elite_indexes)
               new_population.push_back(population[idx]);

          double average_fitness = calculate_fitness(average);

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

     auto elapsed = chrono::high_resolution_clock::now();

     cout << "Distance: " << best_path.distance << "\n";
     cout << "Fitness: " << best_path.fitness << "\n\n";
     cout << "Execution time: "
     << chrono::duration_cast<chrono::milliseconds>(elapsed - start).count() 
     << " ms\n";

    return 0;
}
