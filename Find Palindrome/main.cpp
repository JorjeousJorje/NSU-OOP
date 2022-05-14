#include "FindPalindrom.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
	palindrom palindrom;
	if (argc > 1) {
		std::ifstream f_in;
		f_in.open(argv[1]);
		if (f_in.is_open()) {
			if (!palindrom.read(f_in)) {
				std::cout << "File '" << argv[1] << "' is empty" << std::endl;
				return -1;
			}
		}
		f_in.close();
	}
	else {
		std::istringstream in("Anne, I vote more cars race Rome to Vienna.");
		if (!palindrom.read(in)) {
			std::cout << "String is empty" << std::endl;
			return -1;
		};
	}
	palindrom.write_result();
	return 0;
}