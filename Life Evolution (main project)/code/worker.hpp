#pragma once
#include <mutex>
#include <condition_variable>
#include "task.hpp"

class task_dispatcher;

class worker
{
public:
	explicit worker(const size_t id, task_dispatcher& dispatcher) : id_(id), dispatcher_(dispatcher) {}

	worker(const worker&) = delete;
	worker(worker&&) noexcept = delete;
	worker& operator=(const worker&) = delete;
	worker& operator=(worker&&) noexcept = delete;

	void add_task(task_info&& task);
	void thread_stop();

	void start();
	~worker();

private:
	void run();

	size_t id_;
	task_dispatcher& dispatcher_;
	
	bool doing_task_{ false };
	bool thread_stop__{ false };
	task_info task_{};

	std::mutex task_lock_{};
	
	std::condition_variable waiter_{};
	std::thread myself_;
};