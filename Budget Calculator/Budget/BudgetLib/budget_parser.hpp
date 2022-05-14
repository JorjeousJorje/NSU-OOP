#pragma once
#include <istream>
#include <vector>

enum class modes
{
	EMPTY,
	PLUS,
	COLON,
};

class budget_parser final
{
public:
	budget_parser() = delete;
	explicit budget_parser(std::istream& in_) : in(in_) {}
	std::vector<std::tuple<std::string, double, modes>> parse() const;
	~budget_parser() = default;
private:
	std::istream& in;
};



