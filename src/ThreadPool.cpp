#include "ThreadPool.h"
#include "parameters.h"

using namespace std;

ThreadPool::ThreadPool() {
    int total_to_generate = POPULATION_SIZE - ELITE_SIZE;
    int segment_size = total_to_generate / MAX_THREADS;
    int remainder = total_to_generate % MAX_THREADS;

    thread_starts_.resize(MAX_THREADS);
    thread_ends_.resize(MAX_THREADS);

    int offset = 0;
    for (int i = 0; i < MAX_THREADS; i++) {
        int seg = segment_size + (i < remainder ? 1 : 0);
        thread_starts_[i] = offset;
        thread_ends_[i] = offset + seg;
        offset += seg;
    }

    generators_.resize(MAX_THREADS);
    
    for (int i = 0; i < MAX_THREADS; i++) {
        generators_[i].seed(SEED + i);
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
