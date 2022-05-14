#include "pch.h"
#include <vector>
#include <array>
#include <chrono>
#include <list>
#include <random>
#include "optimized_copy.hpp"


namespace
{
	class integer
	{
	public:
		integer() : value{} {}
		integer(int i) : value(i) {}

		friend bool operator==(const integer& left, const integer& right);

		integer& operator=(const integer& right) {
			if (this == &right) {
				return *this;
			}
			value = right.value;
			return *this;
		}

		~integer() = default;

	private:
		int value;
	};

	bool operator==(const integer& left, const integer& right) {
		return left.value == right.value;
	}

}

namespace fundamental_types
{
	namespace
	{
		TEST(VectorToNewArray, IntCopy) {

			std::vector<int> c1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			auto* c2 = new int[10];
			
			optimized::copy(c1.begin(), c1.end(), c2);
			ASSERT_TRUE(std::equal(c1.begin(), c1.end(), c2));
			delete[] c2;

		}

		TEST(NewArrayToVector, IntCopy)
		{
			auto* c1 = new int[10];
			std::fill_n(c1, 10, 19);
			
			std::vector<int> c2(10);
			optimized::copy(c1, c1 + 10, c2.begin());
			ASSERT_TRUE(std::equal(c2.begin(), c2.end(), c1));
			delete[] c1;
		}

		TEST(NewArrayToVector, StdStringCopy)
		{
			auto* c1 = new std::string[10];
			
			std::fill_n(c1, 10, "abc");
			
			std::vector<std::string> c2(10);
			optimized::copy(c1, c1 + 10, c2.begin());
			ASSERT_TRUE(std::equal(c2.begin(), c2.end(), c1));
			delete[] c1;
		}
		
		TEST(VectorToNewArray, StdStringCopy)
		{
			std::vector<std::string> c1 = { "0, 1", "2, 3", "4, 5", "6, 7", "8, 9" };
			auto* c2 = new std::string[5];
			optimized::copy(c1.begin(), c1.end(), c2);
			ASSERT_TRUE(std::equal(c1.begin(), c1.end(), c2));
			delete[] c2;
		}

		TEST(NewArrayToNewArray, IntCopy)
		{
			auto* c1 = new int[10];
			std::fill_n(c1, 10, 10);
			auto* c2 = new int[10];
			optimized::copy(c1, c1 + 10, c2);
			ASSERT_TRUE(std::equal(c2, c2 + 10, c1));
			delete[] c1;
			delete[] c2;
		}

		TEST(NewArrayToNewArray, StdStringCopy)
		{
			auto* c1 = new std::string[10];
			std::fill_n(c1, 10, "abc");
			auto* c2 = new std::string[10];
			optimized::copy(c1, c1 + 10, c2);
			ASSERT_TRUE(std::equal(c1, c1 + 10, c2));
			delete[] c1;
			delete[] c2;
		}

		TEST(NewArrayTolist, IntCopy)
		{
			auto* c1 = new int[10];
			std::fill_n(c1, 10, 10);
			std::list<int> c2(10);
			optimized::copy(c1, c1 + 10, c2.begin());
			ASSERT_TRUE(std::equal(c2.begin(), c2.end(), c1));
			delete[] c1;
		}

	}
	namespace
	{
		TEST(VectorToVector_f, IntCopy) {

			std::vector<int> v1(10, 10);
			std::vector<int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToVector_f, NegativeIntCopy) {

			std::vector<int> v1(10, -10);
			std::vector<int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(VectorToVector_f, DoubleCopy) {

			std::vector<double> v1(10, 10.2);
			std::vector<double> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToVector_f, NegativeDoubleCopy) {

			std::vector<double> v1(10, -10.2);
			std::vector<double> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToVector_f, ShortCopy) {

			std::vector<short> v1(10, 1);
			std::vector<short> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(VectorToVector_f, LongIntCopy) {

			std::vector<long int> v1(10, 11324);
			std::vector<long int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToVector_f, FloatCopy) {

			std::vector<float> v1(10, 113.4243f);
			std::vector<float> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToVector_f, CharCopy) {

			std::vector<char> v1(10, 'a');
			std::vector<char> v2(10, 'b');

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToList_f, IntCopy) {

			std::vector<int> v1(10, 10);
			std::list<int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToList_f, NegativeIntCopy) {

			std::vector<int> v1(10, -10);
			std::list<int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(VectorToList_f, DoubleCopy) {

			std::vector<double> v1(10, 10.2);
			std::list<double> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToList_f, NegativeDoubleCopy) {

			std::vector<double> v1(10, -10.2);
			std::list<double> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToList_f, ShortCopy) {

			std::vector<short> v1(10, 1);
			std::list<short> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(VectorToList_f, LongIntCopy) {

			std::vector<long int> v1(10, 11324);
			std::list<long int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToList_f, FloatCopy) {

			std::vector<float> v1(10, 113.4243f);
			std::list<float> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToList_f, CharCopy) {

			std::vector<char> v1(10, 'a');
			std::list<char> v2(10, 'b');

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToArray_f, IntCopy) {

			std::vector<int> v1(10, 10);
			std::array<int, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToArray_f, NegativeIntCopy) {

			std::vector<int> v1(10, -10);
			std::array<int, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(VectorToArray_f, DoubleCopy) {

			std::vector<double> v1(10, 10.2);
			std::array<double, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToArray_f, NegativeDoubleCopy) {

			std::vector<double> v1(10, -10.2);
			std::array<double, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToArray_f, ShortCopy) {

			std::vector<short> v1(10, 1);
			std::array<short, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(VectorToArray_f, LongIntCopy) {

			std::vector<long int> v1(10, 11324);
			std::array<long int, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToArray_f, FloatCopy) {

			std::vector<float> v1(10, 113.4243f);
			std::array<float, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(VectorToArray_f, CharCopy) {

			std::vector<char> v1(10, 'a');
			std::array<char, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}
		TEST(VectorToStaticArray_f, CharCopy) {

			std::vector<char> v1(10, 'a');
			char v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}

		TEST(VectorToStaticArray_f, ShortCopy) {

			std::vector<short> v1(10, 1);
			short v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}


		TEST(VectorToStaticArray_f, LongIntCopy) {

			std::vector<long int> v1(10, 11324);
			long int v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}

		TEST(VectorToStaticArray_f, FloatCopy) {

			std::vector<float> v1(10, 113.4243f);
			float v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}

		TEST(VectorToStaticArray_f, IntCopy) {

			std::vector<int> v1(10, 10);
			int v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}

		TEST(VectorToStaticArray_f, NegativeIntCopy) {

			std::vector<int> v1(10, -10);
			int v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}


		TEST(VectorToStaticArray_f, DoubleCopy) {

			std::vector<double> v1(10, 10.2);
			double v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}

		TEST(VectorToStaticArray_f, NegativeDoubleCopy) {

			std::vector<double> v1(10, -10.2);
			double v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}
	}

	namespace
	{
		TEST(StaticArrayToVector_f, IntCopy) {

			int v1[10];
			std::fill_n(&v1[0], std::size(v1), 1);
			std::vector<int> v2(10, 0);

			optimized::copy(&v1[0], &v1[0] + std::size(v1), v2.begin());
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), v2.begin()));
		}

		TEST(StaticArrayToVector_f, NegativeIntCopy) {

			int v1[10];
			std::fill_n(&v1[0], std::size(v1), -1);
			std::vector<int> v2(10, 0);

			optimized::copy(&v1[0], &v1[0] + std::size(v1), v2.begin());
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), v2.begin()));
		}


		TEST(StaticArrayToVector_f, DoubleCopy) {

			double v1[10];
			std::fill_n(&v1[0], std::size(v1), 1.1);
			std::vector<double> v2(10, 0);

			optimized::copy(&v1[0], &v1[0] + std::size(v1), v2.begin());
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), v2.begin()));
		}

		TEST(StaticArrayToVector_f, NegativeDoubleCopy) {

			double v1[10];
			std::fill_n(&v1[0], std::size(v1), -10.1);
			std::vector<double> v2(10, 0);

			optimized::copy(&v1[0], &v1[0] + std::size(v1), v2.begin());
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), v2.begin()));
		}

		TEST(StaticArrayToVector_f, ShortCopy) {

			short v1[10];
			std::fill_n(&v1[0], std::size(v1), 13);
			std::vector<short> v2(10, 0);

			optimized::copy(&v1[0], &v1[0] + std::size(v1), v2.begin());
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), v2.begin()));
		}
	}

	namespace
	{
		TEST(ListToVector_f, IntCopy) {

			std::list<int> v1(10, 1000);
			std::vector<int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(ListToVector_f, NegativeIntCopy) {

			std::list<int> v1(10, -1000);
			std::vector<int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(ListToVector_f, DoubleCopy) {

			std::list<double> v1(10, 1000.123);
			std::vector<double> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(ListToVector_f, NegativeDoubleCopy) {

			std::list<double> v1(10, -1000.23);
			std::vector<double> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(ListToVector_f, ShortCopy) {

			std::list<short> v1(10, 1000);
			std::vector<short> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}
	}

	namespace
	{
		TEST(ListToList_f, IntCopy) {

			std::list<int> v1(10, 1000);
			std::list<int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(ListToList_f, NegativeIntCopy) {

			std::list<int> v1(10, -1000);
			std::list<int> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(ListToList_f, DoubleCopy) {

			std::list<double> v1(10, 1000.123);
			std::list<double> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(ListToList_f, NegativeDoubleCopy) {

			std::list<double> v1(10, -1000.23);
			std::list<double> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(ListToList_f, ShortCopy) {

			std::list<short> v1(10, 1000);
			std::list<short> v2(10, 0);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}
	}

	namespace
	{
		TEST(StaticArrayToStaticArray_f, IntCopy) {

			int v1[10];
			std::fill_n(&v1[0], std::size(v1), 1);
			int v2[10];

			optimized::copy(&v1[0], &v1[0] + std::size(v1), &v2[0]);
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), &v2[0]));
		}

		TEST(StaticArrayToStaticArray_f, NegativeIntCopy) {

			int v1[10];
			std::fill_n(&v1[0], std::size(v1), -1);
			int v2[10];

			optimized::copy(&v1[0], &v1[0] + std::size(v1), &v2[0]);
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), &v2[0]));
		}


		TEST(StaticArrayToStaticArray_f, DoubleCopy) {

			double v1[10];
			std::fill_n(&v1[0], std::size(v1), 1.1);
			double v2[10];

			optimized::copy(&v1[0], &v1[0] + std::size(v1), &v2[0]);
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), &v2[0]));
		}

		TEST(StaticArrayToStaticArray_f, NegativeDoubleCopy) {

			double v1[10];
			std::fill_n(&v1[0], std::size(v1), -10.1);
			double v2[10];

			optimized::copy(&v1[0], &v1[0] + std::size(v1), &v2[0]);
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), &v2[0]));
		}

		TEST(StaticArrayToStaticArray_f, ShortCopy) {

			short v1[10];
			std::fill_n(&v1[0], std::size(v1), 13);
			short v2[10];

			optimized::copy(&v1[0], &v1[0] + std::size(v1), &v2[0]);
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), &v2[0]));
		}
	}
}

namespace non_fundamental_types
{
	namespace
	{
		TEST(StdStringCopy, VectorToVector_nf) {

			std::vector<std::string> v1(10, "0");
			std::vector<std::string> v2(10);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(StdStringCopy, VectorToList_nf) {

			std::vector<std::string> v1(10, "0");
			std::list<std::string> v2(10);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(StdStringCopy, VectorToStaticArray_nf) {

			std::vector<std::string> v1(10, "0");
			std::string v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}

		TEST(StdStringCopy, VectorToStdArray_nf) {

			std::vector<std::string> v1(10, "0");
			std::array<std::string, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(StdStringCopy, ListToVector_nf) {

			std::list<std::string> v1(10, "0");
			std::vector<std::string> v2(10);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(StdStringCopy, StaticArrayToVector_nf) {

			std::string v1[10];
			std::fill_n(&v1[0], std::size(v1), "0");
			std::vector<std::string> v2(10);

			optimized::copy(&v1[0], &v1[0] + std::size(v1), v2.begin());
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), v2.begin()));
		}
	}

	namespace
	{
		TEST(TestObject, VectorToVector_nf) {

			std::vector<integer> v1(10, integer{ 10 });
			std::vector<integer> v2(10);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(TestObject, VectorToList_nf) {

			std::vector<integer> v1(10, integer{ 10 });
			std::list<integer> v2(10);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(TestObject, VectorToStaticArray_nf) {

			std::vector<integer> v1(10, integer{ 10 });
			integer v2[10];

			optimized::copy(v1.begin(), v1.end(), &v2[0]);
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), &v2[0]));
		}

		TEST(TestObject, VectorToStdArray_nf) {

			std::vector<integer> v1(10, integer{ 10 });
			std::array<integer, 10> v2{};

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}

		TEST(TestObject, ListToVector_nf) {

			std::list<integer> v1(10, integer{ 10 });
			std::vector<integer> v2(10);

			optimized::copy(v1.begin(), v1.end(), v2.begin());
			EXPECT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
		}


		TEST(TestObject, StaticArrayToVector_nf) {

			integer v1[10];
			std::fill_n(&v1[0], std::size(v1), integer{ 1 });
			std::vector<integer> v2(10);

			optimized::copy(&v1[0], &v1[0] + std::size(v1), v2.begin());
			EXPECT_TRUE(std::equal(&v1[0], &v1[0] + std::size(v1), v2.begin()));
		}
	}
}