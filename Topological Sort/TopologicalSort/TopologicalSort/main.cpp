#include <iostream>
#include <fstream>
#include <Windows.h>
#include "CommandArgs.hpp"
#include "quick_test.hpp"

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	{
		const auto command_args = commands::parse_command_args(argc, argv);
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
	return 0;
}
