#include "tsp.h"
#include <cmath>
#include <fstream>
#include <limits>
#include <sstream>

using namespace std;

double calculate_distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

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

TspResult nearest_neighbor(const vector<Node>& node_list) {
    vector<Node> node_sequence;
    double res_distance = 0;
    vector<bool> visited(node_list.size(), false);

    int current = 0;

    node_sequence.push_back(node_list[current]);

    visited[current] = true;

    while (node_sequence.size() < node_list.size()) {
        double min_distance = numeric_limits<double>::max();
        int next;

        for (long unsigned int i = 0; i < node_list.size(); i++) {
            if (visited[i] == false) {
                double distance = calculate_distance(node_list[current].x, node_list[current].y,
                                                     node_list[i].x, node_list[i].y);
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

    return {res_distance, node_sequence};
}

vector<Node> two_opt_swap(vector<Node> path, long unsigned int first, long unsigned int second) {
    vector<Node> new_path;

    for (long unsigned int i = 0; i <= first; i++)
        new_path.push_back(path[i]);
    
    for (long unsigned int j = second; j >= first + 1; j--)
        new_path.push_back(path[j]);
        
    for (long unsigned int k = second + 1; k < path.size(); k++)
        new_path.push_back(path[k]);

    return new_path;
}

TspResult two_opt(const vector<Node>& path) {
    vector<Node> opt_sequence;
    vector<Node> current_path = path;
    double best_distance = calculate_total_distance(current_path);

    for (long unsigned int i = 0; i < path.size() - 2; i++) {
        for (long unsigned int j = i + 1; j < path.size() - 1; j++) {
            vector<Node> new_path = two_opt_swap(current_path, i, j);
            double new_distance = calculate_total_distance(new_path);
            
            if (new_distance < best_distance) {
                current_path = new_path;
                best_distance = new_distance;
            }
        }
    }

    return {best_distance, current_path};
}

double calculate_total_distance(const vector<Node>& node_list) {
    double res_distance = 0;

    for (long unsigned int i = 0; i < node_list.size() - 1; i++) 
        res_distance += calculate_distance(node_list[i].x, node_list[i].y, 
                                           node_list[i + 1].x, node_list[i + 1].y);

    return res_distance;
}