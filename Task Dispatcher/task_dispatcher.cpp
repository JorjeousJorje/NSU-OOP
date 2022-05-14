#include "task_dispather.hpp"

task_dispatcher::task_dispatcher(const size_t number_of_workers)
{
	for (auto i = 0u; i < number_of_workers; ++i) {
		workers_.emplace_back(std::make_unique<worker>(i, *this));
		workers_[i]->start();
		available_workers_[i] = true;
	}
}

void task_dispatcher::task_finished(const std::size_t worker_id)
{
	{
		std::unique_lock<std::mutex> lock{ main_lock_ };
		available_workers_[worker_id] = true;
		wait_.notify_one();
	}
	schedule();
}

void task_dispatcher::schedule()
{
	std::unique_lock<std::mutex> lock{ main_lock_ };
	if (tasks_queue_.empty() || available_workers_.none()) {
		return;
	}
	
	const auto worker_id = first_set_bit(available_workers_, 0, available_workers_.size());
	available_workers_[worker_id] = false;

	auto task = std::move(tasks_queue_.front());
	tasks_queue_.pop_front();

	lock.unlock();
	workers_[worker_id]->add_task(std::move(task));
}

void task_dispatcher::add_threads(const std::size_t thread_num)
{
	std::unique_lock<std::mutex> lock(main_lock_);
	for (auto i = workers_.size(); i < workers_.size() + thread_num; ++i) {
		workers_.emplace_back(std::make_unique<worker>(i, *this));
		workers_[i]->start();
		available_workers_[i] = true;
	}
}

void task_dispatcher::wait_finished()
{
	std::unique_lock<std::mutex> lock(main_lock_);
	wait_.wait(lock, [&]{ return tasks_queue_.empty() && available_workers_.count() == workers_.size(); });
}

task_dispatcher::~task_dispatcher()
{
	stop_has_started_ = true;
	std::unique_lock<std::mutex> lock{ main_lock_ };
	workers_.clear();
}
