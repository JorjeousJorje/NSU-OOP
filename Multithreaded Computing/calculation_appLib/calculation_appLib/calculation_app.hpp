#pragma once
#include <functional>
#include "file_manager.hpp"
#include "thread_pool.hpp"
#include "logger_factory.hpp"


class calculation_app final
{
public:
	explicit calculation_app(const std::string& path) : manager_{path} {}

	void calculate_command(command_uptr&& command) {		
		auto [cmd_name, cmd_result] = command->execute();
		log_(log_level::debug) << "operation " + cmd_name + " is done: " << cmd_result << std::endl;
		final_sum_ += cmd_result;
	}
	
	void calculate_from_files(const std::size_t num_of_threads)
	{
		log_(log_level::info) << "<calculation app>: calculation has started" << std::endl;
		thread_pool pool{ num_of_threads };

		for(const auto& directory_file : manager_.directory()) {
			pool.insert_task([&]
			{
				auto&& builders_from_file = directory_file.get_file_commands();
				for (auto&& builder : builders_from_file) {
					thread_safe_calculate_command(builder->construct());
				}
			});
		}

		pool.wait_finished();
		log_(log_level::info) << "<calculation app>: calculation has just ended" << std::endl;
	}

	void calculate_from_files()
	{
		log_(log_level::info) << "<calculation app>: calculation has started" << std::endl;
		for (const auto& directory_file : manager_.directory()) {
			auto&& builders_from_file = directory_file.get_file_commands();
			for (auto&& builder : builders_from_file) {
				calculate_command(builder->construct());
			}
		}
		log_(log_level::info) << "<calculation app>: calculation has just ended" << std::endl;
	}
	
	[[nodiscard]] double result() const {
		return final_sum_;
	}

	void save_result(std::ostream& os) const {
		os << "result is: " << final_sum_ << std::endl;
		log_(log_level::info) << "<calculation app>: result saved" << std::endl;
	}

	void restart() {
		final_sum_ = 0;
		log_(log_level::info) << "<calculation app>: restarted" << std::endl;
	}

private:
	
	void thread_safe_calculate_command(command_uptr&& command) {
		auto [cmd_name, cmd_result] = command->execute();
		log_(log_level::debug) << "operation " + cmd_name + " is done: " << cmd_result << std::endl;
		std::lock_guard<std::mutex> lock{ main_lock_ };
		final_sum_ += cmd_result;
	}
	
	const file_manager manager_;
	double final_sum_{ 0 };
	std::mutex main_lock_{};
	const logger& log_ = logger_factory::get_logger();
};
