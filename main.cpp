#include <iostream>
#include <string>
#include <cstring>

#include "ga/GeneticAlgorithm.h"
#include "ga/Parameters.h"

using namespace std;

void print_usage(const char* program_name) {
    cout << "Usage: " << program_name << " [OPTIONS] [instance_path]" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << "  -s, --sequential       Run in sequential mode (default: parallel)" << endl;
    cout << "  -t, --threads          Set number of threads" << endl;
    cout << "  -h, --help             Show this help message" << endl;
    cout << endl;
    cout << "Arguments:" << endl;
    cout << "  instance_path         Path to TSP instance (default: instances/u574.tsp)" << endl;
    cout << endl;
    cout << "Examples:" << endl;
    cout << "  " << program_name << endl;
    cout << "  " << program_name << " instances/berlin52.tsp" << endl;
    cout << "  " << program_name << " -s instances/berlin52.tsp" << endl;
    cout << "  " << program_name << " --sequential instances/eil51.tsp" << endl;
}

int main(int argc, char* argv[]) {
    bool sequential = false;
    string file_path = "instances/u574.tsp";
    int num_threads = max(2u, thread::hardware_concurrency());
    GAParameters params;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-s" || arg == "--sequential") {
            sequential = true;
        } else if (arg == "-t" || arg == "--threads") {
            if (i + 1 < argc) {
                num_threads = stoi(argv[++i]);
            } else {
                cerr << "Error: Missing value for " << arg << " option." << endl;
                return 1;
            }
        } else {
            file_path = arg;
        }
    }
    
    string mode = sequential ? "sequential" : "parallel";
    cout << "Configuration:" << endl;
    cout << "  Mode: " << mode << endl;
    cout << "  Instance: " << file_path << endl;
    if (!sequential) cout << "  Number of Threads: " << num_threads << endl;
    cout << endl;
    
    return sequential ? genetic_algorithm(file_path, params) : parallel_genetic_algorithm(file_path, params, num_threads);
}
