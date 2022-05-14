#include "Parser.hpp"
#include <istream>
#include <sstream>

void Words_Parser::parse(ParserClient& client)
{
	std::string buffer;
	std::vector<std::string> parsed_string;
	while(std::getline(input, buffer)) {
		std::istringstream iss(buffer);
		std::string word;
		while(iss >> word) {
			parsed_string.emplace(parsed_string.begin(), word);
		}
	}
	client.process_row(parsed_string);
}
