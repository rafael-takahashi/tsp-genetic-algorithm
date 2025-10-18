#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#include "ga/GeneticAlgorithm.h"
#include "ga/Parameters.h"
#include "tsp/Tsp.h"
#include "utils/Results.h"

using namespace std;

const string DEFAULT_FILE_PATH = "instances/u574.tsp";
const string CONFIG_PATH = "config.json";

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
    bool isSequential = false;
    string file_path = DEFAULT_FILE_PATH;
    int num_threads = max(1u, thread::hardware_concurrency());
    GAParameters params = GAParameters::from_file(CONFIG_PATH);
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-s" || arg == "--sequential") {
            isSequential = true;
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
    
    string mode = isSequential ? "sequential" : "parallel";
    cout << "Configuration:" << endl;
    cout << "  Mode: " << mode << endl;
    cout << "  Instance: " << file_path << endl;
    if (!isSequential) cout << "  Number of Threads: " << num_threads << endl;
    params.print();
    cout << endl;
    
    vector<Node> node_list = tsp_to_vector(file_path);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> prob_dist(0.0, 1.0);

    auto start = chrono::high_resolution_clock::now();

    Path found_path = isSequential
        ? genetic_algorithm(node_list, params, gen, prob_dist)
        : parallel_genetic_algorithm(node_list, params, gen, num_threads);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    
    // cout << "Execution time: " << elapsed.count() << " ms\n";

    write_result_to_csv(
        get_instance_name(file_path),
        found_path.size,
        isSequential,
        isSequential ? 1 : num_threads,
        elapsed.count(),
        found_path.distance
    );

    return 0;
}
