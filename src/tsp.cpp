#include "tsp.h"

#include <chrono>
#include <fstream>
#include <limits>
#include <queue>
#include <sstream>

#include "node.h"
#include "path.h"

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

Path nearest_neighbor(vector<Node>& node_list) {
    vector<Node> node_sequence;
    double res_distance = 0;
    vector<bool> visited(node_list.size(), false);

    int current = 0;

    node_sequence.push_back(node_list[current]);

    visited[current] = true;

    while (node_sequence.size() < node_list.size()) {
        double min_distance = numeric_limits<double>::max();
        int next = -1;

        for (size_t i = 0; i < node_list.size(); i++) {
            if (visited[i] == false) {
                double distance = calculate_distance(node_list[current], node_list[i]);
                if (distance < min_distance) {
                    min_distance = distance;
                    next = i;
                }
            }
        }

        if (next == -1) {
            cerr << "Error: No unvisited node found (bug in logic?)\n";
            exit(1);
        }

        node_sequence.push_back(node_list[next]);
        res_distance += min_distance;
        visited[next] = true;
        current = next;
    }

    return Path(res_distance, node_sequence, calculate_fitness(res_distance));
}

Path farthest_insertion(vector<Node>& node_list) {
    vector<Node> node_sequence;
    double res_distance = 0;
    vector<bool> visited(node_list.size(), false);

    node_sequence.push_back(node_list[0]);
    node_sequence.push_back(node_list[1]);
    node_sequence.push_back(node_list[2]);
    visited[0] = visited[1] = visited[2] = true;

    res_distance += calculate_distance(node_list[0], node_list[1]) + 
                    calculate_distance(node_list[1], node_list[2]) + 
                    calculate_distance(node_list[2], node_list[0]);

    auto compare = [&](int i, int j) {
        double max_dist_i = -1;
        double max_dist_j = -1;

        for (const auto& node : node_sequence) {
            max_dist_i = max(max_dist_i, calculate_distance(node_list[i], node));
            max_dist_j = max(max_dist_j, calculate_distance(node_list[j], node));
        }

        return max_dist_i > max_dist_j;
    };

    priority_queue<int, vector<int>, decltype(compare)> pq(compare);

    for (long unsigned int i = 0; i < node_list.size(); i++)
        if (!visited[i]) pq.push(i);

    while (node_sequence.size() < node_list.size()) {
        int next = pq.top();
        pq.pop();

        double min_distance = numeric_limits<double>::max();
        int opt_pos = -1;

        for (size_t j = 0; j < node_sequence.size(); ++j) {
            int current = j;
            int adjacent = (j + 1) % node_sequence.size();

            double distance = calculate_distance(node_sequence[current], node_list[next]) +
                              calculate_distance(node_list[next], node_sequence[adjacent]) - 
                              calculate_distance(node_sequence[current], node_sequence[adjacent]);

            if (distance < min_distance) {
                min_distance = distance;
                opt_pos = adjacent;
            }
        }

        node_sequence.insert(node_sequence.begin() + opt_pos, node_list[next]);
        visited[next] = true;
        res_distance += min_distance;

        if (pq.empty()) break;
    }

    return Path(res_distance, node_sequence, calculate_fitness(res_distance));
}

void two_opt_swap(vector<Node>& sequence, unsigned int first, unsigned int second) {
    first++;
    while (first < second) {
        swap(sequence[first], sequence[second]);
        first++;
        second--;
    }
}

Path two_opt(Path& path) {
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

                if (delta < -1e-10) {
                    two_opt_swap(current_sequence, i, j);
                    current_distance += delta;
                    has_improved = true;
                }
            }
        }
    } while (has_improved);

    return Path(current_distance, current_sequence, calculate_fitness(current_distance));
}

double calculate_swap_delta(int i, int j, vector<Node>& node_sequence, int path_size) {
    int prev_i = (i - 1 + path_size) % path_size;
    int next_i = (i + 1) % path_size;
    int prev_j = (j - 1 + path_size) % path_size;
    int next_j = (j + 1) % path_size;

    double delta;

    // Handle case where i and j are the first and last nodes of the path;
    if (i == 0 && j == path_size - 1) {
        delta = - calculate_distance(node_sequence[i], node_sequence[next_i])
                - calculate_distance(node_sequence[prev_j], node_sequence[j])
                + calculate_distance(node_sequence[j], node_sequence[next_i])
                + calculate_distance(node_sequence[prev_j], node_sequence[i]);

        return delta;
    } 
    
    delta = - calculate_distance(node_sequence[prev_i], node_sequence[i])
            - calculate_distance(node_sequence[j], node_sequence[next_j])
            + calculate_distance(node_sequence[prev_i], node_sequence[j])
            + calculate_distance(node_sequence[i], node_sequence[next_j]);

    // If the i and j are adjacent, then delta is succefully calculated
    if (i + 1 == j)
        return delta;

    delta += - calculate_distance(node_sequence[i], node_sequence[next_i])
            - calculate_distance(node_sequence[prev_j], node_sequence[j])
            + calculate_distance(node_sequence[j], node_sequence[next_i])
            + calculate_distance(node_sequence[prev_j], node_sequence[i]);
    
    return delta;
}

Path pair_swap(Path& path) {
    vector<Node> current_sequence = path.node_sequence;
    double current_distance = path.distance;
    bool has_improved;
    
    do {
        has_improved = false;
        for (size_t i = 0; i < path.size - 1; i++) {
            for (size_t j = i + 1; j < path.size; j++) {
                double delta = calculate_swap_delta(i, j, current_sequence, path.size);

                if (delta < -1e-10) {
                    swap(current_sequence[i], current_sequence[j]);
                    current_distance += delta;
                    has_improved = true;
                }
            }
        }
    } while (has_improved);
    
    return Path(current_distance, current_sequence, calculate_fitness(current_distance));
}