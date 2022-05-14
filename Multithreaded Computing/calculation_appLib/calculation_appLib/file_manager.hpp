#pragma once
#include <filesystem>
#include <fstream>
#include <utility>
#include "command_parser.hpp"
#include "logger_factory.hpp"

namespace  fs = std::filesystem;


class file_manager final
{
	struct directory_file {
		explicit directory_file(fs::path path) : my_path(std::move(path)) {}

		[[nodiscard]] std::vector<builder_uptr> get_file_commands() const
		{
			std::ifstream ifs{ my_path.c_str() };
			command_parser parser{ ifs };
			std::vector<builder_uptr> builders{};

			while (!parser.end()) {
				try {
					builders.push_back(parser.parse_line());
				}
				catch (const std::exception& ex) {
					log(log_level::error) << ex.what() << std::endl;
				}
			}

			return builders;
		}
		
		fs::path my_path;
		const logger& log = logger_factory::get_logger();
	};
public:
	file_manager() = delete;
	explicit file_manager(const std::string& path) {
		log_(log_level::info) << "<file manager> initialization..." << std::endl;
		
		const fs::directory_entry entry{ path };

		if (!entry.exists()) {
			log_(log_level::error) << "<file manager>: directory doesn't exist" << std::endl;
			return;
		}

		if (!entry.is_directory()) {
			log_(log_level::error) << "<file manager>: path isn't directory" << std::endl;
			return;
		}
		
		for (const auto& file : fs::directory_iterator(path)) {
			if (file.is_regular_file()) {
				files_.emplace_back(file.path());
			}
			else {
				log_(log_level::error) << "<file manager>: cannot deduce task from ";
				log_() << file.path().stem().string() << std::endl;
			}
		}

		if(files_.empty()) {
			log_(log_level::error) << "<file manager>: empty directory" << std::endl;
		}

		log_(log_level::info) << "<file manager> initialization done" << std::endl;
		log_(log_level::empty) << "<file manager>: number of files: " << files_.size() << std::endl;
	}

	[[nodiscard]] const std::vector<directory_file>& directory() const {
		return files_;
	}
	
private:

	
	std::vector<directory_file> files_{};
	const logger& log_ = logger_factory::get_logger();
};