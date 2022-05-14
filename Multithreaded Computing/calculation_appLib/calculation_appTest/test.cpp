#include "pch.h"
#include <fstream>
#include "calculation_app.hpp"
#include "config.hpp"
#include <random>
#include <array>
namespace fs = std::filesystem;

constexpr std::string_view tmpdir_base = "CALCULATION_APP_TEST_DIRECTORY";

auto random_data()
{
	std::array<std::mt19937::result_type, std::mt19937::state_size> data{};
	std::random_device rd;
	std::generate(std::begin(data), std::end(data), std::ref(rd));

	return data;
}
auto random_generator()
{
	auto const& data = random_data();
	std::seed_seq seeds(std::begin(data), std::end(data));
	const std::mt19937 generator{ seeds };

	return generator;
}

template<class T, class Dummy = std::enable_if<std::is_integral_v<T>>>
struct gen_int
{
	gen_int(const T min, const T max) : dist{ min, max }
	{}

	const std::uniform_int_distribution<T> dist;
	std::mt19937 engine{ random_generator() };

	auto operator()() {
		return dist(engine);
	}
};

std::string create_tmpdir()
{
	gen_int<unsigned> generator{ 0, std::numeric_limits<unsigned>::max() };
	auto dir = std::string{ tmpdir_base } + std::to_string(generator());
	while (fs::directory_entry{dir}.exists()) {
		dir = std::string{ tmpdir_base } + std::to_string(generator());
	}
	return dir;
}




class MemoryLeaksTests : public testing::Test
{
protected:

	void SetUp() override
	{
		_CrtMemCheckpoint(&startup_);
	}

	void TearDown() override
	{
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &startup_, &teardown)) << "Memory leaks detected";
	}
	_CrtMemState startup_ = {};
};

struct Commands : MemoryLeaksTests {};

namespace commands
{
	TEST(Commands, Add) {
		const add_command add{ 1, 2 };
		auto [cmd_name, cmd_res] = add.execute();

		EXPECT_EQ("add", cmd_name);
		EXPECT_EQ(3.0, cmd_res);
	}

	TEST(Commands, Add_Sq) {
		const add_sq_command add_sq{ 1, 2 };
		auto [cmd_name, cmd_res] = add_sq.execute();

		EXPECT_EQ("add_sq", cmd_name);
		EXPECT_EQ(5.0, cmd_res);
	}

	TEST(Commands, Div) {
		const div_command div{ 1, 2 };
		auto [cmd_name, cmd_res] = div.execute();

		EXPECT_EQ("div", cmd_name);
		EXPECT_EQ(0.5, cmd_res);
	}

	TEST(Commands, Mult) {
		const mult_command milt{ 5, 2 };
		auto [cmd_name, cmd_res] = milt.execute();

		EXPECT_EQ("mult", cmd_name);
		EXPECT_EQ(10.0, cmd_res);
	}

	TEST(Commands, Sq_Add) {
		const sq_add_command sq_add{ 5, 2 };
		auto [cmd_name, cmd_res] = sq_add.execute();

		EXPECT_EQ("sq_add", cmd_name);
		EXPECT_EQ(49.0, cmd_res);
	}

	TEST(Commands, Sub) {
		const sub_command sub{ 5, 2 };
		auto [cmd_name, cmd_res] = sub.execute();

		EXPECT_EQ("sub", cmd_name);
		EXPECT_EQ(3.0, cmd_res);
	}
}

struct Builders : MemoryLeaksTests {};

namespace builders
{
	TEST(Builders, Add_Builder) {
		const add_builder add_build{ 1, 2 };
		const auto cmd = add_build.construct();
		auto [cmd_name, cmd_res] = cmd->execute();

		EXPECT_EQ("add", cmd_name);
		EXPECT_EQ(3.0, cmd_res);
	}

	TEST(Builders, Add_Sq_Builder) {
		const add_sq_builder add_sq__build{ 1, 2 };
		const auto cmd = add_sq__build.construct();
		auto [cmd_name, cmd_res] = cmd->execute();

		EXPECT_EQ("add_sq", cmd_name);
		EXPECT_EQ(5.0, cmd_res);
	}

	TEST(Builders, Div_Builder) {
		const div_builder div_build{ 1, 2 };
		const auto cmd = div_build.construct();
		auto [cmd_name, cmd_res] = cmd->execute();

		EXPECT_EQ("div", cmd_name);
		EXPECT_EQ(0.5, cmd_res);
	}

	TEST(Builders, Mult_Builder) {
		const mult_builder mult_build{ 5, 2 };
		const auto cmd = mult_build.construct();
		auto [cmd_name, cmd_res] = cmd->execute();

		EXPECT_EQ("mult", cmd_name);
		EXPECT_EQ(10.0, cmd_res);
	}

	TEST(Builders, Sq_Add) {
		const sq_add_builder sq_add_build{ 5, 2 };
		const auto cmd = sq_add_build.construct();
		auto [cmd_name, cmd_res] = cmd->execute();

		EXPECT_EQ("sq_add", cmd_name);
		EXPECT_EQ(49.0, cmd_res);
	}

	TEST(Builders, Sub) {
		const sub_builder sub_build{ 5, 2 };
		const auto cmd = sub_build.construct();
		auto [cmd_name, cmd_res] = cmd->execute();

		EXPECT_EQ("sub", cmd_name);
		EXPECT_EQ(3.0, cmd_res);
	}
}

struct Commands_Parser : MemoryLeaksTests {};

namespace cmd_parser
{
	TEST(Commands_Parser, Any_Single_Command) {
		std::istringstream input
		{
		"add 3 4"
		};

		std::string line{};
		command_parser parser{ input };

		const auto builder = parser.parse_line();
		auto cmd = builder->construct();
		auto [cmd_name, cmd_res] = cmd->execute();

		double left{}, right{};

		input.seekg(0);

		input >> line;
		input >> left >> right;

		EXPECT_EQ(line, cmd_name);
		EXPECT_EQ(left + right, cmd_res);
	}
	TEST(Commands_Parser, Any_Multiple_Commands) {
		std::istringstream input
		{
		"add 3 4\n"
			 "div 3 7\n"
			 "add_sq 3 3\n"
			 "sub 3 54"
		};
		command_parser parser{ input };
		std::string line{};

		double left{}, right{};
		while (!parser.end()) {
			const std::size_t line_start = input.tellg();
			const auto builder = parser.parse_line();
			const std::size_t line_end = input.tellg();
			if (input.eof()) {
				input.clear();
			}

			auto cmd = builder->construct();
			auto [cmd_name, cmd_res] = cmd->execute();

			input.seekg(line_start);
			input >> line >> left >> right;
			input.seekg(line_end);

			EXPECT_EQ(line, cmd_name);
			if (cmd_name == "add") {
				EXPECT_EQ(left + right, cmd_res);
			}
			else if (cmd_name == "div") {
				EXPECT_EQ(left / right, cmd_res);
			}
			else if (cmd_name == "add_sq") {
				EXPECT_EQ(left * left + right * right, cmd_res);
			}
			else if (cmd_name == "sub") {
				EXPECT_EQ(left - right, cmd_res);
			}
		}

	}
	TEST(Commands_Parser, Unknown_Command) {
		std::istringstream input
		{
		"aadasddd 3 4\n"
		};
		command_parser parser{ input };

		EXPECT_THROW(parser.parse_line(), std::runtime_error);
	}
	TEST(Commands_Parser, Unknown_Commands) {
		std::istringstream input
		{
		"aadasddd 3 4\n"
			 "addd 3 4\n"
			 "1243\n"
			 "Sub 3 4\n"
			 "diV 3 4\n"
		};
		command_parser parser{ input };
		while (parser.end()) {
			EXPECT_THROW(parser.parse_line(), std::runtime_error);
		}
	}

}

struct Thread_Pool : MemoryLeaksTests {};

namespace threads
{
	TEST(Thread_Pool, Initialization) {
		const thread_pool pool{ 8 };
	}

	TEST(Thread_Pool, Max_Thread_Count) {
		const thread_pool pool{ 512 };

		EXPECT_EQ(std::thread::hardware_concurrency(), pool.number_of_workers());
	}

	TEST(Thread_Pool, Enqueue_One_Task) {
		thread_pool pool{ 7 };

		auto future = pool.insert_task([] { return 10 + 10; });
		auto result = future.get();

		EXPECT_EQ(result, 10 + 10);
	}

	TEST(Thread_Pool, Enqueue_Many_Tasks) {
		thread_pool pool{ 7 };

		std::size_t result{};
		std::size_t checkout{};

		for (std::size_t i = 0; i < 10; ++i) {
			auto task = [=] { return i * i; };
			auto future = pool.insert_task(task);
			result += future.get();
			checkout += i * i;
		}

		ASSERT_TRUE(result == checkout);
	}

	TEST(Thread_Pool, Enqueue_Command) {
		thread_pool pool{ 7 };

		add_command add{ 10, 10 };

		auto future = pool.insert_task([&] {return add.execute(); });
		auto [cmd_name, cmd_res] = future.get();

		EXPECT_EQ(cmd_res, 10 + 10);
	}
}

struct Calculation_App : MemoryLeaksTests {};



namespace application
{
	TEST(Calculation_App, Empty_Directory) {
		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();


		
		ASSERT_TRUE(fs::create_directories(tmpdir));

		const auto result_directory = fs::temp_directory_path().string() + "\\" + tmpdir;

		calculation_app app{ result_directory };

		ASSERT_TRUE(fs::remove(result_directory));
	}
	TEST(Calculation_App, Some_Empty_Files) {
		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();

		fs::create_directories(tmpdir);

		const auto result_directory = fs::temp_directory_path().string() + "\\" + tmpdir;

		const std::uintmax_t num_of_files = 10;
		for (auto i = 1u; i < num_of_files; ++i) {
			std::ofstream new_file{ result_directory + "/in_" + std::to_string(i) + ".txt" };
		}

		calculation_app app{ result_directory };

		app.calculate_from_files(5);

		auto deleted_files = fs::remove_all(result_directory);
		auto result = app.result();

		EXPECT_EQ(result, 0.0);
		EXPECT_EQ(deleted_files, num_of_files);
	}
	TEST(Calculation_App, Some_Files) {
		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();
		fs::create_directories(tmpdir);

		const auto result_directory = fs::temp_directory_path().string() + "\\" + tmpdir;

		const std::uintmax_t num_of_files = 11;
		const auto first_num = std::string{ "3" };
		const auto second_num = std::string{ "4" };

		for (auto i = 1u; i < num_of_files; ++i) {
			std::ofstream new_file{ result_directory + "/in_" + std::to_string(i) + ".txt" };
			new_file << "add" << ' ' << first_num << ' ' << second_num;
			new_file.close();
		}

		const auto to_num_first = std::stoul(first_num);
		const auto to_num_second = std::stoul(second_num);
		const auto test_result = (static_cast<unsigned long long>(to_num_first) + to_num_second) * (num_of_files - 1);

		calculation_app app{ result_directory };

		app.calculate_from_files(5);
		auto result = app.result();

		auto deleted_files = fs::remove_all(result_directory);
		EXPECT_EQ(result, static_cast<double>(test_result));
		EXPECT_EQ(deleted_files, num_of_files);
	}
}

struct Config : MemoryLeaksTests {};

namespace config_
{
	TEST(Config, SomeConfig) {
		config cfg{};

		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();
		ASSERT_TRUE(fs::create_directories(tmpdir));

		const auto result_directory = fs::path{ fs::temp_directory_path().string() + tmpdir };

		std::ofstream new_config{ result_directory.string() + "\\computation_config" };
		std::ofstream output{ result_directory.string() + "\\output" };


		current_path(result_directory);
		ASSERT_TRUE(fs::create_directories("input"));
		current_path(fs::temp_directory_path());

		const auto input_dir = result_directory.string() + "\\input";
		const auto output_dir = result_directory.string() + "\\output";

		new_config << "log=false" << std::endl;
		new_config << "type=false" << std::endl;
		new_config << "color=false" << std::endl;
		new_config << "filename=false" << std::endl;
		new_config << "log_level=false" << std::endl;

		new_config << "input=" << input_dir << std::endl;
		new_config << "output=" << output_dir << std::endl;
		new_config << "threads=5" << std::endl;
		std::ifstream config_file{ result_directory.string() + "\\computation_config" };

		EXPECT_NO_THROW(cfg.read(config_file));

		ASSERT_TRUE(cfg.look_up("input") == input_dir);
		ASSERT_TRUE(cfg.look_up("output") == output_dir);
		ASSERT_TRUE(cfg.thread_count() == 5);

		new_config.close();
		output.close();
		config_file.close();
		auto deleted_files = fs::remove_all(result_directory);
		EXPECT_EQ(deleted_files, 4);
	}
	TEST(Config, WrongInput) {
		config cfg{};

		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();
		ASSERT_TRUE(fs::create_directories(tmpdir));

		const auto result_directory = fs::path{ fs::temp_directory_path().string() + tmpdir };

		std::ofstream new_config{ result_directory.string() + "\\computation_config" };
		std::ofstream input{ result_directory.string() + "\\input" };


		const auto input_dir = result_directory.string() + "\\input";

		new_config << "log=false" << std::endl;
		new_config << "type=false" << std::endl;
		new_config << "color=false" << std::endl;
		new_config << "filename=false" << std::endl;
		new_config << "log_level=false" << std::endl;

		new_config << "input=" << input_dir << std::endl;
		new_config << "output=" << "output_dir" << std::endl;
		new_config << "threads=5" << std::endl;
		std::ifstream config_file{ result_directory.string() + "\\computation_config" };

		EXPECT_THROW(cfg.read(config_file), std::runtime_error);

		new_config.close();
		input.close();
		config_file.close();
		auto deleted_files = fs::remove_all(result_directory);
		EXPECT_EQ(deleted_files, 3);
	}
	TEST(Config, WrongOutput) {
		config cfg{};

		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();
		ASSERT_TRUE(fs::create_directories(tmpdir));

		const auto result_directory = fs::path{ fs::temp_directory_path().string() + tmpdir };

		std::ofstream new_config{ result_directory.string() + "\\computation_config" };

		current_path(result_directory);
		ASSERT_TRUE(fs::create_directories("input"));
		current_path(fs::temp_directory_path());

		const auto input_dir = result_directory.string() + "\\input";


		new_config << "log=false" << std::endl;
		new_config << "type=false" << std::endl;
		new_config << "color=false" << std::endl;
		new_config << "filename=false" << std::endl;
		new_config << "log_level=false" << std::endl;

		new_config << "input=" << input_dir << std::endl;
		new_config << "output=" << input_dir << std::endl;
		new_config << "threads=5" << std::endl;
		std::ifstream config_file{ result_directory.string() + "\\computation_config" };

		EXPECT_THROW(cfg.read(config_file), std::runtime_error);

		new_config.close();
		config_file.close();
		auto deleted_files = fs::remove_all(result_directory);
		EXPECT_EQ(deleted_files, 3);
	}
	TEST(Config, InvalidThreadCount) {
		config cfg{};

		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();
		ASSERT_TRUE(fs::create_directories(tmpdir));

		const auto result_directory = fs::path{ fs::temp_directory_path().string() + tmpdir };

		std::ofstream new_config{ result_directory.string() + "\\computation_config" };
		std::ofstream output{ result_directory.string() + "\\output" };


		current_path(result_directory);
		ASSERT_TRUE(fs::create_directories("input"));
		current_path(fs::temp_directory_path());

		const auto input_dir = result_directory.string() + "\\input";
		const auto output_dir = result_directory.string() + "\\output";

		new_config << "log=false" << std::endl;
		new_config << "type=false" << std::endl;
		new_config << "color=false" << std::endl;
		new_config << "filename=false" << std::endl;
		new_config << "log_level=false" << std::endl;

		new_config << "input=" << input_dir << std::endl;
		new_config << "output=" << output_dir << std::endl;
		new_config << "threads=wefwef" << std::endl;
		std::ifstream config_file{ result_directory.string() + "\\computation_config" };

		EXPECT_NO_THROW(cfg.read(config_file));

		ASSERT_TRUE(cfg.thread_count() == 1);

		new_config.close();
		output.close();
		config_file.close();
		auto deleted_files = fs::remove_all(result_directory);
		EXPECT_EQ(deleted_files, 4);
	}
	TEST(Config, EmptyConfig) {
		config cfg{};

		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();
		ASSERT_TRUE(fs::create_directories(tmpdir));

		const auto result_directory = fs::path{ fs::temp_directory_path().string() + tmpdir };

		std::ofstream new_config{ result_directory.string() + "\\computation_config" };

		std::ifstream config_file{ result_directory.string() + "\\computation_config" };

		EXPECT_THROW(cfg.read(config_file), std::runtime_error);

		new_config.close();
		config_file.close();
		auto deleted_files = fs::remove_all(result_directory);
		EXPECT_EQ(deleted_files, 2);
	}
}

struct Combination : MemoryLeaksTests {};

namespace combination
{
	TEST(Combination, Config_Application) {
		config cfg{};

		current_path(fs::temp_directory_path());
		const auto tmpdir = create_tmpdir();
		ASSERT_TRUE(fs::create_directories(tmpdir));

		const auto result_directory = fs::path{ fs::temp_directory_path().string() + tmpdir };

		std::ofstream new_config{ result_directory.string() + "\\computation_config" };
		std::ofstream output{ result_directory.string() + "\\output" };


		current_path(result_directory);
		ASSERT_TRUE(fs::create_directories("input"));
		current_path(fs::temp_directory_path());

		const auto input_dir = result_directory.string() + "\\input";
		const auto output_dir = result_directory.string() + "\\output";

		new_config << "log=true" << std::endl;
		new_config << "type=std_out" << std::endl;
		new_config << "color=true" << std::endl;
		new_config << "filename=false" << std::endl;
		new_config << "log_level=debug" << std::endl;

		new_config << "input=" << input_dir << std::endl;
		new_config << "output=" << output_dir << std::endl;
		new_config << "threads=5" << std::endl;
		std::ifstream config_file{ result_directory.string() + "\\computation_config" };

		EXPECT_NO_THROW(cfg.read(config_file));

		ASSERT_TRUE(cfg.look_up("input") == input_dir);
		ASSERT_TRUE(cfg.look_up("output") == output_dir);
		ASSERT_TRUE(cfg.thread_count() == 5);

		const std::uintmax_t num_of_files = 11;
		const auto first_num = std::string{ "3" };
		const auto second_num = std::string{ "4" };

		for (auto i = 1u; i < num_of_files; ++i) {
			std::ofstream new_file{ cfg.look_up("input") + "/in_" + std::to_string(i) + ".txt" };
			new_file << "add" << ' ' << first_num << ' ' << second_num;
			new_file.close();
		}

		calculation_app app{ cfg.look_up("input") };

		const auto to_num_first = std::stoul(first_num);
		const auto to_num_second = std::stoul(second_num);
		const auto test_result = (static_cast<unsigned long long>(to_num_first) + to_num_second) * (num_of_files - 1);

		app.calculate_from_files(cfg.thread_count());
		auto result = app.result();

		EXPECT_EQ(result, static_cast<double>(test_result));

		new_config.close();
		output.close();
		config_file.close();
		auto deleted_files = fs::remove_all(result_directory);
		EXPECT_EQ(deleted_files, 14);
	}
}