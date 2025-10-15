#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <random>
#include <future>
#include "ga/Path.h"

class ThreadPool {
public:
    ThreadPool(int work_size, int num_threads);
    ~ThreadPool();

    std::future<std::vector<Path>> enqueue(
      std::function<std::vector<Path>(std::mt19937&, int, int)> task
    );

private:
    std::vector<int> thread_starts_;
    std::vector<int> thread_ends_;
    std::vector<std::thread> threads_;
    std::vector<std::mt19937> generators_;
    std::queue<std::function<void(int)>> queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

#endif
