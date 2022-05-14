#pragma once
#include <regex>
#include "command_builder.hpp"
#include "add_builder.hpp"
#include "add_sq_builder.h"
#include "div_builder.hpp"
#include "mult_builder.hpp"
#include "sq_add_builder.hpp"
#include "sub_builder.hpp"

class command_parser final
{
public:
	explicit command_parser(std::istream& commands_input);
	builder_uptr parse_line();
	[[nodiscard]] bool end() const;

private:
	std::istream& commands_;
	bool end_ = false;
	
	inline static std::regex const regex_{ R"(^\s*?(add|mult|add_sq|sq_add|sub|div){1}(\s[+-]?([0-9]*[.])?[0-9]+){2})" };
};
