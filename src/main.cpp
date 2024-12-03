#include "tsp.h"

using namespace std;

int main() {
    const string file_path = "instances/a280.tsp";
    vector<Node> node_list = tsp_to_vector(file_path);

    Path nearest_neighbor_sequence = nearest_neighbor(node_list);
    cout << "Nearest neighbor traveled distance: " << nearest_neighbor_sequence.distance << '\n';

    Path two_opt_sequence = two_opt(nearest_neighbor_sequence);
    cout << "2-opt traveled distance: " << two_opt_sequence.distance << '\n';

    return 0; 
}