#include <chrono>
#include <iomanip>

#include "tsp.h"
#include "genetic.h"
#include "parameters.h"
#include <random>

using namespace std;

int main() {
     string file_path = "instances/fnl4461.tsp";
     vector<Node> node_list = tsp_to_vector(file_path);

     std::random_device rd;
     std::mt19937 gen(rd());
     uniform_real_distribution<> prob_dist(0.0, 1.0);

     vector<Path> population = generate_population(node_list);

     cout << "Initial population generated:\n";
     double average = 0.0;
     for (auto path : population) 
          average += path.distance;
     average /= POPULATION_SIZE;
     cout << fixed << setprecision(15) << "Average distance: " << average << "\n";

     int generation = 0;

     while (generation != MAX_GENERATIONS) {
          auto [parent1, parent2] = tournament_selection(population, SEED);

          auto [offspring1, offspring2] = pmx_crossover(parent1.node_sequence, parent2.node_sequence);

          if (prob_dist(gen) < MUTATION_RATE) swap_mutation(offspring1);
          if (prob_dist(gen) < MUTATION_RATE) swap_mutation(offspring2);

          auto [worst_idx, second_worst_idx] = find_two_worst_indexes(population);
          if (offspring1.fitness > population[worst_idx].fitness)
               population[worst_idx] = offspring1;
          if (offspring1.fitness > population[second_worst_idx].fitness)
               population[second_worst_idx] = offspring2;

          cout << generation + 1 << "º Generation average: ";
          average = 0.0;
          for (auto path : population) 
               average += path.distance;
          average /= POPULATION_SIZE;
          cout << fixed << setprecision(15) << average << "\n";

          generation++;
     }

    return 0;
}
