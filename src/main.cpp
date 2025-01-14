#include <chrono>
#include <iomanip>

#include "tsp.h"
#include "genetic.h"
#include "parameters.h"

using namespace std;

int main() {
     string file_path = "instances/u574.tsp";
     vector<Node> node_list = tsp_to_vector(file_path);

     vector<Path> population = generate_population(node_list);

     cout << "Initial population generated:\n";

     double average = 0.0;

     for (auto path : population) 
          average += path.distance;

     average /= POPULATION_SIZE;

     cout << fixed << setprecision(15) << "Average distance: " << average << "\n";

     vector<Path> offspring;
     vector<Path> new_population;
     int iterations = 0;
     bool significant_improvement = true;
     while (significant_improvement && iterations < 200) {
          for (int i = 0; i < POPULATION_SIZE - ELITISM_SIZE; i+=2) {     
               auto [parent1, parent2] = tournament_selection(population, TOURNAMENT_SIZE, SEED);

               auto [offspring1, offspring2] = i % 2 ?
                              ox_crossover(parent1.node_sequence, parent2.node_sequence) :
                              pmx_crossover(parent1.node_sequence, parent2.node_sequence);

               offspring.push_back(offspring1);
               offspring.push_back(offspring2);
          }

          new_population = elitism(population, offspring);
          population = new_population;
               
          double new_average = 0.0;

          for (auto path : new_population) 
               new_average += path.distance;

          new_average /= POPULATION_SIZE;

          if (new_average < average)
               average = new_average;
          else 
               significant_improvement = false;

          cout << fixed << setprecision(15) << iterations + 1 << "º Generation average distance: " << new_average << "\n";

          iterations++;
     }

     /*
     char repeat;
     do {
          string file_name; 
          cout << "Digite o nome do arquivo (e.g: a280.tsp): "; 
          cin >> file_name;

          string file_path = "instances/" + file_name;
          cout << "--------------------------------\nCaminho do arquivo: " << file_path << "\n";

          vector<Node> node_list = tsp_to_vector(file_path);
          Path constructed_path;

          int choice;
          bool valid_choice;

          do {
               cout << "Escolha o algoritmo construtivo:\n1. Vizinho mais próximo\n2. Inserção mais distante\n";
               cin >> choice;

               if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Entrada inválida. Digite 1 ou 2.\n";
                    continue;
               }

               valid_choice = true;

               if (choice == 1) {
                    auto nn_start = chrono::high_resolution_clock::now();
                    constructed_path = nearest_neighbor(node_list);
                    auto nn_elapsed = chrono::high_resolution_clock::now();
                    cout << "--------------------------------\n";
                    cout << "Vizinho mais próximo - Distância obtida: " 
                         << fixed << setprecision(15) 
                         << constructed_path.distance << "\n";
                    cout << "Vizinho mais próximo - Tempo de execução: " 
                         << chrono::duration_cast<chrono::milliseconds>(nn_elapsed - nn_start).count() 
                         << "ms\n";
                    cout << "--------------------------------\n";
               } else if (choice == 2) {
                    auto fi_start = chrono::high_resolution_clock::now();
                    constructed_path = farthest_insertion(node_list);
                    auto fi_elapsed = chrono::high_resolution_clock::now();
                    cout << "--------------------------------\n";
                    cout << "Inserção mais distante - Distância obtida: " 
                         << fixed << setprecision(15) 
                         << constructed_path.distance << "\n";
                    cout << "Inserção mais distante - Tempo de execução: " 
                         << chrono::duration_cast<chrono::milliseconds>(fi_elapsed - fi_start).count() 
                         << "ms\n";
                    cout << "--------------------------------\n";
               } else {
                    cout << "Entrada inválida. Digite 1 ou 2.\n";
                    valid_choice = false;
               }
          } while (!valid_choice);

          char repeat_local_search;
          do {
               do {
                    cout << "Escolha o algoritmo melhorativo:\n1. Pair Swap\n2. 2-opt\n";
                    cin >> choice;

                    if (cin.fail()) {
                         cin.clear();
                         cin.ignore(numeric_limits<streamsize>::max(), '\n');
                         cout << "Entrada inválida. Digite 1 ou 2.\n";
                         continue;
                    }

                    valid_choice = true;

                    if (choice == 1) {
                         auto ps_start = chrono::high_resolution_clock::now();
                         Path pair_swap_path = pair_swap(constructed_path);
                         auto ps_elapsed = chrono::high_resolution_clock::now();
                         cout << "--------------------------------\n";
                         cout << "Pair Swap - Distância obtida: " 
                              << fixed << setprecision(15) 
                              << pair_swap_path.distance << "\n";
                         cout << "Pair Swap - Tempo de execução: " 
                              << chrono::duration_cast<chrono::milliseconds>(ps_elapsed - ps_start).count() 
                              << "ms\n";
                         cout << "--------------------------------\n";
                    } else if (choice == 2) {
                         auto two_opt_start = chrono::high_resolution_clock::now();
                         Path two_opt_path = two_opt(constructed_path);
                         auto two_opt_elapsed = chrono::high_resolution_clock::now();
                         cout << "--------------------------------\n";
                         cout << "2-opt - Distância obtida: " 
                              << fixed << setprecision(15) 
                              << two_opt_path.distance << '\n';
                         cout << "2-opt - Tempo de execução: " 
                              << chrono::duration_cast<chrono::milliseconds>(two_opt_elapsed - two_opt_start).count() 
                              << "ms\n";
                         cout << "--------------------------------\n";
                    } else {
                         cout << "Entrada inválida. Digite 1 ou 2.\n";
                         valid_choice = false;
                    }
               } while (!valid_choice);
               cout << "Deseja executar outro algoritmo melhorativo para este caminho? (y/n): ";
               cin >> repeat_local_search;
          } while (repeat_local_search == 'y' || repeat_local_search == 'Y') ;
          cout << "Deseja executar outra instância? (y/n): ";
          cin >> repeat;
     } while (repeat == 'y' || repeat == 'Y');
     */
    return 0;
}
