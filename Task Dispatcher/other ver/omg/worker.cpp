#include "worker.hpp"
#include "task_dispatcher.hpp"

void worker::add_task(task_info&& task)
{
	std::unique_lock<std::mutex> lock{ task_lock_ };
	const auto was_empty = workers_tasks_.empty();
	workers_tasks_.push_back(std::move(task));
	if (was_empty) {
		std::lock_guard wait_lock(wait_lock_);
		waiter_.notify_one();
	}
}

void worker::thread_stop()
{
	thread_stop_.store(true, std::memory_order_relaxed);
	std::unique_lock<std::mutex> lock(wait_lock_);
	waiter_.notify_one();
}

void worker::start()
{
	std::thread thr_(&worker::run, this);
	std::swap(myself_, thr_);
}

worker::~worker()
{
	thread_stop();
	if (myself_.joinable()) {
		myself_.join();
	}
}

void worker::run()
{
	task_info task;
	bool task_observed = false;
	while (!thread_stop_.load(std::memory_order_relaxed)) {
		bool found_task = false;
		std::unique_lock<std::mutex> lock{ task_lock_ };
		if(!workers_tasks_.empty()) {
			task = std::move(workers_tasks_.front());
			workers_tasks_.pop_front();
			found_task = true;
			task_observed = true;
		}
		if(!found_task) {
			if(task_observed) {
				lock.unlock();
				std::this_thread::yield();
				continue;
			}
			std::unique_lock<std::mutex> wait_lock(wait_lock_);
			lock.unlock();
			task = task_info{};
			waiter_.wait(wait_lock);
			task_observed = false;
			continue;
		}
		
		lock.unlock();
		task.task();
		
		lock.lock();
		bool ask_for_next = workers_tasks_.empty();
		lock.unlock();
		dispatcher_.task_finished(task, id_, ask_for_next);
	}

	kek = true;
}
