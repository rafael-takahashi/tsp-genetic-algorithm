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

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result_t<F, std::mt19937&, Args...>>;

private:
    std::vector<std::thread> threads_;
    std::vector<std::mt19937> generators_;
    std::queue<std::function<void(int)>> queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

#include "ThreadPool.tpp"

#endif
