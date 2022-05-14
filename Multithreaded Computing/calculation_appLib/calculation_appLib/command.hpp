#pragma once
#include <memory>

struct cmd_result final
{
	cmd_result() = default;
	cmd_result(std::string cmd, const double res) : command_name(std::move(cmd)), result(res) {}

	std::string command_name{};
	double result{};

};


class command
{
public:
	virtual cmd_result execute() const = 0 ;
	virtual ~command() = default;
};
using command_uptr = std::unique_ptr<command>;