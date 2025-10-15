#pragma once

#include <string>
#include <thread>
#include <algorithm>
#include <iostream>

struct GAParameters {
    size_t population_size = 50;
    size_t tournament_size = 5;
    float mutation_rate = 0.1f;
    float local_opt_rate = 0.3f;
    size_t elite_size = 2;
    int max_generations = 50;
    
    // static GAParameters from_file(const std::string& config_path);
    
    void print() const {
        std::cout << "GA Configuration:\n"
                  << "  Population size: " << population_size << "\n"
                  << "  Elite size: " << elite_size << "\n"
                  << "  Tournament size: " << tournament_size << "\n"
                  << "  Mutation rate: " << mutation_rate << "\n"
                  << "  Local opt rate: " << local_opt_rate << "\n"
                  << "  Max generations: " << max_generations << "\n";
    }
    
    bool validate() const {
        if (population_size < elite_size * 2) {
            std::cerr << "Error: population_size must be at least 2x elite_size\n";
            return false;
        }
        if (tournament_size > population_size) {
            std::cerr << "Error: tournament_size cannot exceed population_size\n";
            return false;
        }
        if (mutation_rate < 0.0f || mutation_rate > 1.0f) {
            std::cerr << "Error: mutation_rate must be between 0.0 and 1.0\n";
            return false;
        }
        if (local_opt_rate < 0.0f || local_opt_rate > 1.0f) {
            std::cerr << "Error: local_opt_rate must be between 0.0 and 1.0\n";
            return false;
        }
        if (max_generations <= 0) {
            std::cerr << "Error: max_generations must be positive\n";
            return false;
        }
        return true;
    }
};
