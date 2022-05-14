#include "Parser.hpp"
#include <istream>
#include <sstream>

void graph_parser::parse(parser_client& client)
{
	std::string buffer;
	std::vector<std::string> parse_buffer;

	while (std::getline(input, buffer, '\n')) {
		if (!buffer.empty()) {
			size_t previous = 0;
			size_t current = buffer.find('=');
			const size_t one_adj_ver = current;
			if (current != std::string::npos) {
				parse_buffer.push_back(buffer.substr(previous, current - previous - 1));
				
				previous = buffer.find_first_not_of(" \t\n=", current);
			}
			else {
				throw std::bad_exception();
			}
			current = buffer.find(',', previous);
			if(current == std::string::npos) {
				current = buffer.find_first_not_of(" \n\t", one_adj_ver);
				if(current == std::string::npos) {
					client.process_row(parse_buffer);
					parse_buffer.clear();
					continue;
				}
				parse_buffer.push_back(buffer.substr(previous, previous - current));
				client.process_row(parse_buffer);
				parse_buffer.clear();
				continue;
			}
			size_t save_cur  = 0;
			while (current != std::string::npos) {
				parse_buffer.push_back(buffer.substr(previous, current - previous));
				
				previous = buffer.find_first_not_of(" \t\n,", current);
				save_cur = current;
				current = buffer.find(',', previous);
			}
			previous = buffer.find_first_not_of(" \t\n,", save_cur);
			current = buffer.size();
			
			parse_buffer.push_back(buffer.substr(previous, current - previous));
			client.process_row(parse_buffer);
			parse_buffer.clear();
		}
	}
}