#include "config_parser.hpp"


std::string config_parser::parse_line()
{
	std::string line;
	end_ = in_.eof();
	std::getline(in_.ignore(std::numeric_limits<std::streamsize>::max(), '='), line);
	return line;
}

bool config_parser::end() const
{
	return end_;
}

