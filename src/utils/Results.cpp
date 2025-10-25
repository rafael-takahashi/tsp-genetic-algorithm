#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <thread>
#include <sys/resource.h>
#include <unistd.h>

#include "utils/Results.h"

std::string get_instance_name(std::string& file_path) {
    return std::filesystem::path(file_path).stem().string();
}

void result_to_csv(
    const std::string& instance_name,
    int size,
    bool is_sequential,
    int num_threads,
    double elapsed_ms,
    double best_distance
) {
    std::ofstream file(RESULT_FILE_PATH, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Error: could not open " << RESULT_FILE_PATH << std::endl;
        return;
    }

    file.seekp(0, std::ios::end);
    if (file.tellp() == 0)
        file << "instance,size,mode,threads,time_ms,best_distance\n";

    std::string mode = is_sequential ? "sequential" : "parallel";

    file << instance_name << ","
         << size << ","
         << mode << ","
         << num_threads << ","
         << std::fixed << std::setprecision(4) << elapsed_ms << ","
         << best_distance << "\n";

    file.close();
}

std::atomic<bool> monitor_running(true);

double get_cpu_time() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6 +
           usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6;
}

double get_memory_mb() {
    std::ifstream file("/proc/self/status");
    std::string line;
    while (getline(file, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            size_t pos = line.find_first_of("0123456789");
            return std::stod(line.substr(pos)) / 1024.0;
        }
    }
    return 0.0;
}

void profile_to_csv(
    const std::string& instance_name,
    bool is_sequential,
    int num_threads
) {
    std::string profile_file = PROFILE_FILE_PATH
        + instance_name 
        + "_profile_" 
        + (is_sequential ? "sequential" : "parallel") 
        + (is_sequential ? "" : "_t" + std::to_string(num_threads)) 
        + ".csv";
    std::ofstream file(profile_file);
    file << "timestamp_s,cpu_percent,memory_mb\n";

    auto start = std::chrono::high_resolution_clock::now();
    double last_cpu = get_cpu_time();

    while (monitor_running.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(int(SAMPLE_INTERVAL*1000)));

        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - start).count();

        double current_cpu = get_cpu_time();
        double cpu_percent = (current_cpu - last_cpu) / SAMPLE_INTERVAL * 100.0;
        last_cpu = current_cpu;

        double mem_mb = get_memory_mb();

        file << elapsed << "," << cpu_percent << "," << mem_mb << "\n";
        file.flush();
    }
}
