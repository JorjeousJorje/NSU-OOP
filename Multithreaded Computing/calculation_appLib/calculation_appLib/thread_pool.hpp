#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>


class thread_pool final
{
    using task = std::function<void()>;
public:

    explicit thread_pool(const std::size_t thread_num = 1)
    {
        log_(log_level::info) << "<thread pool> initialization..." << std::endl;
        const auto result_threads = thread_num > std::thread::hardware_concurrency() ?
														  std::thread::hardware_concurrency() : thread_num;
        start(result_threads);
        available_workers_ = result_threads;
        log_(log_level::info) << "<thread pool> initialization done" << std::endl;
    }

    [[nodiscard]] std::size_t number_of_workers() const {
        std::lock_guard<std::mutex> lock(event_lock_);
    	return workers_.size();
    }

    template<class T>
    auto insert_task(T task) -> std::future<decltype(task())>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));
        {
            std::lock_guard<std::mutex> lock{ event_lock_ };
            tasks_.emplace([=] {
                (*wrapper)();
                });
        }
        notification_.notify_one();
        return wrapper->get_future();
    }

    void wait_finished()
    {
        std::unique_lock<std::mutex> lock(event_lock_);
        notification_.wait(lock, [=] { return tasks_.empty() && available_workers_ == workers_.size(); });
    }

    ~thread_pool() {
        stop();
    }
private:
    std::vector<std::thread> workers_{};
    std::condition_variable notification_{};

    mutable std::mutex event_lock_{};
    std::queue<task> tasks_{};
    std::size_t available_workers_{};
    bool thread_stopping_{ false };

    const logger& log_ = logger_factory::get_logger();

    void start(const std::size_t thread_num)
    {
        for (auto i = 0u; i < thread_num; ++i)
        {
            workers_.emplace_back([=] {
                while (true) {
                    task task;
                    {
                        std::unique_lock<std::mutex> lock{ event_lock_ };
                        notification_.wait(lock, [=] { return thread_stopping_ || !tasks_.empty(); });

                        if (!tasks_.empty()) {
                            --available_workers_;
                            task = std::move(tasks_.front());
                            tasks_.pop();
                        }
                        else if (thread_stopping_) {
                            break;
                        }
                    }

                    task();

                    std::unique_lock<std::mutex> lock{ event_lock_ };
                    ++available_workers_;
                    notification_.notify_all();
                }
                });
        }
    }

    void stop() noexcept
    {
        {
            std::unique_lock<std::mutex> lock{ event_lock_ };
            thread_stopping_ = true;
            notification_.notify_all();
        }

        for (auto& thread : workers_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
};

