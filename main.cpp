#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "node.h"

std::vector<Node> tsp_to_vector(std::string file_path);

int main() {
    std::string file_path = "instances/a280.tsp";
    std::vector node_list = tsp_to_vector(file_path);

    for (Node node : node_list)
        node.print();

    return 0; 
}

std::vector<Node> tsp_to_vector(std::string file_path) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        exit(1);
    }

    std::string line;
    std::vector<Node> node_list;
    
    while (std::getline(file, line) && line != "NODE_COORD_SECTION") continue;
    
    int id;
    double x, y;
    
    while (std::getline(file, line) && line != "EOF") {
        std::istringstream stream(line);
        std::string string_id, string_x, string_y;

        stream >> string_id >> string_x >> string_y;
        id = stoi(string_id);
        x = stod(string_x);
        y = stod(string_y); 

        node_list.emplace_back(id, x, y);
    }

    file.close();

    return node_list;
}