#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "node.h"

int main() {
    std::ifstream file("instances/a280.tsp");

    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    std::string line;
    int dimension;
    std::vector<Node> node_list;
    
    while (std::getline(file, line) && line != "NODE_COORD_SECTION") {
        if (line.find("DIMENSION:") != std::string::npos) {
            std::stringstream ss(line);
            std::string keyword;
            ss >> keyword >> dimension;
        }
    }

    std::cout << "Dimension: " << dimension << '\n';
    std::cout << "Data Section\n";

    while (std::getline(file, line) && line != "EOF") {
        std::istringstream stream(line);
        std::string string_id, string_x, string_y;

        stream >> string_id >> string_x >> string_y;
        
        std::stringstream idStream(string_id), xStream(string_x), yStream(string_y);
        int id;
        double x, y;
        
        idStream >> id;
        xStream >> x;
        yStream >> y;

        Node node(id, x, y);
        node_list.push_back(node);
    }

    for (Node node : node_list)
        node.print();

    file.close();
    return 0;
}
