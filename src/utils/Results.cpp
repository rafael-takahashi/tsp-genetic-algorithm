#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "utils/Results.h"

std::string get_instance_name(std::string& file_path) {
    return std::filesystem::path(file_path).stem().string();
}

void write_result_to_csv(
    const std::string& instance_name,
    int size,
    bool isSequential,
    int num_threads,
    double elapsed_ms,
    double best_distance
) {
    std::string file_name = "results/data.csv";
    std::ofstream file(file_name, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Error: could not open " << file_name << std::endl;
        return;
    }

    file.seekp(0, std::ios::end);
    if (file.tellp() == 0)
        file << "instance,size,mode,threads,time_ms,best_distance\n";

    std::string mode = isSequential ? "sequential" : "parallel";

    file << instance_name << ","
         << size << ","
         << mode << ","
         << num_threads << ","
         << std::fixed << std::setprecision(4) << elapsed_ms << ","
         << best_distance << "\n";

    file.close();
}
