#pragma once
#include <bitset>
#include <deque>
#include <unordered_set>
#include <future>
#include <memory>
#include "worker.hpp"


static inline constexpr int32_t max_workers_capacity = 50;

template <size_t N>
size_t first_set_bit(const std::bitset<N>& bit_set, const size_t left = 0, const size_t right = N)
{
	size_t set_bit{ 0 };
	for (auto i = left; i < right; ++i) {
		if (bit_set[i]) {
			set_bit = i;
		}
	}
	return set_bit;
}


class task_dispatcher final
{
public:
	explicit task_dispatcher(size_t number_of_workers = 1);

	template<class T>
	auto insert_task(T task) -> std::future<decltype(task())>
	{
		auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));
		if (!stop_has_started_) {
			std::unique_lock<std::mutex> lock{ main_lock_ };
			tasks_queue_.emplace_back(task_info{ [=] {
				(*wrapper)();
			} });
		}
		schedule();
	
		return wrapper->get_future();
	}


	std::size_t number_of_workers() const { return workers_.size(); }

	void task_finished(std::size_t worker_id);
	void add_threads(std::size_t thread_num);
	void wait_finished();
	
	~task_dispatcher();

private:
	void schedule();
	
	std::vector<std::unique_ptr<worker>> workers_{};
	std::deque<task_info> tasks_queue_{};
	std::bitset<max_workers_capacity> available_workers_{};

	mutable std::mutex main_lock_{};
	std::condition_variable wait_{};

	std::atomic_bool stop_has_started_{ false };
};
