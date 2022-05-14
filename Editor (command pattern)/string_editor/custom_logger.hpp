#pragma once
#include <string>
#include "logger_strategy.hpp"

template <typename T, size_t S>
constexpr size_t get_file_name_offset(const T(&str)[S], size_t i = S - 1)
{
	return (str[i] == '/' || str[i] == '\\') ? i + 1 : (i > 0 ? get_file_name_offset(str, i - 1) : 0);
}

template <typename T>
constexpr size_t get_file_name_offset(T(&str)[1])
{
	return 0;
}

namespace utility {

	template <typename T, T V>
	struct const_expr_value
	{
		static constexpr T value = V;
	};

}

#define UTILITY_CONST_EXPR_VALUE(exp)::utility::const_expr_value<decltype(exp), exp>::value
#define LOG_MSG(lvl, msg) log_message(lvl, msg, &__FILE__[UTILITY_CONST_EXPR_VALUE(get_file_name_offset(__FILE__))], __LINE__)


template <typename Strategy>
class logger
{
public:

	explicit logger(Strategy&& strategy) : strategy_{ std::move(strategy) } {}

	logger() = delete;

	void log_message(const log_level level, const std::string& message, const char* file = nullptr, const size_t line = 0) const
	{
		strategy_.write(level, message, file, line);
	}

private:
	Strategy strategy_;

};
