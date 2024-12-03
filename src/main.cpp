#include "tsp.h"

using namespace std;

int main() {
    const string file_path = "instances/a280.tsp";
    vector<Node> node_list = tsp_to_vector(file_path);

    TspResult node_sequence = nearest_neighbor(node_list);
    cout << "Nearest neighbor traveled distance: " << node_sequence.traveled_distance << '\n';

    TspResult res = two_opt(node_sequence.node_sequence);
    cout << "2-opt traveled distance: " << res.traveled_distance << '\n';

    return 0; 
}