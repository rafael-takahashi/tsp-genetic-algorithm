#include "parallel/ThreadPool.h"
#include "ga/Parameters.h"

using namespace std;

ThreadPool::ThreadPool(int work_size, int num_threads, int base_seed) {
    int segment_size = work_size / num_threads;
    int remainder = work_size % num_threads;

    thread_starts_.resize(num_threads);
    thread_ends_.resize(num_threads);

    int offset = 0;
    for (int i = 0; i < num_threads; i++) {
        int seg = segment_size + (i < remainder ? 1 : 0);
        thread_starts_[i] = offset;
        thread_ends_[i] = offset + seg;
        offset += seg;
    }

    generators_.resize(num_threads);
    
    for (int i = 0; i < num_threads; i++) {
        generators_[i].seed(base_seed + i);
        threads_.emplace_back([this, i] {
            while (true) {
                function<void(int)> task;
                {
                    unique_lock<mutex> lock(queue_mutex_);
                    
                    condition_.wait(lock, [this] {
                        return !queue_.empty() || stop_;
                    });
                    
                    if (stop_ && queue_.empty()) return;
                    
                    task = move(queue_.front());
                    queue_.pop();

                    lock.unlock();
                }
                task(i);
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        unique_lock<mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (auto& thread : threads_) thread.join();
}

future<vector<Path>> ThreadPool::enqueue(function<vector<Path>(mt19937&, int, int)> task) {
    auto wrapper = make_shared<packaged_task<vector<Path>(mt19937&, int, int)>>(task);
    future<vector<Path>> res = wrapper->get_future(); 
    {
        unique_lock<mutex> lock(queue_mutex_);
        
        if (stop_) throw runtime_error("enqueue on stopped ThreadPool");

        queue_.emplace([wrapper, this](int worker_index) {
            int start = thread_starts_[worker_index];
            int end = thread_ends_[worker_index];
            (*wrapper)(generators_[worker_index], start, end);
        });
    } 
    condition_.notify_one();
    return res;
}
