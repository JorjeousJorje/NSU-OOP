#pragma once
#include <string>
#include <vector>

class ParserClient
{
public:
	virtual void process_row(const std::vector<std::string>& parsed_data) {}
	virtual ~ParserClient() = default;
};


class Parser
{
public:
	explicit Parser(std::istream& _input) : input(_input) {}
	virtual void parse(ParserClient& client) {}
	virtual ~Parser() = default;

protected:
	std::istream& input;
};

class Words_Parser final : Parser
{
public:
	explicit Words_Parser(std::istream& _input) : Parser(_input) {}
	void parse(ParserClient& client) override;
};
