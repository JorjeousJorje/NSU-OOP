#pragma once
#include <sstream>
#include <istream>
#include <regex>
#include <string>
#include "custom_logger.hpp"
#include "string_editor.hpp"

class command_parser final
{
public:
	command_parser() = delete;
	explicit command_parser(std::istream& in) : in_(in) {}
	
	template<class Strategy>
	void parse(string_editor& editor, const logger<Strategy>& log) const
	{
		std::smatch match{};
		for (std::string line; std::getline(in_ >> std::ws, line); ) {
			try {
				if (!std::regex_search(line, match, regex_)) {
					throw std::invalid_argument("invalid command: " + line);
				}

				std::istringstream stream{ match.str() };
				std::string buffer{};
				std::string idx2{};
				stream >> buffer;
				if (buffer == "redo") {
					editor.redo();
				}
				else if (buffer == "undo") {
					editor.undo();
				}
				else if (buffer == "paste") {
					stream >> buffer;
					editor.paste(std::stoul(buffer));
				}
				else if (buffer == "copy") {
					stream >> buffer;
					stream >> idx2;
					editor.copy(std::stoul(buffer), std::stoul(idx2));
				}
				else if (buffer == "delete") {
					stream >> buffer;
					stream >> idx2;
					editor.delete_elements(std::stoul(buffer), std::stoul(idx2));
				}
				else if (buffer == "insert") {
					stream >> buffer;
					stream >> idx2;
					editor.insert(buffer, std::stoul(idx2));
				}
			}
			catch (const std::invalid_argument& ex) {
				log.log_message(log_level::error, ex.what());
			}
			catch (const std::range_error& ex) {
				log.log_message(log_level::error, ex.what());
			}
		}
	}
private:
	std::istream& in_;
	const std::regex regex_{ std::string{R"(^\s*(copy(\s+\d+){2}|paste(\s+\d+){1}|insert\s+.*\s+\d+|delete(\s+\d+){2}|undo|redo){1})" } };
};

