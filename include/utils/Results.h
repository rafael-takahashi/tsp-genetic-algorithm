#pragma once

#include <string>

std::string get_instance_name(std::string& file_path);

void write_result_to_csv(
    const std::string& instance_name,
    bool sequential,
    int num_threads,
    double elapsed_ms,
    double best_distance
);
