#include "command_args.hpp"

commands::command_args commands::parse_command_args(int argc, char* argv[]) {
	command_args args;
	if (argc > 1) {
		args.budget_file = argv[1];
	}
	else {
		args.budget_file = "budget.txt";
	}
	if (argc > 2) {
		args.expenses_file = argv[2];
	}
	else {
		args.expenses_file = "expenses.txt";
	}
	if (argc > 3) {
		args.date_file = argv[3];
	}
	else {
		args.date_file = "date.txt";
	}
	if (argc > 4) {
		args.output_file = argv[4];
	}
	else {
		args.output_file = "out.txt";
	}
	return args;
}