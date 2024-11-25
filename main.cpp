#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "node.h"

int main() {
    std::ifstream file("instances/a280.tsp");

    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    std::string line;
    int dimension;
    
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
        std::cout << line << '\n';
    }
    
    file.close();
    return 0;
}
