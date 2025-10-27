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
    cout << "  -t, --threads <n>      Set number of threads" << endl;
    cout << "  -P, --population <n>   Overwrites configured population size with n" << endl;
    cout << "  -p, --profile          Enable profiling output" << endl;
    cout << "  -h, --help             Show this help message" << endl;
    cout << endl;
    cout << "Arguments:" << endl;
    cout << "  instance_path          Path to TSP instance (default: instances/u574.tsp)" << endl;
    cout << endl;
    cout << "Examples:" << endl;
    cout << "  " << program_name << endl;
    cout << "  " << program_name << " instances/berlin52.tsp" << endl;
    cout << "  " << program_name << " -s instances/berlin52.tsp" << endl;
    cout << "  " << program_name << " -t 4 -P 200 instances/a280.tsp" << endl;
    cout << "  " << program_name << " --profile instances/eil51.tsp" << endl;
}

int main(int argc, char* argv[]) {
    bool is_sequential = false;
    bool is_profiling_enabled = false;
    string file_path = DEFAULT_FILE_PATH;
    int num_threads = max(1u, thread::hardware_concurrency());
    GAParameters params = GAParameters::from_file(CONFIG_PATH);
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-s" || arg == "--sequential") {
            is_sequential = true;
            num_threads = 1;
        } else if (arg == "-t" || arg == "--threads") {
            if (i + 1 < argc) {
                num_threads = stoi(argv[++i]);
            } else {
                cerr << "Error: Missing value for " << arg << " option." << endl;
                return 1;
            }
        } else if (arg == "-P" || arg == "--population") {
            if (i + 1 < argc) {
                params.population_size = std::stoi(argv[++i]);
            } else {
                std::cerr << "Error: Missing value for " << arg << " option." << std::endl;
                return 1;
            }
        } else if (arg == "-p" || arg == "--profile") {
            is_profiling_enabled = true;
        } else {
            file_path = arg;
        }
    }

    string instance_name = get_instance_name(file_path);
    
    string mode = is_sequential ? "sequential" : "parallel";
    cout << "Configuration:" << endl;
    cout << "  Profiling Enabled: " << boolalpha << is_profiling_enabled << endl;
    cout << "  Mode: " << mode << endl;
    cout << "  Instance: " << instance_name << endl;
    if (!is_sequential) cout << "  Number of Threads: " << num_threads << endl;
    params.print();
    cout << endl;
    
    vector<Node> node_list = tsp_to_vector(file_path);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> prob_dist(0.0, 1.0);

    if (is_profiling_enabled) {
        std::thread monitor(
            profile_to_csv,
            instance_name,
            is_sequential,
            num_threads
        );

        Path found_path = is_sequential
            ? genetic_algorithm(node_list, params, gen, prob_dist)
            : parallel_genetic_algorithm(node_list, params, gen, num_threads);

        monitor_running.store(false);
        monitor.join();
    } else {
        auto start = std::chrono::high_resolution_clock::now();

        Path found_path = is_sequential
            ? genetic_algorithm(node_list, params, gen, prob_dist)
            : parallel_genetic_algorithm(node_list, params, gen, num_threads);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        result_to_csv(
            instance_name,
            found_path.size,
            params.population_size,
            is_sequential,
            is_sequential ? 1 : num_threads,
            elapsed.count(),
            found_path.distance
        );
    }

    return 0;
}
