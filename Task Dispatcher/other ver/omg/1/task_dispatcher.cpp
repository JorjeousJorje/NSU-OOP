#include "task_dispatcher.hpp"

task_dispatcher::task_dispatcher(const size_t number_of_workers)
{
	for (auto i = 0u; i < number_of_workers; ++i) {
		workers_.emplace_back(std::make_unique<worker>(i, *this));
		workers_[i]->start();
		available_workers_.insert(i);
	}
}

void task_dispatcher::insert_task(task_info&& task)
{
	if(!stop_has_started_) {
		std::unique_lock<std::mutex> lock{ main_lock_ };
		tasks_queue_.push_back(task);
	}
	schedule();
}

void task_dispatcher::task_finished(const std::size_t worker_id)
{
	if(!queue_fully_empty_) {
		std::unique_lock<std::mutex> lock{ main_lock_ };
		available_workers_.insert(worker_id);
	}
	schedule();
}

void task_dispatcher::schedule()
{
	/*SpinLockHolder lock{ &main_lock_, stop_has_started_ };

	if (!lock.isLocked()) {
		return;
	}*/
	
	std::unique_lock<std::mutex> lock{ main_lock_ };
	if (tasks_queue_.empty()) {
		queue_empty_ = true;
		wait_.notify_one();
		return;
	}
	if (available_workers_.empty()) {
		return;
	}

	const auto worker_id = *available_workers_.begin();
	available_workers_.erase(available_workers_.begin());
	
	auto task = std::move(tasks_queue_.front());
	tasks_queue_.pop_front();
	
	lock.unlock();
	workers_[worker_id]->add_task(std::move(task));
}

task_dispatcher::~task_dispatcher()
{
	stop_has_started_ = true;
	std::unique_lock<std::mutex> wait_lock{ main_lock_ };
	wait_.wait(wait_lock, [=] { return queue_empty_; });
	queue_fully_empty_ = true;
	workers_.clear();
}
