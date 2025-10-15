#pragma once

#include <string>
#include <iostream>

struct GAParameters {
    size_t population_size = 50;
    size_t tournament_size = 5;
    float mutation_rate = 0.1f;
    float local_opt_rate = 0.3f;
    size_t elite_size = 2;
    int max_generations = 50;

    static GAParameters from_file(const std::string& config_path);

    void print() const;
    bool validate() const;
};
