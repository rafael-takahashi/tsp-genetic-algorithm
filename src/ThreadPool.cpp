#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "parameters.h"

using namespace std;

class ThreadPool {
  public:
    ThreadPool() {
      for (int i = 0; i < MAX_THREADS; i++) {
        threads_.emplace_back([this] {
          while (true) {
            function<void()> task;
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
            task();
          }
        });
      }
    }

    ~ThreadPool() {
      {
        unique_lock<mutex> lock(queue_mutex_);
        stop_ = true;
      }

      condition_.notify_all();

      for (auto& thread : threads_) thread.join();
    }

    void enqueue(function<void()> task) {
      {
          unique_lock<std::mutex> lock(queue_mutex_);
          queue_.emplace(move(task));
      }
      condition_.notify_one();
    }

  private:
    vector<thread> threads_;
    queue<function<void()>> queue_;
    mutex queue_mutex_;
    condition_variable condition_;
    bool stop_ = false;
};
