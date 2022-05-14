#include <fstream>
#include "calculation_app.hpp"
#include "config.hpp"
#include "execution_timer.hpp"
#include "logger_factory.hpp"

int main()
{
	std::ifstream config_file{ "config.txt" };
	config cfg{};

	try {
		cfg.read(config_file);
	} catch (...) {
		return EXIT_FAILURE;
	}
	auto& log = logger_factory::get_logger();

	log() << "result config: " << std::endl
		  << "input path='" << cfg.look_up("input") << "'" << std::endl
		  << "output file='" << cfg.look_up("output") << "'" << std::endl
		  << "threads='" << cfg.thread_count() << "'" << std::endl
		  << std::endl;
	

	log(log_level::info) << "<calculation app> initialization..." << std::endl;
	calculation_app application{ cfg.look_up("input") };
	log(log_level::info) << "<calculation app> initialization done" << std::endl;

	// Many threads.
	timer multi_thread_calculation_time{};
	application.calculate_from_files(cfg.thread_count());
	log(log_level::empty) << "multi-thread calculation time " << multi_thread_calculation_time.time() << std::endl;
	std::ofstream ofs{ cfg.look_up("output") };
	application.save_result(ofs);
	application.restart();

	// One thread.
	timer one_thread_calculation_time{};
	application.calculate_from_files();
	log(log_level::empty) << "one thread calculation time " << one_thread_calculation_time.time() << std::endl;
	application.save_result(ofs);
	
}