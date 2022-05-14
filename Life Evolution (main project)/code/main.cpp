#include <fstream>
#include <iostream>

#include "application.hpp"
int main()
{

	std::ifstream readme_file{ "controls.txt" };

	std::string buffer{};
	while (std::getline(readme_file, buffer)) {
		std::cout << buffer << std::endl;
	}
	
	application app{};

	app.start_program(6);

    return 0;
}

