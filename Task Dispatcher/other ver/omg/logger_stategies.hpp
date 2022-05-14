#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <ctime> 
#include <unordered_map>
#include <utility>
//TODO: remove log_levels from log_strategy constructor and push logging_config_t;


struct enum_hasher
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};

enum class log_level : uint8_t {
	empty,
	fatal,
	error,
	warning,
	info,
	debug
};

const std::unordered_map<log_level, std::string, enum_hasher> uncolored
{
    {log_level::empty,   {}},
	{log_level::fatal,   "[FATAL] : "},
	{log_level::error,   "[ERROR] : "},
	{log_level::warning, "[WARNING] : "},
	{log_level::debug,   "[DEBUG] : "},
	{log_level::info,    "[INFO] : "},
};

const std::unordered_map<log_level, std::string, enum_hasher> colored
{
	{log_level::empty,   {}},
	{log_level::fatal,   "\x1b[31;1m[FATAL]\x1b[0m : "},
	{log_level::error,   "\x1b[91;1m[ERROR]\x1b[0m : "},
	{log_level::warning, "\x1b[33;1m[WARNING]\x1b[0m : "},
	{log_level::debug,	 "\x1b[92;1m[DEBUG]\x1b[0m : "},
	{log_level::info,	 "\x1b[36;1m[INFO]\x1b[0m : "}
};


inline std::string timestamp()
{
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char timestamp[26];
	ctime_s(timestamp, sizeof timestamp, &now);

	const std::string timestamp_to_str(timestamp);
	return "[" + timestamp_to_str.substr(11, 8) + "] ";
}

using logging_config_t = std::unordered_map<std::string, std::string>;
using log_levels = std::unordered_map<log_level, std::string, enum_hasher>;




class log_strategy
{
public:
	explicit log_strategy(log_levels levels = colored) : levels_(std::move(levels)) {}
	virtual void write(const std::string& message, log_level level = log_level::empty, const std::string& filename = {}, const std::string& line = {}) const = 0;
	virtual ~log_strategy() = default;
protected:
	static void default_output(std::ostream& os, const std::string& message, const std::string& filename = {}, const std::string& line = {})
	{
		if (!filename.empty() && !line.empty()) {
			os << '(' << filename << ", " << line << ") ";
		}
		os << message << std::endl;
	}
	const log_levels levels_;
};

class console_log : public log_strategy
{
public:
	explicit console_log(log_levels levels = colored) : log_strategy(std::move(levels)) {}
	void write(const std::string& message, const log_level level = log_level::empty, const std::string& filename = {}, const std::string& line = {}) const override
	{
		auto& os_ = level < log_level::info ? std::cerr : std::cout;
		os_ << levels_.find(level)->second;
		default_output(os_, message, filename, line);
	}
};


class console_time_log final : public console_log
{
public:
	explicit console_time_log(log_levels levels = colored) : console_log(std::move(levels)) {}
	void write(const std::string& message, const log_level level = log_level::empty, const std::string& filename = {}, const std::string& line = {}) const override
	{
		auto& os_ = level < log_level::info ? std::cerr : std::cout;
		os_ << timestamp();
		os_ << levels_.find(level)->second;
		default_output(os_, message, filename, line);
	}
};


class file_log : public log_strategy
{
public:
	file_log() = delete;
	explicit file_log(std::ostream& stream) : log_strategy{ uncolored }, os_ { stream } {}


	void write(const std::string& message, const log_level level = log_level::empty, const std::string& filename = {}, const std::string& line = {}) const override
	{
		os_ << levels_.find(level)->second;
		default_output(os_, message, filename, line);
	}
protected:
	std::ostream& os_;
};


class file_time_log final : public file_log
{
public:
	explicit file_time_log(std::ostream& stream) : file_log{ stream } {}

	void write(const std::string& message, const log_level level = log_level::empty, const std::string& filename = {}, const std::string& line = {}) const override
	{
		os_ << timestamp();
		os_ << levels_.find(level)->second;
		default_output(os_, message, filename, line);
	}
};