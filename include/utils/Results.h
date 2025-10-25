#pragma once

#include <atomic>
#include <string>

const std::string RESULT_FILE_PATH = "results/data.csv";
const std::string PROFILE_FILE_PATH = "results/profiles/";
const double SAMPLE_INTERVAL = 0.2;

std::string get_instance_name(std::string& file_path);

void result_to_csv(
    const std::string& instance_name,
    int size,
    bool is_sequential,
    int num_threads,
    double elapsed_ms,
    double best_distance
);

extern std::atomic<bool> monitor_running;

void profile_to_csv(
    const std::string& instance_name,
    bool is_sequential,
    int num_threads
);
