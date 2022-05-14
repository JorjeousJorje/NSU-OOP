#include "budget_parser.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

namespace
{
	std::vector<std::string> split(const std::string& line, char delim = ' ')
	{
		if (static_cast<int>(line[0]) < 58 && static_cast<int>(line[0]) > 48) {
			throw std::invalid_argument("wrong file sequence or wrong content");
		}
		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> ret;
		while (std::getline(ss, token, delim)) {
			if (!token.empty() || !std::all_of(token.begin(), token.end(), std::isspace)) {
				ret.push_back(token);
			}
		}
		return ret;
	}
}

std::vector<std::tuple<std::string, double, modes>> budget_parser::parse() const
{
	std::string buffer;
	std::vector<std::tuple<std::string, double, modes>> v;
	while (std::getline(in, buffer, '\n')) {
		if (!buffer.empty()) {
			auto tokens = split(buffer);
			double money = 0;
			try {
				money = std::stod(tokens.back());
			}
			catch (const std::invalid_argument& ia) {
				throw std::invalid_argument(ia.what());
			}

			tokens.pop_back();
			for (auto token = tokens.begin(); token != tokens.end(); ++token) {
				auto colon_tokens = split(*token, ':');
				auto next = std::next(token, 1);
				if (colon_tokens.size() == 1) {
					if (next != tokens.end()) {
						if (*next == "+") {
							v.emplace_back(std::make_tuple(*token, money, modes::PLUS));
							++token;
							continue;
						}
					}
					v.emplace_back(std::make_tuple(*token, money, modes::EMPTY));
				}
				else {
					v.emplace_back(std::make_tuple(colon_tokens[0], money, modes::COLON));
					if (next != tokens.end()) {
						if (*next == "+") {
							v.emplace_back(std::make_tuple(colon_tokens[1], money, modes::PLUS));
							++token;
							continue;
						}
					}
					v.emplace_back(std::make_tuple(colon_tokens[1], money, modes::EMPTY));
				}
			}
		}
	}
	return v;
}

