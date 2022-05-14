#pragma once

#include <algorithm>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>
#include "task.hpp"

class task_dispatcher;

class worker
{

public:
	explicit worker(size_t id, task_dispatcher& dispatcher) : id_(id), dispatcher_(dispatcher) {}
	
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

	
	std::deque<task_info> workers_tasks_;
	
	std::mutex wait_lock_{};
	std::mutex task_lock_{};
	
	std::condition_variable waiter_{};
	std::thread myself_;
	std::atomic_bool thread_stop_{ false };

	std::atomic_bool kek{ false };
};
