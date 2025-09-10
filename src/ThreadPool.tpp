#pragma once

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result_t<F, std::mt19937&, Args...>>
{
    using return_type = typename std::invoke_result_t<F, std::mt19937&, Args...>;

    auto task = std::make_shared<std::packaged_task<return_type(std::mt19937&)>>(
        std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        if (stop_)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        queue_.emplace([task, this](int worker_index) {
            (*task)(generators_[worker_index]);
        });
    }
    condition_.notify_one();
    return res;
}
