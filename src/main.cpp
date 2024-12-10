#include "tsp.h"

using namespace std;

int main() {

    /*
        -gr666.tsp
        pcb1173.tsp
        pr1002.tsp
        brd14051.tsp
        fnl4461.tsp
        d15112.tsp
        pla33810.tsp
        pla85900.tsp
    */

    vector<string> instances_file_paths = {
        "instances/att532.tsp",
        "instances/pcb1173.tsp",
        "instances/pr1002.tsp",
        "instances/brd14051.tsp",
        "instances/fnl4461.tsp",
        "instances/d15112.tsp",
        "instances/pla33810.tsp",
        "instances/pla85900.tsp"
    };

    //for (size_t i = 0; i < instances_file_paths.size(); i++) {
        string file_path = "instances/brd14051.tsp";
        cout << "--------------------------------\nFile path: " << file_path << "\n";

        vector<Node> node_list = tsp_to_vector(file_path);

        Path nearest_neighbor_path = nearest_neighbor(node_list);
        cout << "\nNearest neighbor traveled distance: " << nearest_neighbor_path.distance << "\n";

        Path two_opt_path = two_opt(nearest_neighbor_path);
        cout << "\n2-opt traveled distance: " << two_opt_path.distance << "\n--------------------------------\n";
    //}
    
    return 0; 
}