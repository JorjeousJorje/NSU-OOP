#pragma once
#include <string>
#include <vector>

class parser_client
{
public:
	virtual void process_row(const std::vector<std::string>&) {}
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

class graph_parser final : parser
{
public:
	graph_parser() = delete;
	explicit graph_parser(std::istream& _input) : parser(_input) {}
	void parse(parser_client& client) override;
};
