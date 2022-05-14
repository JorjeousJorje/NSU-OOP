#pragma once
#include <string>
#include <fstream>

namespace commands
{
	struct command_args final
	{
		std::string expenses_file;
		std::string output_file;
		std::string date_file;
		std::string budget_file;
	};
	command_args parse_command_args(int argc, char* argv[]);
}
