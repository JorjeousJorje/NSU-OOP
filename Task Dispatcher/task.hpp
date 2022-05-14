#pragma once
#include <functional>


struct task_info
{
	task_info() = default;
	explicit task_info(std::function<void()>&& task) : task(std::move(task)) {}
	
	std::function<void()> task;
};
