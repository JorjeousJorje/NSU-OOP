#pragma once
#include <string>

class parser_client
{
public:
	virtual void process_row(const std::string& parsed_data) {}
	virtual ~parser_client() = default;
};


class parser
{
public:
	explicit parser(std::istream& _input) : input(_input) {}
	virtual void parse(parser_client& client) {}
	virtual ~parser() = default;

protected:
	std::istream& input;
};

class words_parser final : parser
{
public:
	explicit words_parser(std::istream& _input) : parser(_input) {}
	void parse(parser_client& client) override;
};
