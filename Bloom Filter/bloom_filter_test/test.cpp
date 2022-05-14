#include "pch.h"
#include "bloom_filter.hpp"
#include "source_bloom_filter.h"
#include <functional>
#include "logger.hpp"


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


namespace refactored
{
	struct RefactoredInsertQuery : MemoryLeaksTests {};

	TEST_F(RefactoredInsertQuery, OneInteger) {

		bloom_filter<int> filter{};

		filter.insert(10);

		ASSERT_TRUE(filter.query(10));
		ASSERT_FALSE(filter.query(11));
	}


	TEST_F(RefactoredInsertQuery, ManyInters) {

		bloom_filter<int, 16000, 15> filter{};


		for (auto i = 0; i < 1000; ++i) {
			filter.insert(i);
		}

		for (auto i = 0; i < 1000; ++i) {
			ASSERT_TRUE(filter.query(i));
		}

		for (auto i = 3000; i < 4000; ++i) {
			ASSERT_FALSE(filter.query(i));
		}
	}


	TEST_F(RefactoredInsertQuery, OneDouble) {

		bloom_filter<double> filter{};

		filter.insert(10.0);

		ASSERT_TRUE(filter.query(10.0));
		ASSERT_FALSE(filter.query(11.0));
	}


	TEST_F(RefactoredInsertQuery, ManyDoubles) {

		bloom_filter<double, 17000, 15> filter{};


		for (auto i = 0; i < 1000; ++i) {
			auto double_i = i + 0.01;
			filter.insert(double_i);
		}

		for (auto i = 0; i < 1000; ++i) {
			auto double_i = i + 0.01;
			ASSERT_TRUE(filter.query(double_i));
		}

		for (auto i = 3000; i < 4000; ++i) {
			auto double_i = i + 0.01;
			ASSERT_FALSE(filter.query(double_i));
		}
	}


	TEST_F(RefactoredInsertQuery, OneString) {

		bloom_filter<std::string> filter{};

		filter.insert("10");

		ASSERT_TRUE(filter.query("10"));
		ASSERT_FALSE(filter.query("11"));
	}


	TEST_F(RefactoredInsertQuery, ManyStrings) {

		bloom_filter<std::string, 17000, 15> filter{};

		//std::cout << "precision for 17(15 hash) and 2000 is: " << get_precision(16000, 2000, 15) << std::endl;

		for (auto i = 0; i < 1000; ++i) {
			filter.insert(std::to_string(i));
		}

		for (auto i = 0; i < 1000; ++i) {
			ASSERT_TRUE(filter.query(std::to_string(i)));
		}

		for (auto i = 3000; i < 4000; ++i) {
			ASSERT_FALSE(filter.query(std::to_string(i)));
		}
	}


	TEST_F(RefactoredInsertQuery, OneVector) {

		bloom_filter<std::vector<bool>> filter{};

		const std::vector<bool> was_in_filter = { true, false, false };
		filter.insert(was_in_filter);

		ASSERT_TRUE(filter.query(was_in_filter));

		const std::vector<bool> was_not_in_filter = { false, false, false };
		ASSERT_FALSE(filter.query(was_not_in_filter));
	}


	TEST_F(RefactoredInsertQuery, ManyVectors) {

		bloom_filter<std::vector<bool>, 5000, 15> filter{};

		for (auto i = 0; i < 250; i += 2) {
			std::vector<bool> was_in_filter = { i % 2 == 0 };

			for (auto j = 0; j < i; ++j) {
				was_in_filter.push_back(i % 2 == 0);
			}

			filter.insert(was_in_filter);
		}

		for (auto i = 0; i < 250; i += 2) {
			std::vector<bool> was_in_filter = { i % 2 == 0 };

			for (auto j = 0; j < i; ++j) {
				was_in_filter.push_back(i % 2 == 0);
			}

			ASSERT_TRUE(filter.query(was_in_filter));
		}

		for (auto i = 1; i < 250; i += 2) {
			std::vector<bool> was_not_in_filter = { i % 2 == 0 };

			for (auto j = 0; j < i; ++j) {
				was_not_in_filter.push_back(i % 2 == 0);
			}

			ASSERT_FALSE(filter.query(was_not_in_filter));
		}
	}
	

	struct RefactoredUnion : MemoryLeaksTests {};

	TEST_F(RefactoredUnion, WithOneInteger) {

		bloom_filter<int> one_filter{};

		one_filter.insert(10);

		ASSERT_TRUE(one_filter.query(10));
		ASSERT_FALSE(one_filter.query(11));

		bloom_filter<int> second_filter{};

		second_filter.insert(11);

		ASSERT_TRUE(second_filter.query(11));
		ASSERT_FALSE(second_filter.query(10));


		auto united = one_filter | second_filter;

		ASSERT_TRUE(united.query(11));
		ASSERT_TRUE(united.query(10));
	}
	

	TEST_F(RefactoredUnion, WithManyInteger) {

		bloom_filter<int, 16000, 15> one_filter{};


		for (auto i = 0; i < 1000; ++i) {
			one_filter.insert(i);
		}

		bloom_filter<int, 16000, 15> second_filter{};

		for (auto i = 1000; i < 2000; ++i) {
			second_filter.insert(i);
		}

		auto united = one_filter | second_filter;

		for (auto i = 0; i < 2000; ++i) {
			ASSERT_TRUE(united.query(i));
		}
	}
	

	TEST_F(RefactoredUnion, WithTheSameFilter) {

		bloom_filter<int> one_filter{};

		one_filter.insert(10);

		ASSERT_TRUE(one_filter.query(10));
		ASSERT_FALSE(one_filter.query(11));

		auto united = one_filter | one_filter;

		ASSERT_TRUE(united.query(10));
		ASSERT_FALSE(united.query(11));
	}


	struct RefactoredIntersection : MemoryLeaksTests {};

	TEST_F(RefactoredIntersection, EmptyResultSet) {

		bloom_filter<int> one_filter{};

		one_filter.insert(10);

		ASSERT_TRUE(one_filter.query(10));
		ASSERT_FALSE(one_filter.query(11));

		bloom_filter<int> second_filter{};

		second_filter.insert(11);

		ASSERT_TRUE(second_filter.query(11));
		ASSERT_FALSE(second_filter.query(10));


		auto united = one_filter & second_filter;

		ASSERT_FALSE(united.query(11));
		ASSERT_FALSE(united.query(10));
	}
	

	TEST_F(RefactoredIntersection, OneIntInResultSet) {

		bloom_filter<int> one_filter{};

		one_filter.insert(10);
		one_filter.insert(12);

		ASSERT_TRUE(one_filter.query(10));
		ASSERT_TRUE(one_filter.query(12));
		ASSERT_FALSE(one_filter.query(11));

		bloom_filter<int> second_filter{};

		second_filter.insert(11);
		second_filter.insert(12);

		ASSERT_TRUE(second_filter.query(11));
		ASSERT_TRUE(second_filter.query(12));
		ASSERT_FALSE(second_filter.query(10));


		auto united = one_filter & second_filter;

		ASSERT_FALSE(united.query(10));
		ASSERT_FALSE(united.query(11));
		ASSERT_TRUE(second_filter.query(12));
	}


	TEST_F(RefactoredIntersection, WithManyInteger) {

		bloom_filter<int, 16000, 15> one_filter{};

		for (auto i = 0; i < 750; ++i) {
			one_filter.insert(i);
		}

		bloom_filter<int, 16000, 15> second_filter{};

		for (auto i = 500; i < 1000; ++i) {
			second_filter.insert(i);
		}

		auto united = one_filter & second_filter;

		for (auto i = 500; i < 750; ++i) {
			ASSERT_TRUE(united.query(i));
		}
	}


	TEST_F(RefactoredIntersection, WithTheSameFilter) {

		bloom_filter<int> one_filter{};

		one_filter.insert(10);

		ASSERT_TRUE(one_filter.query(10));
		ASSERT_FALSE(one_filter.query(11));

		auto united = one_filter & one_filter;

		ASSERT_TRUE(united.query(10));
		ASSERT_FALSE(united.query(11));
	}
	

	struct RefactoredEqualityOperator : MemoryLeaksTests {};
	
	TEST_F(RefactoredEqualityOperator, Equal) {

		bloom_filter<int> one_filter{};
		bloom_filter<int> second_filter{};

		one_filter.insert(10);
		second_filter.insert(10);

		ASSERT_TRUE(one_filter.query(10));
		ASSERT_TRUE(second_filter.query(10));

		ASSERT_TRUE(one_filter == second_filter);
	}

	TEST_F(RefactoredEqualityOperator, NotEqual) {

		bloom_filter<int> one_filter{};
		bloom_filter<int> second_filter{};

		one_filter.insert(10);
		second_filter.insert(11);

		ASSERT_TRUE(one_filter.query(10));
		ASSERT_TRUE(second_filter.query(11));

		ASSERT_FALSE(one_filter == second_filter);
	}

}


namespace source
{
	struct SourceInsertQuery : MemoryLeaksTests {};

	
	TEST_F(SourceInsertQuery, OneInteger) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<int>{}(*static_cast<int*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(100, hash_func, 15);

		auto was_in_filter = 10;
		bloom_filter_insert(bloom_filter_c, &was_in_filter);

		ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_in_filter), 1);

		auto was_not_in_filter = 11;
		ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_not_in_filter), 0);

		bloom_filter_free(bloom_filter_c);
	}


	TEST_F(SourceInsertQuery, ManyInters) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<int>{}(*static_cast<int*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(16000, hash_func, 15);

		for (auto i = 0; i < 1000; ++i) {
			bloom_filter_insert(bloom_filter_c, &i);
		}

		for (auto i = 0; i < 1000; ++i) {
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i), 1);
		}

		for (auto i = 3000; i < 4000; ++i) {
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i), 0);
		}

		bloom_filter_free(bloom_filter_c);
	}


	TEST_F(SourceInsertQuery, OneDouble) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<double>{}(*static_cast<double*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(100, hash_func, 15);

		auto was_in_filter = 10.0;
		bloom_filter_insert(bloom_filter_c, &was_in_filter);

		ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_in_filter), 1);

		auto was_not_in_filter = 11.0;
		ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_not_in_filter), 0);

		bloom_filter_free(bloom_filter_c);
	}


	TEST_F(SourceInsertQuery, ManyDoubles) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<double>{}(*static_cast<double*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(17000, hash_func, 15);

		for (auto i = 0; i < 1000; ++i) {
			auto double_i = i + 0.01;
			bloom_filter_insert(bloom_filter_c, &double_i);
		}

		for (auto i = 0; i < 1000; ++i) {
			auto double_i = i + 0.01;
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &double_i), 1);
		}

		for (auto i = 3000; i < 4000; ++i) {
			auto double_i = i + 0.01;
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &double_i), 0);
		}

		bloom_filter_free(bloom_filter_c);
	}


	TEST_F(SourceInsertQuery, OneString) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::string>{}(*static_cast<std::string*> (data))); };

		auto* bloom_filter_c = bloom_filter_new(100, hash_func, 15);

		std::string was_in_filter = "10";
		bloom_filter_insert(bloom_filter_c, &was_in_filter);

		ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_in_filter), 1);

		std::string was_not_in_filter = "11";
		ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_not_in_filter), 0);

		bloom_filter_free(bloom_filter_c);
	}


	TEST_F(SourceInsertQuery, ManyStrings) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::string>{}(*static_cast<std::string*> (data))); };

		auto* bloom_filter_c = bloom_filter_new(16000, hash_func, 15);

		for (auto i = 0; i < 1000; ++i) {
			std::string i_to_str{ std::to_string(i) };
			bloom_filter_insert(bloom_filter_c, &i_to_str);
		}

		for (auto i = 0; i < 1000; ++i) {
			std::string i_to_str{ std::to_string(i) };
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i_to_str), 1);
		}

		for (auto i = 3000; i < 4000; ++i) {
			std::string i_to_str{ std::to_string(i) };
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i_to_str), 0);
		}

		bloom_filter_free(bloom_filter_c);
	}


	TEST_F(SourceInsertQuery, OneVector) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::vector<bool>>{}(*static_cast<std::vector<bool>*> (data))); };

		auto* bloom_filter_c = bloom_filter_new(100, hash_func, 15);

		std::vector<bool> was_in_filter = {true, false, false};
		bloom_filter_insert(bloom_filter_c, &was_in_filter);

		ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_in_filter), 1);

		std::vector<bool> was_not_in_filter = { false, false, false };
		ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_not_in_filter), 0);

		bloom_filter_free(bloom_filter_c);
	}
	

	TEST_F(SourceInsertQuery, ManyVectors) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::vector<bool>>{}(*static_cast<std::vector<bool>*> (data))); };

		auto* bloom_filter_c = bloom_filter_new(5000, hash_func, 15);


		for (auto i = 0; i < 250; i += 2) {
			std::vector<bool> was_in_filter = { i % 2 == 0 };

			for (auto j = 0; j < i; ++j) {
				was_in_filter.push_back(i % 2 == 0);
			}

			bloom_filter_insert(bloom_filter_c, &was_in_filter);
		}


		for (auto i = 0; i < 250; i += 2) {
			std::vector<bool> was_in_filter = { i % 2 == 0 };

			for (auto j = 0; j < i; ++j) {
				was_in_filter.push_back(i % 2 == 0);
			}

			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_in_filter), 1);
		}

		for (auto i = 1; i < 250; i += 2) {
			std::vector<bool> was_not_in_filter = { i % 2 == 0 };

			for (auto j = 0; j < i; ++j) {
				was_not_in_filter.push_back(i % 2 == 0);
			}

			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &was_not_in_filter), 0);
		}

		bloom_filter_free(bloom_filter_c);
	}
}


namespace performance_tests
{
	constexpr int cnt = 1000;
	constexpr int min_false = 3000;
	constexpr int max_false = 4000;
	constexpr int result_set_count = max_false - min_false + cnt;

	constexpr size_t initial_table_size = 10000;
	constexpr size_t initial_function_number = 20;

	constexpr size_t tb_size_decrease_rate = 3000;
	constexpr size_t func_num_decrease_rate = 15;
	constexpr size_t func_num_increase_rate = 30;
	
	struct ErrorRateIntegers : MemoryLeaksTests {};

	TEST_F(ErrorRateIntegers, InitialTest) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<int>{}(*static_cast<int*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size, hash_func, initial_function_number);

		
		for (auto i = 0; i < cnt; ++i) {
			bloom_filter_insert(bloom_filter_c, &i);
		}
		
		for (auto i = 0; i < cnt; ++i) {
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i), 1);
		}
		
		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			source_false_positives += bloom_filter_query(bloom_filter_c, &i);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size << ". Number of functions: " << initial_function_number << std::endl;
		
		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<int, initial_table_size, initial_function_number> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(i));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(i);
		}

		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}

	TEST_F(ErrorRateIntegers, DecreaseTableSize) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<int>{}(*static_cast<int*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size - tb_size_decrease_rate, hash_func, initial_function_number);


		for (auto i = 0; i < cnt; ++i) {
			bloom_filter_insert(bloom_filter_c, &i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			source_false_positives += bloom_filter_query(bloom_filter_c, &i);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size - tb_size_decrease_rate << ". Number of functions: " << initial_function_number << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<int, initial_table_size - tb_size_decrease_rate, initial_function_number> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(i));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(i);
		}

		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}

	TEST_F(ErrorRateIntegers, DecreaseFuncNum) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<int>{}(*static_cast<int*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size, hash_func, initial_function_number - func_num_decrease_rate);


		for (auto i = 0; i < cnt; ++i) {
			bloom_filter_insert(bloom_filter_c, &i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			source_false_positives += bloom_filter_query(bloom_filter_c, &i);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size << ". Number of functions: " << initial_function_number - func_num_decrease_rate << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<int, initial_table_size, initial_function_number - func_num_decrease_rate> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(i));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(i);
		}

		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}

	TEST_F(ErrorRateIntegers, IncreaseFuncNum) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<int>{}(*static_cast<int*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size, hash_func, initial_function_number + func_num_increase_rate);


		for (auto i = 0; i < cnt; ++i) {
			bloom_filter_insert(bloom_filter_c, &i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			source_false_positives += bloom_filter_query(bloom_filter_c, &i);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size << ". Number of functions: " << initial_function_number + func_num_increase_rate << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<int, initial_table_size, initial_function_number + func_num_increase_rate> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(i));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(i);
		}

		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}

	TEST_F(ErrorRateIntegers, DecreaseBoth) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<int>{}(*static_cast<int*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size - tb_size_decrease_rate, hash_func, initial_function_number - func_num_decrease_rate);


		for (auto i = 0; i < cnt; ++i) {
			bloom_filter_insert(bloom_filter_c, &i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			source_false_positives += bloom_filter_query(bloom_filter_c, &i);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size - tb_size_decrease_rate << ". Number of functions: " << initial_function_number - func_num_decrease_rate << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<int, initial_table_size - tb_size_decrease_rate, initial_function_number - func_num_decrease_rate> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(i);
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(i));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(i);
		}

		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}
	

	struct ErrorRateStrings : MemoryLeaksTests {};

	TEST_F(ErrorRateStrings, InitialTest) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::string>{}(*static_cast<std::string*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size, hash_func, initial_function_number);


		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			bloom_filter_insert(bloom_filter_c, &i_to_str);
		}

		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i_to_str), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			std::string i_to_str{ std::to_string(i) };
			source_false_positives += bloom_filter_query(bloom_filter_c, &i_to_str);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size << ". Number of functions: " << initial_function_number << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<std::string, initial_table_size, initial_function_number> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(std::to_string(i));
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(std::to_string(i)));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(std::to_string(i));
		}

		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}

	TEST_F(ErrorRateStrings, DecreaseTableSize) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::string>{}(*static_cast<std::string*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size - tb_size_decrease_rate, hash_func, initial_function_number);


		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			bloom_filter_insert(bloom_filter_c, &i_to_str);
		}

		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i_to_str), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			std::string i_to_str{ std::to_string(i) };
			source_false_positives += bloom_filter_query(bloom_filter_c, &i_to_str);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size - tb_size_decrease_rate << ". Number of functions: " << initial_function_number << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<std::string, initial_table_size - tb_size_decrease_rate, initial_function_number> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(std::to_string(i));
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(std::to_string(i)));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(std::to_string(i));
		}

		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}

	TEST_F(ErrorRateStrings, DecreaseFuncNum) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::string>{}(*static_cast<std::string*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size, hash_func, initial_function_number - func_num_decrease_rate);


		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			bloom_filter_insert(bloom_filter_c, &i_to_str);
		}

		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i_to_str), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			std::string i_to_str{ std::to_string(i) };
			source_false_positives += bloom_filter_query(bloom_filter_c, &i_to_str);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size << ". Number of functions: " << initial_function_number - func_num_decrease_rate << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<std::string, initial_table_size, initial_function_number - func_num_decrease_rate> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(std::to_string(i));
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(std::to_string(i)));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(std::to_string(i));
		}


		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}

	TEST_F(ErrorRateStrings, IncreaseFuncNum) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::string>{}(*static_cast<std::string*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size, hash_func, initial_function_number + func_num_increase_rate);


		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			bloom_filter_insert(bloom_filter_c, &i_to_str);
		}

		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i_to_str), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			std::string i_to_str{ std::to_string(i) };
			source_false_positives += bloom_filter_query(bloom_filter_c, &i_to_str);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size << ". Number of functions: " << initial_function_number + func_num_increase_rate << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<std::string, initial_table_size, initial_function_number + func_num_increase_rate> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(std::to_string(i));
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(std::to_string(i)));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(std::to_string(i));
		}


		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}

	TEST_F(ErrorRateStrings, DecreaseBoth) {

		auto hash_func = [](BloomFilterValue data) { return static_cast<unsigned>(std::hash<std::string>{}(*static_cast<std::string*>(data))); };

		auto* bloom_filter_c = bloom_filter_new(initial_table_size - tb_size_decrease_rate, hash_func, initial_function_number - func_num_decrease_rate);


		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			bloom_filter_insert(bloom_filter_c, &i_to_str);
		}

		for (auto i = 0; i < cnt; ++i) {
			std::string i_to_str{ std::to_string(i) };
			ASSERT_EQ(bloom_filter_query(bloom_filter_c, &i_to_str), 1);
		}

		size_t source_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			std::string i_to_str{ std::to_string(i) };
			source_false_positives += bloom_filter_query(bloom_filter_c, &i_to_str);
		}

		const console_logger logger{ {{"color", "true"}, {"log_level", "debug"}} };

		logger(log_level::info) << "Table size is: " << initial_table_size - tb_size_decrease_rate << ". Number of functions: " << initial_function_number - func_num_decrease_rate << std::endl;

		logger(log_level::info) << "SOURCE BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << source_false_positives << " (" << static_cast<double>(source_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;

		bloom_filter_free(bloom_filter_c);


		bloom_filter<std::string, initial_table_size - tb_size_decrease_rate, initial_function_number - func_num_decrease_rate> filter{};


		for (auto i = 0; i < cnt; ++i) {
			filter.insert(std::to_string(i));
		}

		for (auto i = 0; i < cnt; ++i) {
			ASSERT_TRUE(filter.query(std::to_string(i)));
		}

		size_t refactored_false_positives{};
		for (auto i = min_false; i < max_false; ++i) {
			refactored_false_positives += filter.query(std::to_string(i));
		}
		
		logger(log_level::debug) << "REFACTORED BLOOM -> range: [" << max_false << " - " << min_false << " = " << max_false - min_false;
		logger() << "]. Number of false_positives = " << refactored_false_positives << " (" << static_cast<double>(refactored_false_positives) / (result_set_count * 100.0) << "%)" << std::endl;
	}
	
}