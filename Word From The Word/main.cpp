#include <cassert>
#include <fstream>
#include <iostream>
#include "CommandArgs.hpp"
#include "Test_WFTW.hpp"
#include "WordFinder.hpp"


int main(int argc, char* argv[]) 
{
	{
		const auto command_args = Commands::parse_command_args(argc, argv);
		std::ifstream in(command_args.input_file);
		if (!in) {
			std::cout << "Error: File not found/EmptyFile\n" << std::endl;
			return 1;
		}
		std::ofstream out(command_args.output_file);
		if (!out) {
			std::cout << "Error: Cannot open file for writing!" << std::endl;
			return 1;
		}
		start_test(in, out);
	}
	assert(!_CrtDumpMemoryLeaks());
	return 0;
}