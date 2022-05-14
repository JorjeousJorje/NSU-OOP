#include "task_dispather.hpp"

void worker::add_task(task_info&& task)
{
	std::unique_lock<std::mutex> lock{ task_lock_ };
	task_ = std::move(task);
	doing_task_ = true;
	waiter_.notify_one();
}

void worker::thread_stop()
{
	std::unique_lock<std::mutex> lock(task_lock_);
	thread_stop__ = true;
	waiter_.notify_one();
}

void worker::start()
{
	std::thread thread(&worker::run, this);
	std::swap(myself_, thread);
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
	while (true) {
		{
			std::unique_lock<std::mutex> lock{ task_lock_ };
			waiter_.wait(lock, [=] {return doing_task_ || thread_stop__; });
			if(doing_task_) {
				task = std::move(task_);
			}
			else if (thread_stop__) {
				break;
			}
		}
		task.task();
		
		std::unique_lock<std::mutex> lock{ task_lock_ };
		doing_task_ = false;
		lock.unlock();
		
		dispatcher_.task_finished(id_);
	}
}
