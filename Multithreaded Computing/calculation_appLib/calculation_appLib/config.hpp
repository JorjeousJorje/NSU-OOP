#pragma once
#include <unordered_map>
#include "config_parser.hpp"
#include <filesystem>
#include "logger.hpp"

namespace fs = std::filesystem;
using config_t = std::unordered_map<std::string, std::string>;


class config final
{
	class logger_config final
	{
	public:
		logger_config() = default;

		void read(config_parser& parser)
		{
			auto default_log{ false };
			auto empty_log{ false };

			for (const auto& name : order_) {
				config_.at(name) = parser.parse_line();

				if (name == "log" && config_.at(name) != "true" && config_.at(name) != "false") {
					default_log = true;
					logger_factory::get_logger();
				}
				else if (name == "log" && config_.at(name) == "false") {
					empty_log = true;
					config_.at("log_level") = "empty";
					logger_factory::get_logger(config_);
				}
			}

			if (!default_log && !empty_log) {
				logger_factory::get_logger(config_);
			}
		}

	private:
		config_t config_ =
		{
			{"log", ""},
			{"type", ""},
			{"color", ""},
			{"filename", ""},
			{"log_level", ""}
		};

		const std::vector<std::string> order_{ "log", "type", "color", "filename", "log_level" };
	};
	
public:
	config() = default;

	void read(std::istream& input) {
		if(input.peek() == std::istream::traits_type::eof()) {
			auto& log = logger_factory::get_logger();
			
			log(log_level::fatal) << "<config>: " << "empty config file" << std::endl;
			throw std::runtime_error{ "" };
		}
		
		config_parser parser{ input };
		log_config_.read(parser);
		
		const auto& log = logger_factory::get_logger();

		for(const auto& name : order_) {
			config_.at(name) = parser.parse_line();
			
			if (name == "input" && !fs::directory_entry{ config_.at(name) }.is_directory()) {
				if(config_.at(name).empty()) {
					log(log_level::fatal) << "<config>: item 'input directory' is empty" << std::endl;
				}
				else {
					log(log_level::fatal) << "<config>: " << name << " is not directory." << std::endl;
				}
				throw std::runtime_error{ "" };
			}

			if (name == "output" && !fs::directory_entry{ config_.at(name) }.is_regular_file()) {
				if (config_.at(name).empty()) {
					log(log_level::fatal) << "<config>: item 'output file' is empty" << std::endl;
				}
				else {
					log(log_level::fatal) << "<config>: " << name << " is not regular file." << std::endl;
				}
				throw std::runtime_error{ "" };
			}
		}

		log(log_level::info) << "<config>: received" << std::endl;
	}

	std::string look_up(const std::string& name)
	{
		std::string setting{};
		try {
			setting = config_.at(name);
		}
		catch (const std::out_of_range& ex) {
			const auto& log = logger_factory::get_logger();
			
			log(log_level::error) << "<config>: " << ex.what() << std::endl;
			return {};
		}
		return setting;
	}

	[[nodiscard]] std::size_t thread_count() const
	{
		std::size_t thread_count{};
		try {
			thread_count = std::stoul(config_.at("threads"));
		} catch (const std::exception& ex) {
			const auto& log = logger_factory::get_logger();
			
			log(log_level::error) << "<config>: " << ex.what() << std::endl;
			return 1u;
		}
		return thread_count;
	}

private:
	config_t config_ = 
	{
		{"input", ""},
		{"output", ""},
		{"threads", ""},
	};

	const std::vector<std::string> order_{ "input", "output", "threads" };
	logger_config log_config_{};
	
};