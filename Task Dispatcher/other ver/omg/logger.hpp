#pragma once
#include "logger_stategies.hpp"
#include <condition_variable>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <mutex>
#include <utility>


inline std::string filename_offset(const std::string& filename)
{
	const auto pos = filename.find_last_of("/\\");
	if(pos != std::string::npos) {
		return filename.substr(pos + 1, filename.size() - pos);
	}
	return filename;
}


template<class Strategy>
class logger
{
	struct std_log_msg
	{
		std_log_msg(const log_level log_level, std::string file_name, std::string line) :
			level{ log_level }, filename{ std::move(file_name) }, str_line{ std::move(line)} {}

		
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
public:
	explicit logger(Strategy&& strategy) : strategy_(strategy) {}
	logger(const logger&) = delete;
	logger(logger&&) = delete;
	logger& operator=(const logger&) = delete;
	logger& operator=(logger&&) = delete;

	stream_wrapper operator()(const log_level level = log_level::empty, const std::string& filename = {}, const size_t line = 0) const
	{
		return stream_wrapper{ *this, std_log_msg{level, filename, std::to_string(line)} };
	}

	~logger() = default;
private:
	Strategy strategy_;
	mutable std::mutex log_lock_{};
	
	void flush(const std_log_msg& log_meta_info, const std::string& data) const {
		std::istringstream iss{ data };
		std::string buf{};
		while (std::getline(iss, buf, '\n')) {
			{
				std::lock_guard <std::mutex> lock{ log_lock_ };
				strategy_.write(buf, log_meta_info.level, log_meta_info.filename, log_meta_info.str_line);
			}
		}
	}
};
