#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

#include "ga/Parameters.h"

GAParameters GAParameters::from_file(const std::string& config_path) {
    GAParameters params;

    std::ifstream file(config_path);
    if (!file.is_open()) {
        std::cerr << "Error: could not open config file: " << config_path << std::endl;
        return params;
    }

    std::string line;
    while (std::getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        auto colon_pos = line.find(':');
        if (colon_pos == std::string::npos) continue;

        std::string key = line.substr(0, colon_pos);
        std::string value_str = line.substr(colon_pos + 1);

        key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
        value_str.erase(std::remove(value_str.begin(), value_str.end(), ','), value_str.end());

        if (key == "population_size") params.population_size = std::stoi(value_str);
        else if (key == "tournament_size") params.tournament_size = std::stoi(value_str);
        else if (key == "mutation_rate") params.mutation_rate = std::stof(value_str);
        else if (key == "local_opt_rate") params.local_opt_rate = std::stof(value_str);
        else if (key == "elite_size") params.elite_size = std::stoi(value_str);
        else if (key == "max_generations") params.max_generations = std::stoi(value_str);
    }

    return params;
}

void GAParameters::print() const {
    std::cout << "  Population size: " << population_size << "\n"
              << "  Elite size: " << elite_size << "\n"
              << "  Tournament size: " << tournament_size << "\n"
              << "  Mutation rate: " << mutation_rate << "\n"
              << "  Local opt rate: " << local_opt_rate << "\n"
              << "  Max generations: " << max_generations << "\n";
}

bool GAParameters::validate() const {
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
