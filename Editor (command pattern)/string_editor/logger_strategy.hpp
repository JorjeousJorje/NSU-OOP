#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <ctime> 

template <typename E>
constexpr std::underlying_type_t<E> to_underlying(E e) {
	return static_cast<std::underlying_type_t<E>>(e);
}

enum class log_level : int {
	fatal,
	error,
	warning,
	info,
	debug
};

inline int operator-(const log_level& lhs, const log_level& rhs)
{
	return to_underlying(lhs) - to_underlying(rhs);
}


class log_strategy
{
public:
	virtual void write(log_level level, const std::string& message, const char* file = nullptr, size_t line = 0) const = 0;
	virtual ~log_strategy() = default;
protected:
	static void default_output(std::ostream& os, const std::string& message, const char* file = nullptr, const size_t line = 0)
	{
		os << ": ";
		if (file && line != 0) {
			os << '(' << file << ", " << std::to_string(line) << ") ";
		}
		os << message << std::endl;
	}
	static const inline char* to_string_level_[] = { "FATAL", "ERROR", "WARNING", "INFO", "DEBUG" };
};


class console_log : public log_strategy
{
public:
	void write(const log_level level, const std::string& message, const char* file = nullptr, const size_t line = 0) const override
	{
		auto& os_ = level < log_level::info ? std::cerr : std::cout;
		color_text(os_, level);
		default_output(os_, message, file, line);
	}
protected:
	static void color_text(std::ostream& os, const log_level level)
	{
		if (level == log_level::fatal) {
			os << "\x1B[31m" << to_string_level_[to_underlying(level)] << "\033[0m";
		}
		else if (level == log_level::error) {
			os << "\x1B[91m" << to_string_level_[to_underlying(level)] << "\033[0m";
		}
		else if (level == log_level::warning) {
			os << "\x1B[33m" << to_string_level_[to_underlying(level)] << "\033[0m";
		}
		else if (level == log_level::info) {
			os << "\x1B[36m" << to_string_level_[to_underlying(level)] << "\033[0m";
		}
		else if (level == log_level::debug) {
			os << "\x1B[92m" << to_string_level_[to_underlying(level)] << "\033[0m";
		}
	}
};


class console_time_log final : public console_log
{
public:
	void write(const log_level level, const std::string& message, const char* file = nullptr, const size_t line = 0) const override
	{
		auto& os_ = level < log_level::info ? std::cerr : std::cout;
		os_ << "[" << current_time() << "] ";
		color_text(os_, level);
		default_output(os_, message, file, line);
	}
private:
	static std::string current_time()
	{
		auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char timestamp[26];
		ctime_s(timestamp, sizeof timestamp, &now);

		const std::string timestamp_to_str(timestamp);
		return timestamp_to_str.substr(11, 8);
	}
};


class file_log : public log_strategy
{
public:
	file_log() = delete;
	explicit file_log(std::ostream& stream) : os_{ stream } {}


	void write(const log_level level, const std::string& message, const char* file = nullptr, const size_t line = 0) const override
	{
		os_ << to_string_level_[to_underlying(level)];
		default_output(os_, message, file, line);
	}
protected:
	std::ostream& os_;
};


class file_time_log final : public file_log
{
public:
	explicit file_time_log(std::ostream& stream) : file_log{ stream } {}

	void write(const log_level level, const std::string& message, const char* file = nullptr, const size_t line = 0) const override
	{
		os_ << "[" << current_time() << "] ";
		os_ << to_string_level_[to_underlying(level)];
		default_output(os_, message, file, line);
	}
private:
	static std::string current_time()
	{
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char timestamp[26];
		ctime_s(timestamp, sizeof timestamp, &now);

		const std::string ts_wo_endl(timestamp);
		return ts_wo_endl.substr(11, 8);
	}
};