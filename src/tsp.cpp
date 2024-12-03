#include "tsp.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>
#include <sstream>
#include "node.h"

using namespace std;

vector<Node> tsp_to_vector(const string& file_path) {
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

    int current = 0;

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

Path two_opt_swap(Path path, unsigned int first, unsigned int second) {
    double new_distance = path.distance;

    new_distance += - calculate_distance(path.node_sequence[first], path.node_sequence[first + 1])
                    - calculate_distance(path.node_sequence[second], path.node_sequence[(second + 1) % path.size])
                    + calculate_distance(path.node_sequence[first], path.node_sequence[second])
                    + calculate_distance(path.node_sequence[first + 1], path.node_sequence[(second + 1) % path.size]);

    vector<Node> new_sequence = path.node_sequence;

    reverse(new_sequence.begin() + first + 1, new_sequence.begin() + second + 1);

    return Path(new_distance, new_sequence);
}

Path two_opt(const Path& path) {
    vector<Node> opt_sequence;
    Path current_path = path;
    bool has_improved = true;

    while (has_improved) {
        has_improved = false;
        for (size_t i = 0; i < path.size - 2; i++) {
            for (size_t j = i + 1; j < path.size - 1; j++) {
                Path new_path = two_opt_swap(current_path, i, j);
                
                if (new_path.distance < current_path.distance) {
                    current_path = new_path;
                    has_improved = true;
                }
            }
        }
    }

    return current_path;
}

double calculate_total_distance(const vector<Node>& node_list) {
    double res_distance = 0;

    for (size_t i = 0; i < node_list.size() - 1; i++) 
        res_distance += calculate_distance(node_list[i], node_list[i + 1]);

    return res_distance;
}