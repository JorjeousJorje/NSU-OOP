#include "tree_parcer.hpp"
#include "date.hpp"
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace
{
	std::vector<std::string> split(const std::string& line, bool colomn, char delim = ' ')
	{
		if ((static_cast<int>(line[0]) < 0 || static_cast<int>(line[0]) > 57 || static_cast<int>(line[0]) < 48) && !colomn) {
			throw std::invalid_argument("wrong file sequence or wrong content");
		}
		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> ret;
		while (std::getline(ss, token, delim) && ret.size() != 3) {
			if (!token.empty() || !std::all_of(token.begin(), token.end(), std::isspace)) {
				ret.push_back(token);
			}
		}
		return ret;
	}
}

void expenses_parser::parse(parser_client& client)
{
	std::string buffer;
	while (std::getline(input, buffer, '\n')) {
		if (!buffer.empty()) {
			auto tokens = split(buffer, false);
			if (tokens.size() != 3) {
				throw std::invalid_argument("wrong file content");
			}
			double money = 0;
			try {
				money = std::stod(tokens.back());
			}
			catch (const std::invalid_argument& ia) {
				throw std::invalid_argument(ia.what());
			}
			auto current_date = date::parse(tokens[0]);
			auto colon_tokens = split(tokens[1], true, ':');
			client.process_row(colon_tokens[0], colon_tokens[1], current_date, money);
		}
	}
}
