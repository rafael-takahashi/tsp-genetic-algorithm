#include <chrono>
#include <iomanip>

#include "tsp.h"

using namespace std;

int main() {
     char repeat;
     do {
          string file_name; 
          cout << "Enter file name (e.g: a280.tsp): "; 
          cin >> file_name;

          string file_path = "instances/" + file_name;
          cout << "--------------------------------\nFile path: " << file_path << "\n";

          vector<Node> node_list = tsp_to_vector(file_path);
          Path constructed_path;

          int choice;
          bool valid_choice;

          do {
               cout << "Choose constructive heuristic:\n1. Nearest Neighbor\n2. Farthest Insertion\n";
               cin >> choice;

               if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter 1 or 2.\n";
                    continue;
               }

               valid_choice = true;

               if (choice == 1) {
                    auto nn_start = chrono::high_resolution_clock::now();
                    constructed_path = nearest_neighbor(node_list);
                    auto nn_elapsed = chrono::high_resolution_clock::now();
                    cout << "--------------------------------\n";
                    cout << "Nearest neighbor traveled distance: " 
                         << fixed << setprecision(15) 
                         << constructed_path.distance << "\n";
                    cout << "Nearest neighbor execution time: " 
                         << chrono::duration_cast<chrono::milliseconds>(nn_elapsed - nn_start).count() 
                         << "ms\n";
                    cout << "--------------------------------\n";
               } else if (choice == 2) {
                    auto fi_start = chrono::high_resolution_clock::now();
                    constructed_path = farthest_insertion(node_list);
                    auto fi_elapsed = chrono::high_resolution_clock::now();
                    cout << "--------------------------------\n";
                    cout << "Farthest insertion traveled distance: " 
                         << fixed << setprecision(15) 
                         << constructed_path.distance << "\n";
                    cout << "Farthest insertion execution time: " 
                         << chrono::duration_cast<chrono::milliseconds>(fi_elapsed - fi_start).count() 
                         << "ms\n";
                    cout << "--------------------------------\n";
               } else {
                    cout << "Invalid input. Please enter 1 or 2.\n";
                    valid_choice = false;
               }
          } while (!valid_choice);

          do {
               cout << "Choose local search heuristic:\n1. Pair Swap\n2. 2-opt\n";
               cin >> choice;

               if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter 1 or 2.\n";
                    continue;
               }

               valid_choice = true;

               if (choice == 1) {
                    auto ps_start = chrono::high_resolution_clock::now();
                    Path pair_swap_path = pair_swap(constructed_path);
                    auto ps_elapsed = chrono::high_resolution_clock::now();
                    cout << "--------------------------------\n";
                    cout << "Pair Swap traveled distance: " 
                         << fixed << setprecision(15) 
                         << pair_swap_path.distance << "\n";
                    cout << "Pair Swap execution time: " 
                         << chrono::duration_cast<chrono::milliseconds>(ps_elapsed - ps_start).count() 
                         << "ms\n";
                    cout << "--------------------------------\n";
               } else if (choice == 2) {
                    auto two_opt_start = chrono::high_resolution_clock::now();
                    Path two_opt_path = two_opt(constructed_path);
                    auto two_opt_elapsed = chrono::high_resolution_clock::now();
                    cout << "--------------------------------\n";
                    cout << "2-opt traveled distance: " 
                         << fixed << setprecision(15) 
                         << two_opt_path.distance << '\n';
                    cout << "2-opt execution time: " 
                         << chrono::duration_cast<chrono::milliseconds>(two_opt_elapsed - two_opt_start).count() 
                         << "ms\n";
                    cout << "--------------------------------\n";
               } else {
                    cout << "Invalid input. Please enter 1 or 2.\n";
                    valid_choice = false;
               }
          } while (!valid_choice);
          cout << "Do you wish to run another instance? (y/n): ";
          cin >> repeat;
     } while (repeat == 'y' || repeat == 'Y');
     
    return 0;
}
