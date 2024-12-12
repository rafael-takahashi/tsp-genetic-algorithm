#include <chrono>
#include <iomanip>
#include "tsp.h"

using namespace std;

int main() {
    vector<string> instances_file_paths = {
        "instances/pcb1173.tsp",
        "instances/pr1002.tsp",
        "instances/brd14051.tsp",
        "instances/fnl4461.tsp",
        "instances/d15112.tsp",
        "instances/pla33810.tsp",
        "instances/pla85900.tsp"
    };

    for (size_t i = 0; i < instances_file_paths.size(); i++) {
        string file_path = instances_file_paths[i];
        cout << "--------------------------------\nFile path: " << file_path << "\n";

        vector<Node> node_list = tsp_to_vector(file_path);

        auto start = chrono::high_resolution_clock::now();
        Path nearest_neighbor_path = nearest_neighbor(node_list);
        auto elapsed = chrono::high_resolution_clock::now();

        cout << "\nNearest neighbor traveled distance: " 
             << fixed << setprecision(15) 
             << nearest_neighbor_path.distance << "\n";
        cout << "Nearest neighbor execution time: " 
             << chrono::duration_cast<chrono::milliseconds>(elapsed - start).count() 
             << "ms\n";

        start = chrono::high_resolution_clock::now();
        Path two_opt_path = two_opt(nearest_neighbor_path);
        elapsed = chrono::high_resolution_clock::now();

        cout << "\n2-opt traveled distance: " 
             << fixed << setprecision(15) 
             << two_opt_path.distance << '\n';
        cout << "Nearest neighbor execution time: " 
             << chrono::duration_cast<chrono::milliseconds>(elapsed - start).count() 
             << "ms\n";
    }
    cout << "--------------------------------\n";
    return 0; 
}
