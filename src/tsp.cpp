#include "tsp.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>
#include <sstream>
#include "node.h"

using namespace std;

vector<Node> tsp_to_vector(string& file_path) {
    ifstream file(file_path);

    if (!file.is_open()) {
        cerr << "Failed to open file.\n";
        exit(1);
    }

    string line;
    vector<Node> node_list;
    
    while (getline(file, line) && line != "NODE_COORD_SECTION") continue;
    
    int id;
    double x, y;
    
    while (getline(file, line) && line != "EOF") {
        istringstream stream(line);
        string string_id, string_x, string_y;

        stream >> string_id >> string_x >> string_y;
        id = stoi(string_id);
        x = stod(string_x);
        y = stod(string_y); 

        node_list.emplace_back(id, x, y);
    }

    file.close();

    return node_list;
}

Path nearest_neighbor(const vector<Node>& node_list) {
    vector<Node> node_sequence;
    double res_distance = 0;
    vector<bool> visited(node_list.size(), false);

    int current = 0; // Change this to a random number to get a different starting point

    node_sequence.push_back(node_list[current]);

    visited[current] = true;

    while (node_sequence.size() < node_list.size()) {
        double min_distance = numeric_limits<double>::max();
        int next;

        for (size_t i = 0; i < node_list.size(); i++) {
            if (visited[i] == false) {
                double distance = calculate_distance(node_list[current], node_list[i]);
                if (distance < min_distance) {
                    min_distance = distance;
                    next = i;
                }
            }
        }

        node_sequence.push_back(node_list[next]);
        res_distance += min_distance;
        visited[next] = true;
        current = next;
    }

    return Path(res_distance, node_sequence);
}

void two_opt_swap(vector<Node>& sequence, unsigned int first, unsigned int second) {
    first++;
    while (first < second) {
        swap(sequence[first], sequence[second]);
        first++;
        second--;
    }
}

Path two_opt(const Path& path) {
    vector<Node> current_sequence = path.node_sequence;
    double current_distance = path.distance;
    bool has_improved;

    do {
        has_improved = false;
        for (size_t i = 0; i < path.size - 1; i++) {
            for (size_t j = i + 2; j < path.size; j++) {
                double delta = - calculate_distance(current_sequence[i], current_sequence[i + 1])
                               - calculate_distance(current_sequence[j], current_sequence[(j + 1) % path.size])
                               + calculate_distance(current_sequence[i], current_sequence[j])
                               + calculate_distance(current_sequence[i + 1], current_sequence[(j + 1) % path.size]);

                if (delta < 0 && delta < -1e-10) {
                    two_opt_swap(current_sequence, i, j);
                    current_distance += delta;
                    has_improved = true;
                }
            }
        }
    } while (has_improved);

    return Path(current_distance, current_sequence);
}
