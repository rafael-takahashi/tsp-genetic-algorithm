#include "ThreadPool.h"
#include "parameters.h"

using namespace std;

    
ThreadPool::ThreadPool() {
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
