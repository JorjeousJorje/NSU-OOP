#pragma once
#include <istream>

#include "date.hpp"

class parser_client
{
public:
	virtual void process_row(const std::string& root, const std::string& node, const date& date, double money) {}
	virtual ~parser_client() = default;
};

class parser
{
public:
	parser() = delete;
	explicit parser(std::istream& _input) : input(_input) {}
	virtual void parse(parser_client& client) {}
	virtual ~parser() = default;

protected:
	std::istream& input;
};

class expenses_parser final : public parser
{
public:
	expenses_parser() = delete;
	explicit expenses_parser(std::istream& _input) : parser(_input) {}
	void parse(parser_client& client) override;
};
