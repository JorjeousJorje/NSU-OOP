#include "CommandArgs.hpp"

Commands::CommandArgs Commands::parse_command_args(int argc, char* argv[]) {
	CommandArgs args;
	if (argc > 1) {
		args.input_file = argv[1];
	}
	else {
		args.input_file = "in.txt";
	}
	if (argc > 2) {
		args.output_file = argv[2];
	}
	else {
		args.output_file = "out.txt";
	}
	return args;
}