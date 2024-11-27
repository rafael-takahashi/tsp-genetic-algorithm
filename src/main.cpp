#include "tsp.h"

using namespace std;

int main() {
    const string file_path = "instances/kroA100.tsp";
    vector<Node> node_list = tsp_to_vector(file_path);

    TspResult res = nearest_neighbor(node_list);

    cout << "Traveled distance: " << res.traveled_distance << '\n';

    for (Node node : res.node_sequence)
        node.print();

    return 0; 
}