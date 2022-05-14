#pragma once
#include <mutex>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "logger_details.hpp"


class logger
{
protected:
	struct std_log_msg
	{
		std_log_msg() = default;
		explicit std_log_msg(const log_level log_level, std::string file_name, std::string line) :
			level{ log_level }, filename{ std::move(file_name) }, str_line{ std::move(line) } {}


		log_level level{ log_level::empty };
		std::string filename{};
		std::string str_line{};

	};
	class stream_wrapper
	{
	public:
		stream_wrapper() = delete;
		explicit stream_wrapper(const logger& log, std_log_msg meta_info) : log_(log), meta_info_(std::move(meta_info)) {}

		template <typename T>
		stream_wrapper& operator<<(T const& value) {
			oss_ << value;
			return *this;
		}

		stream_wrapper& operator <<(std::ostream& (*pf)(std::ostream&)) {
			oss_ << pf;
			return *this;
		}
		stream_wrapper& operator <<(std::ios& (*pf)(std::ios&)) {
			oss_ << pf;
			return *this;
		}
		stream_wrapper& operator <<(std::ios_base& (*pf)(std::ios_base&)) {
			oss_ << pf;
			return *this;
		}

		~stream_wrapper() { //TODO: i don't know what to do here.
			log_.flush(meta_info_, oss_.str());
		}

	private:
		const logger& log_;
		std_log_msg meta_info_{};
		std::ostringstream oss_{};
	};
	virtual void flush(const std_log_msg& log_meta_info, const std::string& message) const {}
public:


	logger() = delete;
	explicit logger(const config_t& config) :
		log_level_cutoff_{ config.find("log_level") == config.end() ? log_level::empty : str_to_level(config.at("log_level")) } {}

	stream_wrapper operator()(const log_level level = log_level::empty, const std::string& filename = {}, const size_t line = 0) const
	{
		return stream_wrapper{ *this, std_log_msg{level, filename_offset(filename), std::to_string(line)} };
	}

	virtual ~logger() = default;
protected:
	void default_output(std::ostream& os, const std::string& message, const std::string& filename, const std::string& line) const {
		if (!filename.empty() && !line.empty()) {
			os << '(' << filename << ", " << line << ") ";
		}
		os << message;
	}
	const log_level log_level_cutoff_;
	mutable std::mutex log_lock_{};
};


class console_logger : public logger {
public:
	explicit console_logger(const config_t& config = { {"color", "true"} }) :
		logger{ config }, levels_{ config.find("color") == config.end() ? uncolored : color_map(config.at("color")) } {}

private:

	void flush(const std_log_msg& log_meta_info, const std::string& message) const override {
		if (log_meta_info.level < log_level_cutoff_) {
			return;
		}

		auto& os = log_meta_info.level < log_level::info ? std::cerr : std::cout;
		std::lock_guard<std::mutex> lock{ log_lock_ };
		os << levels_.find(log_meta_info.level)->second;
		default_output(os, message, log_meta_info.filename, log_meta_info.str_line);
	}
protected:
	const log_levels levels_;
};



class console_time_logger final : public console_logger {
public:
	explicit console_time_logger(const config_t& config = { {"color", "true"} }) : console_logger{ config } {}
private:

	void flush(const std_log_msg& log_meta_info, const std::string& message) const override {
		if (log_meta_info.level < log_level_cutoff_) {
			return;
		}

		auto& os = log_meta_info.level < log_level::info ? std::cerr : std::cout;
		std::lock_guard<std::mutex> lock{ log_lock_ };
		os << timestamp();
		os << levels_.find(log_meta_info.level)->second;
		default_output(os, message, log_meta_info.filename, log_meta_info.str_line);
	}
};



inline constexpr std::string_view default_file = "default_log_file.txt";

class file_logger : public logger {
public:
	file_logger() = delete;
	explicit file_logger(const config_t& config) :
		logger{ config }, levels_{ uncolored },
		ofs_{ config.find("filename") == config.end() ? std::string{default_file} : config.at("filename") } {}

private:

	void flush(const std_log_msg& log_meta_info, const std::string& message) const override {
		if (log_meta_info.level < log_level_cutoff_) {
			return;
		}

		std::lock_guard<std::mutex> lock{ log_lock_ };
		ofs_ << levels_.find(log_meta_info.level)->second;
		default_output(ofs_, message, log_meta_info.filename, log_meta_info.str_line);
	}
protected:
	const log_levels levels_;
	mutable std::ofstream ofs_;
};



class file_time_logger final : public file_logger {
public:
	file_time_logger() = delete;
	explicit file_time_logger(const config_t& config) : file_logger{ config } {}
private:

	void flush(const std_log_msg& log_meta_info, const std::string& message) const override {
		if (log_meta_info.level < log_level_cutoff_) {
			return;
		}

		std::lock_guard<std::mutex> lock{ log_lock_ };
		ofs_ << timestamp();
		ofs_ << levels_.find(log_meta_info.level)->second;
		default_output(ofs_, message, log_meta_info.filename, log_meta_info.str_line);
	}
};