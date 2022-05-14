#include "pch.h"

#include <fstream>

#include "logger.hpp"

TEST(ConsoleLogger, LogWithoutNameAndLine) {
    logger log{ console_log{} };
    log.log_message(log_level::fatal, "console, fatal message");
    log.log_message(log_level::error, "console, error message");
    log.log_message(log_level::warning, "console, warning message");
    log.log_message(log_level::info, "console, info message");
    log.log_message(log_level::debug, "console, debug message");
}

TEST(ConsoleLogger, LogWithNameAndLine) {
    logger log{ console_log{} };
    log.LOG_MSG(log_level::fatal, "console, fatal message");
    log.LOG_MSG(log_level::error, "console, error message");
    log.LOG_MSG(log_level::warning, "console, warning message");
    log.LOG_MSG(log_level::info, "console, info message");
    log.LOG_MSG(log_level::debug, "console, debug message");
}

TEST(ConsoleTimeLogger, LogWithoutNameAndLine) {
    logger log{ console_time_log{} };
    log.log_message(log_level::fatal, "time_console, fatal message");
    log.log_message(log_level::error, "time_console, error message");
    log.log_message(log_level::warning, "time_console, warning message");
    log.log_message(log_level::info, "time_console, info message");
    log.log_message(log_level::debug, "time_console, debug message");
}

TEST(ConsoleTimeLogger, LogWithNameAndLine) {
    logger log{ console_time_log{} };
    log.LOG_MSG(log_level::fatal, "time_console, fatal message");
    log.LOG_MSG(log_level::error, "time_console, error message");
    log.LOG_MSG(log_level::warning, "time_console, warning message");
    log.LOG_MSG(log_level::info, "time_console, info message");
    log.LOG_MSG(log_level::debug, "time_console, debug message");
}

TEST(FileLoggerCout, LogWithoutNameAndLine) {
    logger log{ file_log{std::cout} };
    log.log_message(log_level::fatal, "file_cout, fatal message");
    log.log_message(log_level::error, "file_cout, error message");
    log.log_message(log_level::warning, "file_cout, warning message");
    log.log_message(log_level::info, "file_cout, info message");
    log.log_message(log_level::debug, "file_cout, debug message");
}

TEST(FileLoggerCout, LogWithNameAndLine) {
    logger log{ file_log{std::cout} };
    log.LOG_MSG(log_level::fatal, "file_cout, fatal message");
    log.LOG_MSG(log_level::error, "file_cout, error message");
    log.LOG_MSG(log_level::warning, "file_cout, warning message");
    log.LOG_MSG(log_level::info, "file_cout, info message");
    log.LOG_MSG(log_level::debug, "file_cout, debug message");
}

TEST(FileTimeLoggerCout, LogWithoutNameAndLine) {
    logger log{ file_time_log{std::cout} };
    log.log_message(log_level::fatal, "time_file_cout, fatal message");
    log.log_message(log_level::error, "time_file_cout, error message");
    log.log_message(log_level::warning, "time_file_cout, warning message");
    log.log_message(log_level::info, "time_file_cout, info message");
    log.log_message(log_level::debug, "time_file_cout, debug message");
}

TEST(FileTimeLoggerCout, LogWithNameAndLine) {
    logger log{ file_time_log{std::cout} };
    log.LOG_MSG(log_level::fatal, "time_file_cout, fatal message");
    log.LOG_MSG(log_level::error, "time_file_cout, error message");
    log.LOG_MSG(log_level::warning, "time_file_cout, warning message");
    log.LOG_MSG(log_level::info, "time_file_cout, info message");
    log.LOG_MSG(log_level::debug, "time_file_cout, debug message");
}

TEST(FileLoggerOfs, LogWithoutNameAndLine) {
    std::ofstream ofs{ "gtest_flog_wo_fn.txt" };
    logger log{ file_log{ofs} };
    std::ostringstream oss{};
    log.log_message(log_level::fatal, "file_ofs, fatal message");
    oss << "FATAL: file_ofs, fatal message\n";
    log.log_message(log_level::error, "file_ofs, error message");
    oss << "ERROR: file_ofs, error message\n";
    log.log_message(log_level::warning, "file_ofs, warning message");
    oss << "WARNING: file_ofs, warning message\n";
    log.log_message(log_level::info, "file_ofs, info message");
    oss << "INFO: file_ofs, info message\n";
    log.log_message(log_level::debug, "file_ofs, debug message");
    oss << "DEBUG: file_ofs, debug message\n";

	
    std::ifstream ifs{ "gtest_flog_wo_fn.txt" };
    std::string buffer{};
    for (std::string line; std::getline(ifs, line); ) {
        buffer += line + '\n';
    }
    EXPECT_EQ(oss.str(), buffer);
}

std::string current_time()
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestamp[26];
    ctime_s(timestamp, sizeof timestamp, &now);

    const std::string timestamp_to_str(timestamp);
    return timestamp_to_str.substr(11, 8);
}
TEST(FileLoggerOfs, LogWithNameAndLine) {
    std::ofstream ofs{ "gtest_flog_w_fn.txt" };
    logger log{ file_log{ofs} };
    std::ostringstream oss{};
    log.LOG_MSG(log_level::fatal, "file_ofs, fatal message");
    oss <<"FATAL: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, fatal message\n";
    log.LOG_MSG(log_level::error, "file_ofs, error message");
    oss << "ERROR: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, error message\n";
    log.LOG_MSG(log_level::warning, "file_ofs, warning message");
    oss << "WARNING: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, warning message\n";
    log.LOG_MSG(log_level::info, "file_ofs, info message");
    oss << "INFO: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, info message\n";
    log.LOG_MSG(log_level::debug, "file_ofs, debug message");
    oss << "DEBUG: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, debug message\n";

	
    std::ifstream ifs{ "gtest_flog_w_fn.txt" };
    std::string buffer{};
    for (std::string line; std::getline(ifs, line); ) {
        buffer += line + '\n';
    }
    EXPECT_EQ(oss.str(), buffer);
}

TEST(TimeFileLoggerOfs, LogWithoutNameAndLine) {
    std::ofstream ofs{ "gtest_tflog_wo_fn.txt" };
    logger log{ file_time_log{ofs} };
    std::ostringstream oss{};
    log.log_message(log_level::fatal, "tfile_ofs, fatal message");
    oss << "[" + current_time() + "] " + "FATAL: tfile_ofs, fatal message\n";
    log.log_message(log_level::error, "tfile_ofs, error message");
    oss << "[" + current_time() + "] " + "ERROR: tfile_ofs, error message\n";
    log.log_message(log_level::warning, "tfile_ofs, warning message");
    oss << "[" + current_time() + "] " + "WARNING: tfile_ofs, warning message\n";
    log.log_message(log_level::info, "tfile_ofs, info message");
    oss << "[" + current_time() + "] " + "INFO: tfile_ofs, info message\n";
    log.log_message(log_level::debug, "tfile_ofs, debug message");
    oss << "[" + current_time() + "] " + "DEBUG: tfile_ofs, debug message\n";


    std::ifstream ifs{ "gtest_tflog_wo_fn.txt" };
    std::string buffer{};
    for (std::string line; std::getline(ifs, line); ) {
        buffer += line + '\n';
    }
    EXPECT_EQ(oss.str(), buffer);
}

TEST(TimeFileLoggerOfs, LogWithNameAndLine) {
    std::ofstream ofs{ "gtest_tflog_w_fn.txt" };
    logger log{ file_time_log{ofs} };
    std::ostringstream oss{};
    log.LOG_MSG(log_level::fatal, "file_ofs, fatal message");
    oss << "[" + current_time() + "] " + "FATAL: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, fatal message\n";
    log.LOG_MSG(log_level::error, "file_ofs, error message");
    oss << "[" + current_time() + "] " + "ERROR: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, error message\n";
    log.LOG_MSG(log_level::warning, "file_ofs, warning message");
    oss << "[" + current_time() + "] " + "WARNING: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, warning message\n";
    log.LOG_MSG(log_level::info, "file_ofs, info message");
    oss << "[" + current_time() + "] " + "INFO: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, info message\n";
    log.LOG_MSG(log_level::debug, "file_ofs, debug message");
    oss << "[" + current_time() + "] " + "DEBUG: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_ofs, debug message\n";


    std::ifstream ifs{ "gtest_tflog_w_fn.txt" };
    std::string buffer{};
    for (std::string line; std::getline(ifs, line); ) {
        buffer += line + '\n';
    }
    EXPECT_EQ(oss.str(), buffer);

}

TEST(FileLoggerOss, LogWithoutNameAndLine) {
    std::ostringstream ss{};
    logger log{ file_log{ss} };
    log.log_message(log_level::fatal, "file_oss, fatal message");
    EXPECT_EQ(ss.str(), "FATAL: file_oss, fatal message\n");
    ss.str(std::string{});
    log.log_message(log_level::error, "file_oss, error message");
    EXPECT_EQ(ss.str(), "ERROR: file_oss, error message\n");
    ss.str(std::string{});
    log.log_message(log_level::warning, "file_oss, warning message");
    EXPECT_EQ(ss.str(), "WARNING: file_oss, warning message\n");
    ss.str(std::string{});
    log.log_message(log_level::info, "file_oss, info message");
    EXPECT_EQ(ss.str(), "INFO: file_oss, info message\n");
    ss.str(std::string{});
    log.log_message(log_level::debug, "file_oss, debug message");
    EXPECT_EQ(ss.str(), "DEBUG: file_oss, debug message\n");
}

TEST(FileLoggerOss, LogWithNameAndLine) {
    std::ostringstream ss{};
    logger log{ file_log{ss} };
    log.LOG_MSG(log_level::fatal, "file_oss, fatal message");
    auto string = "FATAL: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, fatal message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.LOG_MSG(log_level::error, "file_oss, error message");
    string = "ERROR: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, error message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.LOG_MSG(log_level::warning, "file_oss, warning message");
    string = "WARNING: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, warning message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.LOG_MSG(log_level::info, "file_oss, info message");
    string = "INFO: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, info message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.LOG_MSG(log_level::debug, "file_oss, debug message");
    string = "DEBUG: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, debug message\n";
    EXPECT_EQ(ss.str(), string);
}

TEST(TimeFileLoggerOss, LogWithoutNameAndLine) {
    std::ostringstream ss{};
    logger log{ file_time_log{ss} };
    log.log_message(log_level::fatal, "file_oss, fatal message");
    auto string = "[" + current_time() + "] " + "FATAL: file_oss, fatal message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.log_message(log_level::error, "file_oss, error message");
    string = "[" + current_time() + "] " + "ERROR: file_oss, error message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.log_message(log_level::warning, "file_oss, warning message");
    string = "[" + current_time() + "] " + "WARNING: file_oss, warning message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.log_message(log_level::info, "file_oss, info message");
    string = "[" + current_time() + "] " + "INFO: file_oss, info message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.log_message(log_level::debug, "file_oss, debug message");
    string = "[" + current_time() + "] " + "DEBUG: file_oss, debug message\n";
    EXPECT_EQ(ss.str(), string);
}

TEST(TimeFileLoggerOss, LogWithNameAndLine) {
    std::ostringstream ss{};
    logger log{ file_time_log{ss} };
    log.LOG_MSG(log_level::fatal, "file_oss, fatal message");
    auto string = "[" + current_time() + "] " + "FATAL: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, fatal message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.LOG_MSG(log_level::error, "file_oss, error message");
    string = "[" + current_time() + "] " + "ERROR: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, error message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.LOG_MSG(log_level::warning, "file_oss, warning message");
    string = "[" + current_time() + "] " + "WARNING: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, warning message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.LOG_MSG(log_level::info, "file_oss, info message");
    string = "[" + current_time() + "] " + "INFO: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, info message\n";
    EXPECT_EQ(ss.str(), string);
    ss.str(std::string{});
    log.LOG_MSG(log_level::debug, "file_oss, debug message");
    string = "[" + current_time() + "] " + "DEBUG: (test.cpp, " + std::to_string(__LINE__ - 1) + ") file_oss, debug message\n";
    EXPECT_EQ(ss.str(), string);
}

template<class Strategy>
void some_func(const logger<Strategy>& log)
{
	try {
        throw std::range_error{ "test_exception" };
	}
	catch (const std::range_error& ex) {
        log.log_message(log_level::fatal, ex.what());
	}
}

TEST(ConsoleLogger, SEH_exception) {
    logger log{ console_log{} };
    some_func(log);
}


TEST(ConsoleTimeLogger, SEH_exception) {
    logger log{ console_time_log{} };
    some_func(log);
}

TEST(FileLoggerCout, SEH_exception) {
    logger log{ file_log{std::cout} };
    some_func(log);
}

TEST(FileTimeLoggerCout, SEH_exception) {
    logger log{ file_time_log{std::cout} };
    some_func(log);
}


TEST(FileLoggerOfs, SEH_exception) {
    std::ofstream ofs{ "gtest_flog.txt" };
    logger log{ file_log{ofs} };
    some_func(log);
}

TEST(TimeFileLoggerOfs, SEH_exception) {
    std::ofstream ofs{ "gtest_tflog.txt" };
    logger log{ file_log{ofs} };
    some_func(log);
}


TEST(FileLoggerOss, SEH_exception) {
    std::ostringstream ss{};
    logger log{ file_log{ss} };
    some_func(log);
    EXPECT_EQ(ss.str(), "FATAL: test_exception\n");
}


TEST(TimeFileLoggerOss, SEH_exception) {
    std::ostringstream ss{};
    logger log{ file_time_log{ss} };
    some_func(log);
    auto string = "[" + current_time() + "] " + "FATAL: test_exception\n";
    EXPECT_EQ(ss.str(), string);
}