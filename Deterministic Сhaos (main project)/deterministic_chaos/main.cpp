#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <cassert>
#include "simulation_system.hpp"
#include <iostream>
#define NEWD new(_NORMAL_BLOCK, __FILE__, __LINE__)

int main()
{
	try {
		simulation_system system(6000, 45.f, 100.f, 0.f, 0.f,
			drawing_engine::screen_weight, drawing_engine::screen_height, true, true, true, false);
		system.simulate();
	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
	}

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}