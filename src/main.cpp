#include "tsp.h"

using namespace std;

int main() {
    const string file_path = "instances/pr1002.tsp";
    vector<Node> node_list = tsp_to_vector(file_path);

    Path nearest_neighbor_path = nearest_neighbor(node_list);
    /*
    for (Node node : nearest_neighbor_path.node_sequence)
        node.print();
    */
   cout << "Nearest neighbor traveled distance: " << nearest_neighbor_path.distance << "\n\n";

    Path two_opt_path = two_opt(nearest_neighbor_path);
    /*
    for (Node node : two_opt_path.node_sequence)
        node.print();
    */
    cout << "2-opt traveled distance: " << two_opt_path.distance << "\n\n";

    return 0; 
}