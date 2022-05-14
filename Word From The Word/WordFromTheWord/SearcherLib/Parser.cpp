#include "Parser.hpp"
#include <istream>
#include <sstream>

void words_parser::parse(parser_client& client)
{
	std::string buffer;
	while(std::getline(input, buffer)) {
		std::istringstream iss(buffer);
		std::string word;
		while(iss >> word) {
			client.process_row(word);
		}
	}
}
