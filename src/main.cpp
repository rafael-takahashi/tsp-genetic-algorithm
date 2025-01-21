#include <chrono>
#include <iomanip>

#include "tsp.h"
#include "genetic.h"
#include "parameters.h"
#include <random>

using namespace std;

int main() {
     string file_path = "instances/u574.tsp";
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
     int iterations_without_improvement = 0;

     while (iterations_without_improvement <= NO_IMPROVEMENT_TOLERANCE && generation != MAX_GENERATIONS) {
          auto [parent1, parent2] = tournament_selection(population, gen);

          auto [offspring1, offspring2] = partially_mapped_crossover(parent1.node_sequence, parent2.node_sequence, gen);

          if (prob_dist(gen) < MUTATION_RATE) swap_mutation(offspring1, gen);
          if (prob_dist(gen) < MUTATION_RATE) swap_mutation(offspring2, gen);

          offspring1 = two_opt(offspring1);
          offspring2 = two_opt(offspring2);

          auto [worst_idx, second_worst_idx] = find_two_worst_indexes(population);
          if (offspring1.fitness > population[worst_idx].fitness)
               population[worst_idx] = offspring1;
          if (offspring1.fitness > population[second_worst_idx].fitness)
               population[second_worst_idx] = offspring2;

          cout << generation + 1 << "º Generation average: ";
          double new_average = 0.0;
          for (auto path : population) 
               new_average += path.distance;
          new_average /= POPULATION_SIZE;
          cout << fixed << setprecision(15) << new_average << "\n";

          if (new_average < average) {
               average = new_average;
               iterations_without_improvement = 0;
          } else 
               iterations_without_improvement++;
               
          generation++;
     }

     Path best_path = population[0];

     for (auto path : population) {
          if (best_path.fitness < path.fitness) 
               best_path = path;
     }

     cout << "\nBest path found:\n";
     for (auto node : best_path.node_sequence) 
          cout << node.id << " ";
     cout << "\nDistance: " << best_path.distance << "\n";
     cout << "Fitness: " << best_path.fitness << "\n\n";

    return 0;
}
