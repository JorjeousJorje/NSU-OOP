#pragma once
#include <string>
#include <istream>

class config_parser final
{
public:
	config_parser() = delete;
	explicit config_parser(std::istream& in) : in_(in) {}

	[[nodiscard]] std::string parse_line();
	[[nodiscard]] bool end() const;
private:
	std::istream& in_;
	bool end_{ false };
};
