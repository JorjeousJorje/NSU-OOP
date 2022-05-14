#include <cassert>
#include <fstream>
#include <iostream>
#include "command_args.hpp"
#include "test.hpp"
#include <Windows.h>


int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	{
		const auto command_args = commands::parse_command_args(argc, argv);
		std::ifstream budget(command_args.budget_file);
		if (!budget) {
			std::cout << "Error: File not found/EmptyFile\n" << std::endl;
			return 1;
		}
		std::ifstream date(command_args.date_file);
		if (!date) {
			std::cout << "Error: File not found/EmptyFile\n" << std::endl;
			return 1;
		}
		std::ifstream expenses(command_args.expenses_file);
		if (!expenses) {
			std::cout << "Error: File not found/EmptyFile\n" << std::endl;
			return 1;
		}
		std::ofstream out(command_args.output_file);
		if (!out) {
			std::cout << "Error: Cannot open file for writing!" << std::endl;
			return 1;
		}
		start_test(budget, expenses, out, date);
	}
	assert(!_CrtDumpMemoryLeaks());
	return 0;
}