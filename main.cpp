#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "node.h"

using namespace std;

vector<Node> tsp_to_vector(const string& file_path);
vector<Node> nearest_neighbor(const vector<Node>& node_list);
double calculate_distance(double x1, double y1, double x2, double y2);

int main() {
    string file_path = "instances/a280.tsp";
    vector<Node> node_list = tsp_to_vector(file_path);

    vector<Node> node_sequence = nearest_neighbor(node_list);

    for (Node node : node_sequence)
        node.print();

    return 0; 
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

vector<Node> nearest_neighbor(const vector<Node>& node_list) {
    vector<Node> node_sequence;
    vector<bool> visited(node_list.size(), false);

    int current = 0;

    node_sequence.push_back(node_list[current]);

    visited[current] = true;

    while (node_sequence.size() < node_list.size()) {
        double min_distance = numeric_limits<double>::max();
        int next;

        for (int i = 0; i < node_list.size(); i++) {
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
        visited[next] = true;
        current = next;
    }

    return node_sequence;
}

double calculate_distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}