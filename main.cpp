#include <iostream>
#include <string>
#include <cstring>

#include "ga/genetic.h"

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS] [instance_path]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -s, --sequential       Run in sequential mode (default: parallel)" << std::endl;
    std::cout << "  -h, --help            Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "  instance_path         Path to TSP instance (default: instances/u574.tsp)" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program_name << std::endl;
    std::cout << "  " << program_name << " instances/berlin52.tsp" << std::endl;
    std::cout << "  " << program_name << " -s instances/berlin52.tsp" << std::endl;
    std::cout << "  " << program_name << " --sequential instances/eil51.tsp" << std::endl;
}

int main(int argc, char* argv[]) {
    bool sequential = false;
    std::string file_path = "instances/u574.tsp";
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-s" || arg == "--sequential") {
            sequential = true;
        } else {
            file_path = arg;
        }
    }
    
    std::string mode = sequential ? "sequential" : "parallel";
    std::cout << "Configuration:" << std::endl;
    std::cout << "  Mode: " << mode << std::endl;
    std::cout << "  Instance: " << file_path << std::endl;
    std::cout << std::endl;
    
    return sequential ? genetic_algorithm(file_path) : parallel_genetic_algorithm(file_path);
}