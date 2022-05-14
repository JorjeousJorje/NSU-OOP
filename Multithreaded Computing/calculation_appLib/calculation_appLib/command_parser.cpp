#include <sstream>
#include "command_parser.hpp"


command_parser::command_parser(std::istream& commands_input) : commands_(commands_input) {}

builder_uptr command_parser::parse_line()
{
	std::smatch match;
	std::string line;

	std::getline(commands_, line);
	end_ = commands_.eof();

	if (!std::regex_match(line, match, regex_)) {
		if(line.empty()) {
			throw std::runtime_error("<parser error>: empty line");
		}
		throw std::runtime_error("<parser error>: invalid command '" + line + "'");
	}
	std::istringstream command_line{ match.str() };
	double left{}, right{};
	
	std::string cmd_name;
	command_line >> std::ws >> cmd_name;
	command_line >> left >> right;
	
	if (cmd_name == "add_sq") {
		return std::make_unique<add_sq_builder>(left, right);
	}
	if (cmd_name == "div") {
		return std::make_unique<div_builder>(left, right);
	}
	if (cmd_name == "mult") {
		return std::make_unique<mult_builder>(left, right);
	}
	if (cmd_name == "sq_add") {
		return std::make_unique<sq_add_builder>(left, right);
	}
	if (cmd_name == "sub") {
		return std::make_unique<sub_builder>(left, right);
	}
	return std::make_unique<add_builder>(left, right);
}

bool command_parser::end() const
{
	return end_;
}
