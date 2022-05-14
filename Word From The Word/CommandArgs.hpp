#pragma once

#include <string>
#include <fstream>

namespace Commands
{
	typedef struct {
		std::string input_file;
		std::string output_file;
	} CommandArgs;

	CommandArgs parse_command_args(int argc, char* argv[]);
}