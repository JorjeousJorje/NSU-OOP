//#include <sstream>
//
//#include "logger.hpp"
//
//template<class Strategy>
//void some_func(const logger<Strategy>& log)
//{
//	try {
//		throw std::range_error{ "test_exception" };
//	}
//	catch (const std::range_error& ex) {
//		log.log_message(log_level::fatal, ex.what());
//	}
//}
//int main(int argc, char* argv[])
//{
//	logger log{ console_log{} };
//	log.log_message(log_level::info, "info");
//	some_func(log);
//
//	auto new_log = log;
//	new_log.log_message(log_level::info, "new_info");
//	some_func(new_log);
//
//
//	std::ostringstream oss{};
//	logger slog{ file_log{oss} };
//	slog.log_message(log_level::error, "error");
//	std::cout << std::endl << oss.str();
//	oss.str("");
//	some_func(slog);
//	std::cout << oss.str();
//	oss.str("");
//
//	auto new_slog = slog;
//	new_slog.log_message(log_level::info, "new_info");
//	std::cout << oss.str();
//	oss.str("");
//	some_func(new_slog);
//	std::cout << oss.str();
//	oss.str("");
//    return 0;
//}
