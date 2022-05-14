#include "pch.h"
#include <chrono>
#include <deque>
#include <array>
#include "selection_sort.hpp"
#include <string>
#include <random>
#include <unordered_set>

namespace overloaded_operator
{
	class test_object
	{
	public:
		test_object() : object_id() {}
		explicit test_object(size_t obj_id) : object_id(obj_id) {}
		size_t get_id() const { return object_id; }
		friend bool operator> (const test_object& o1, const test_object& o2) { return  o1.get_id() > o2.get_id(); }
		friend bool operator< (const test_object& o1, const test_object& o2) { return  o1.get_id() < o2.get_id(); }

	private:
		size_t object_id;
	};
}

namespace with_comparator
{
	
	class test_object
	{
	public:
		test_object() : object_id() {}
		explicit test_object(size_t obj_id) : object_id(obj_id) {}
		size_t get_id() const { return object_id; }

	private:
		size_t object_id;
	};

	bool comparator(const test_object& o1, const test_object& o2)
	{
		return o1.get_id() < o2.get_id();
	}

}

namespace
{
	template <class T>
	using is_int = std::enable_if<std::is_integral_v<T>>;

	template <class T>
	using is_float = std::enable_if<std::is_floating_point_v<T>>;

	template<class T, class dummy = is_float<T>>
	struct gen_float
	{
		gen_float(const T min, const T max) :dist{ min, max } {}

		const std::uniform_real_distribution<T> dist;
		std::mt19937 engine{ 0 };

		auto operator()() {
			return dist(engine);
		}
	};

	template<class T, class dummy = is_int<T>>
	struct gen_int
	{
		gen_int(const T min, const T max) : dist{ min, max } {}

		const std::uniform_int_distribution<T> dist;
		std::mt19937 engine{ 0 };

		auto operator()() {
			return dist(engine);
		}
	};
}

TEST(Vector, IntegerSort)
{
	std::vector<int> data{ 100 };

	std::generate_n(data.begin(), std::size(data), gen_int{ 1, 45 });

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}
TEST(Vector, StringLambdaLength)
{
	const std::string s1{ "Hello" };
	const std::string s2{ "Welcome" };
	const std::string s3{ "Yes" };
	const std::string s4{ "Gone" };

	std::vector <std::string> stings = { s1, s2, s3, s4 };

	selection_sort::sort(stings.begin(), stings.end(), [](const std::string& str1, const std::string& str2)
		{ return str1.length() < str2.length(); });
	ASSERT_EQ(stings[0], s3);
	ASSERT_EQ(stings[1], s4);
	ASSERT_EQ(stings[2], s1);
	ASSERT_EQ(stings[3], s2);
}
TEST(Vector, TestObjectOperator)
{
	std::vector <overloaded_operator::test_object> objects{ 10 };
	gen_int<size_t> gen{ 1, 45 };

	std::generate_n(objects.begin(), std::size(objects),
		[&gen]() {return overloaded_operator::test_object{ gen() }; });

	selection_sort::sort(objects.begin(), objects.end());

	EXPECT_TRUE(std::is_sorted(objects.begin(), objects.end()));
}
TEST(Vector, TestObjectComparator)
{

	std::vector <with_comparator::test_object> objects{ 10 };
	gen_int<size_t> gen{ 1, 45 };

	std::generate_n(objects.begin(), std::size(objects),
		[&gen]() {return with_comparator::test_object{ gen() }; });

	selection_sort::sort(objects.begin(), objects.end(), with_comparator::comparator);

	EXPECT_TRUE(std::is_sorted(objects.begin(), objects.end(), with_comparator::comparator));
}
TEST(Vector, DoubleSort)
{
	std::vector<double> data{ 10 };

	std::generate_n(data.begin(), std::size(data), gen_float{ 1.89, 45.435 });

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(Vector, FloatSort)
{
	std::vector<float> data{ 10 };

	std::generate_n(data.begin(), std::size(data), gen_float{ 1.89f, 45.435f });

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}
TEST(Vector, ManyIntegerSort)
{
	std::vector<int> numbers{ 10000 };

	std::generate(numbers.begin(), numbers.end(), gen_int{ 1, 45 });

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}

TEST(Vector, ManyLongUnsignetIntSort)
{
	std::vector<long unsigned> numbers{ 1000 };

	std::generate(numbers.begin(), numbers.end(), gen_int{ 1lu, 45lu });

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}

TEST(Vector, ManyLongLongIntSort)
{
	std::vector<long long> numbers{ 1000 };

	std::generate(numbers.begin(), numbers.end(), gen_int{ 1ll, 45ll });

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}
TEST(Vector, ManyDoubleSort)
{
	std::vector<double> numbers{ 10000 };

	std::generate(numbers.begin(), numbers.end(), gen_float{ 1.89, 45.435 });

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}
TEST(StdArray, IntegerSort)
{
	std::array<int, 100> data{};

	std::generate_n(data.begin(), std::size(data), gen_int{ 1, 45 });

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}
TEST(StaticArray, IntegerSort)
{
	int data[100];

	std::generate_n(data, std::size(data), gen_int{ 1, 45 });

	selection_sort::sort(data, data + std::size(data));

	EXPECT_TRUE(std::is_sorted(data, data + std::size(data)));
}
TEST(StaticArray, DoubleSort)
{
	double data[100];

	std::generate_n(data, std::size(data), gen_float(1.2, 45.5));

	selection_sort::sort(data, data + std::size(data));

	EXPECT_TRUE(std::is_sorted(data, data + std::size(data)));
}

TEST(StaticArray, ShortSort)
{
	short data[100];

	std::generate_n(data, std::size(data), gen_int<short>{ 1, 45 });

	selection_sort::sort(data, data + std::size(data));

	EXPECT_TRUE(std::is_sorted(data, data + std::size(data)));
}

TEST(StaticArray, LongInt)
{
	long data[100];

	std::generate_n(data, std::size(data), gen_int{ 1l, 45l });

	selection_sort::sort(data, data + std::size(data));

	EXPECT_TRUE(std::is_sorted(data, data + std::size(data)));
}


TEST(StaticArray, NegativeLongInts)
{
	long data[] = { -1, -4, 5, 43, -4, 345, -345 };

	selection_sort::sort(data, data + std::size(data));

	EXPECT_TRUE(std::is_sorted(data, data + std::size(data)));
}

TEST(StaticArray, CStringLambdaLength)
{
	const char* data[] = { "21", "322325", "32124", "235433546" };

	selection_sort::sort(data, data + std::size(data), [](const char* str1, const char* str2)
		{ return std::strlen(str1) < std::strlen(str2); });
	EXPECT_TRUE(std::is_sorted(data, data + std::size(data), [](const char* str1, const char* str2)
		{ return std::strlen(str1) < std::strlen(str2); }));
}

TEST(StaticArray, StdStringLambdaLength)
{
	std::string data[] = { "21", "322325", "32124", "235433546" };

	selection_sort::sort(data, data + std::size(data), [](const std::string& str1, const std::string& str2)
		{ return str1.length() < str2.length(); });
	EXPECT_TRUE(std::is_sorted(data, data + std::size(data), [](const std::string& str1, const std::string& str2)
		{ return str1.length() < str2.length(); }));
}
TEST(StdArray, DoubleSort)
{
	std::array<double, 100> data{};

	std::generate_n(data.begin(), std::size(data), gen_float{ 1.67, 45.453 });

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}
TEST(Deque, ManyDoubleSort)
{
	std::deque<double> numbers(10000);

	std::generate(numbers.begin(), numbers.end(), gen_float{ 1.67, 45.453 });

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}
TEST(Deque, ShortSort)
{
	std::deque<short> data(100);

	std::generate_n(data.begin(), std::size(data), gen_int<short>{ 1, 45 });

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(Deque, NegativeInts)
{
	std::deque<int> data = { -1, 3, -4, 5, -56765 };

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(Deque, CStringLambdaLength)
{
	std::deque<const char*> data = { "21", "322325", "32124", "235433546" };

	selection_sort::sort(data.begin(), data.end(), [](const char* str1, const char* str2)
		{ return std::strlen(str1) < std::strlen(str2); });
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end(), [](const char* str1, const char* str2)
		{ return std::strlen(str1) < std::strlen(str2); }));
}

TEST(Deque, StdStringLambdaLength)
{
	std::deque<std::string> data = { "21", "322325", "32124", "235433546" };

	selection_sort::sort(data.begin(), data.end(), [](const std::string& str1, const std::string& str2)
		{ return str1.length() < str2.length(); });
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end(), [](const std::string& str1, const std::string& str2)
		{ return str1.length() < str2.length(); }));
}
TEST(Deque, LongInt)
{
	std::deque<long> data(100);

	std::generate_n(data.begin(), std::size(data), gen_int{ 1l, 45l });

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}


TEST(StdArray, ShortSort)
{
	std::array<short, 100> data{};

	std::generate_n(data.begin(), std::size(data), gen_int<short>{ 14, 425 });

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(StdArray, LongInt)
{
	std::array<long, 100> data{};

	std::generate_n(data.begin(), std::size(data), gen_int{ 1l, 45l });
	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}


TEST(StdArray, NegativeInts)
{
	std::array<long int, 5> data = { 1, -3, -4, -432, 4 };

	selection_sort::sort(data.begin(), data.end());

	EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(StdArray, CStringLambdaLength)
{
	std::array<const char*, 4> data = { "21", "322325", "32124", "235433546" };

	selection_sort::sort(data.begin(), data.end(), [](const char* str1, const char* str2)
		{ return std::strlen(str1) < std::strlen(str2); });
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end(), [](const char* str1, const char* str2)
		{ return std::strlen(str1) < std::strlen(str2); }));
}

TEST(StdArray, StdStringLambdaLength)
{
	std::array<std::string, 4> data = { "21", "322325", "32124", "235433546" };

	selection_sort::sort(data.begin(), data.end(), [](const std::string& str1, const std::string& str2)
		{ return str1.length() < str2.length(); });
	EXPECT_TRUE(std::is_sorted(data.begin(), data.end(), [](const std::string& str1, const std::string& str2)
		{ return str1.length() < str2.length(); }));
}



TEST(Vector, NegativeIntegerSort)
{
	std::vector<int> numbers = { 1, -5, 3, -123, -34, -334, -234 };

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}

TEST(Vector, NegativeDoubleSort)
{
	std::vector<double> numbers = { 1.3, -5.1, 3.2, -123, -34.4, -334, -234.3 };

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}



TEST(Vector, LongLongSort)
{
	std::vector<long long> numbers = { 1, -5, 3, 123, -34, -334, -234 };

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}

TEST(Vector, Short)
{
	std::vector<short> numbers = { 1, -5, 3, 123, -34, -334, -234 };

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}

TEST(Vector, Char)
{
	std::vector<char> numbers = { 1, -5, 3, 123, -34, 4, 1 };

	selection_sort::sort(numbers.begin(), numbers.end());

	EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}