#pragma once
#include <chrono>
#include <sstream>
#include <unordered_map>
#include <utility>

inline std::string filename_offset(const std::string& filename)
{
	const auto pos = filename.find_last_of("/\\");
	if (pos != std::string::npos) {
		return filename.substr(pos + 1, filename.size() - pos);
	}
	return filename;
}

struct enum_hasher
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};

enum class log_level : uint8_t {
	all,
	debug,
	info,
	warning,
	error,
	fatal,
	empty,
};

static inline std::unordered_map<log_level, std::string, enum_hasher> const uncolored
{
	{log_level::empty,   {}},
	{log_level::fatal,   "[FATAL] : "},
	{log_level::error,   "[ERROR] : "},
	{log_level::warning, "[WARNING] : "},
	{log_level::debug,   "[DEBUG] : "},
	{log_level::info,    "[INFO] : "},
};

static inline std::unordered_map<log_level, std::string, enum_hasher> const colored
{
	{log_level::empty,   {}},
	{log_level::fatal,   "\x1b[31;1m[FATAL]\x1b[0m : "},
	{log_level::error,   "\x1b[91;1m[ERROR]\x1b[0m : "},
	{log_level::warning, "\x1b[33;1m[WARNING]\x1b[0m : "},
	{log_level::debug,	 "\x1b[92;1m[DEBUG]\x1b[0m : "},
	{log_level::info,	 "\x1b[35;1m[INFO]\x1b[0m : "}
};

inline std::string timestamp()
{
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char timestamp[26];
	ctime_s(timestamp, sizeof timestamp, &now);

	const std::string timestamp_to_str(timestamp);
	return "[" + timestamp_to_str.substr(11, 8) + "] ";
}

inline log_level str_to_level(const std::string& level)
{
	if (level == "debug") {
		return log_level::debug;
	}
	if (level == "info") {
		return log_level::info;
	}
	if (level == "warning") {
		return log_level::warning;
	}
	if (level == "error") {
		return log_level::error;
	}
	if (level == "fatal") {
		return log_level::fatal;
	}
	if (level == "all") {
		return log_level::all;
	}
	
	return log_level::empty;
}

using config_t = std::unordered_map<std::string, std::string>;
using log_levels = std::unordered_map<log_level, std::string, enum_hasher>;

inline log_levels color_map(const std::string& option)
{
	if (option == "true") {
		return colored;
	}
	return uncolored;
}