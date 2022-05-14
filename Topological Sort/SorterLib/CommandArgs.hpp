#pragma once
#include <string>
#include <fstream>

namespace commands
{
	struct command_args {
		std::string input_file;
		std::string output_file;
	};

	command_args parse_command_args(int argc, char* argv[]);
}