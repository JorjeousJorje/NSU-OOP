#include "pch.h"
#include <random>
#include <map>
#include <xmemory>
#include "skip_list.hpp"

template<class T>
class custom_alloc {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using void_pointer = void*;
	using const_void_pointer = const void*;

	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	
public:
	pointer allocate(size_type num_of_objects)
	{
		return static_cast<pointer>(operator new(sizeof(T) * num_of_objects));
	}
	
	pointer allocate(size_type num_of_objects, const_void_pointer hint)
	{
		return allocate(num_of_objects);
	}
	
	void deallocate(pointer p, size_type num_of_objects)
	{
		operator delete(p);
	}
};

using ca_pair_int_string = custom_alloc<std::pair<const int, std::string>>;

TEST(Costructor, Default)
{
	const skip_list<int, std::string> l1;
	ASSERT_TRUE(l1.empty());
}

TEST(Costructor, CustomComparator)
{
	std::greater<> comp{};
	skip_list<int, std::string, std::greater<>> l1(comp);
	l1[1] = "1";
	l1[12] = "12";
	l1[4] = "4";
	l1[124] = "124";
	ASSERT_EQ(l1.begin()->first, 124);
}

TEST(Costructor, CustomAllocator)
{
	std::greater<> comp{};
	ca_pair_int_string alloc{};
	skip_list<int, std::string, std::greater<>, ca_pair_int_string> l1(comp, alloc);
	l1[1] = "1";
	l1[12] = "12";
	l1[4] = "4";
	l1[124] = "124";
	ASSERT_EQ(l1.begin()->first, 124);
}

TEST(CopyCostructor, CopyCostructor)
{
	skip_list<int, std::string> l1;
	l1[1] = "one";
	auto l2{ l1 };
	ASSERT_EQ(l2, l1);
}

TEST(CopyCostructor, CopyCostructorWithCustomComp)
{
	skip_list<int, std::string, std::greater<>> l1;
	l1[1] = "one";
	l1[2] = "two";
	auto l2{ l1 };
	ASSERT_EQ(l2.begin()->first, 2);
	ASSERT_EQ(l2, l1);
}

TEST(CopyCostructor, CopyCostructorWithCustomAlloc)
{
	skip_list<int, std::string, std::less<>, ca_pair_int_string> l1;
	l1[1] = "one";
	auto l2{ l1 };
	ASSERT_EQ(l2, l1);
}

TEST(MoveCostructor, MoveCostructor)
{
	skip_list<int, std::string> l1;
	l1[1] = "one";
	auto l2{ l1 };
	auto l3{ std::move(l1) };
	ASSERT_EQ(l3, l2);
}

TEST(MoveCostructor, MoveCostructorWithCustomComp)
{
	skip_list<int, std::string, std::greater<>> l1;
	l1[1] = "one";
	l1[2] = "two";
	auto l2 = l1;
	auto l3{ std::move(l1) };
	ASSERT_EQ(l3, l2);
	ASSERT_EQ(l3.begin()->first, 2);
}

TEST(MoveCostructor, MoveCostructorWithCustomAlloc)
{
	skip_list<int, std::string, std::less<>, ca_pair_int_string> l1;
	l1[1] = "one";
	l1[2] = "two";
	auto l2 = l1;
	auto l3{ std::move(l1) };
	ASSERT_EQ(l3, l2);
}

TEST(MoveCostructor, MoveSameEqual)
{
	skip_list<int, std::string> l1;
	l1[1] = "1";
	l1[1245] = "1245";
	l1[2] = "2";
	l1 = std::move(l1);
	EXPECT_FALSE(l1.empty());
	EXPECT_NO_THROW(EXPECT_EQ(l1.at(1), "1"));
	EXPECT_NO_THROW(EXPECT_EQ(l1.at(1245), "1245"));
	EXPECT_NO_THROW(EXPECT_EQ(l1.at(2), "2"));
}

TEST(Methods, Insert)
{
	skip_list<int, std::string> list;
	auto [it1, bool1] = list.insert({ 1, "1" });
	auto [it2, bool2] = list.insert({ 2, "2" });
	auto [it3, bool3] = list.insert({ 3, "3" });
	EXPECT_EQ(list.find(1)->second, "1");
	list.erase(1);
	EXPECT_EQ(list.find(1), list.end());
	EXPECT_EQ(list.find(2)->second, "2");
}

template<class T, class dummy = std::enable_if<std::is_integral_v<T>>>
struct gen_int
{
	gen_int(const T min, const T max) : dist{ min, max } {}

	const std::uniform_int_distribution<T> dist;
	std::mt19937 engine{ 0 };

	auto operator()() {
		return dist(engine);
	}
};

TEST(Methods, InsertRandomly)
{
	skip_list<int, int> list;
	std::map<int, int> map;
	gen_int generator{ 1, 547 };
	for (int i = 0; i < 100; ++i) {
		auto r = generator();
		list[r] = r;
		map[r] = r;
	}
	EXPECT_EQ(list.size(), map.size());
	EXPECT_TRUE(std::is_sorted(list.begin(), list.end(), [](const auto& l, const auto& r) { return l.first < r.first; }));
	EXPECT_TRUE(std::equal(list.begin(), list.end(), map.begin(), map.end()));
}

TEST(Methods, InsertForward)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	list[4] = "4";
	list[5] = "5";
	list[6] = "6";
	std::map<int, std::string> map;
	map[1] = "1";
	map[2] = "2";
	map[3] = "3";
	map[4] = "4";
	map[5] = "5";
	map[6] = "6";
	EXPECT_TRUE(std::equal(list.begin(), list.end(), map.begin(), map.end()));
	EXPECT_TRUE(std::is_sorted(list.begin(), list.end(), [](const auto& l, const auto& r) { return l.first < r.first; }));
}

TEST(Methods, InsertBackward)
{
	skip_list<int, std::string> list;
	list[6] = "1";
	list[5] = "2";
	list[4] = "3";
	list[3] = "4";
	list[3] = "5";
	list[2] = "6";
	list[1] = "6";
	std::map<int, std::string> map;
	map[6] = "1";
	map[5] = "2";
	map[4] = "3";
	map[3] = "4";
	map[3] = "5";
	map[2] = "6";
	map[1] = "6";
	EXPECT_TRUE(std::equal(list.begin(), list.end(), map.begin(), map.end()));
	EXPECT_TRUE(std::is_sorted(list.begin(), list.end(), [](const auto& l, const auto& r) { return l.first < r.first; }));
}
TEST(Methods, InsertExsistingKey)
{
	skip_list<std::string, int> list;
	list["two"] = 2;
	auto it = list.insert(std::make_pair("two", 1));
	ASSERT_EQ(it.first, list.begin());
	ASSERT_EQ(it.second, false);
	ASSERT_EQ(list.size(), 1);
	ASSERT_EQ(list.begin()->second, 2);
	it = list.insert(std::make_pair("two", 33));
	ASSERT_EQ(it.first, list.begin());
	ASSERT_EQ(it.second, false);
	ASSERT_EQ(list.size(), 1);
	ASSERT_EQ(list.begin()->second, 2);
}
TEST(Methods, InsertCustomAlloc)
{
	skip_list<int, std::string, std::less<>, ca_pair_int_string> list;
	auto [it1, bool1] = list.insert({ 1, "1" });
	auto [it2, bool2] = list.insert({ 2, "2" });
	auto [it3, bool3] = list.insert({ 3, "3" });
	EXPECT_EQ(list.find(1)->second, "1");
	list.erase(1);
	EXPECT_EQ(list.find(1), list.end());
	EXPECT_EQ(list.find(2)->second, "2");
}

TEST(Methods, Find)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	EXPECT_EQ(list.find(1)->second, "1");
	list.erase(1);
	EXPECT_EQ(list.find(1), list.end());
	EXPECT_EQ(list.find(2)->second, "2");
}

TEST(Methods, Begin)
{
	skip_list<int, std::string> list;
	list[1] = "one";
	ASSERT_EQ(list.begin()->second, "one");
	list[2] = "two";
	ASSERT_EQ(list.begin()->second, "one");
}

TEST(Methods, At)
{
	skip_list<int, std::string> list;
	try {
		list.at(1);
	}
	catch (std::out_of_range&) {
		ASSERT_TRUE(list.empty());
		ASSERT_EQ(list.size(), 0);
	}
	list[1] = "one";
	ASSERT_EQ(list.at(1), "one");
	list[2] = "two";
	ASSERT_EQ(list.at(1), "one");
}

TEST(Methods, Empty)
{
	skip_list<int, std::string> list;
	EXPECT_TRUE(list.empty());
	list[1] = "1";
	EXPECT_TRUE(!list.empty());
	list.erase(1);
	EXPECT_TRUE(list.empty());	
}

TEST(Methods, Size)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	EXPECT_EQ(list.size(), 3);
	list.erase(1);
	EXPECT_EQ(list.size(), 2);
}

TEST(Methods, EraseIterator)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	EXPECT_EQ(list.at(1), "1");
	list.erase(list.begin());
	EXPECT_EQ(list.find(1), list.end());
	EXPECT_EQ(list.find(2), list.begin());
	EXPECT_EQ(list.size(), 2);
}

TEST(Methods, EraseFromIteratorToIterator)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	list[4] = "4";
	list[5] = "5";
	list[6] = "6";
	const auto last = list.find(5);
	list.erase(list.begin(), last);
	EXPECT_EQ(list.find(1), list.end());
	EXPECT_EQ(list.find(2), list.end());
	EXPECT_EQ(list.find(3), list.end());
	EXPECT_EQ(list.find(4), list.end());
	EXPECT_EQ(list.begin()->second ,"5");
	EXPECT_EQ(list.size(), 2);
}

TEST(Methods, EraseByKey)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	list[4] = "4";
	list[5] = "5";
	list[6] = "6";
	list.erase(1);
	EXPECT_EQ(list.find(1), list.end());
	EXPECT_EQ(list.find(2)->second, "2");
	EXPECT_EQ(list.find(3)->second, "3");
	EXPECT_EQ(list.find(4)->second, "4");
	EXPECT_EQ(list.begin()->second, "2");
	EXPECT_EQ(list.size(), 5);
	list.erase(10);
	EXPECT_EQ(list.size(), 5);
}

TEST(Methods, EraseAll)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	list[4] = "4";
	list[5] = "5";
	list[6] = "6";
	list.erase(list.begin(), list.end());
	EXPECT_EQ(list.size(), 0);
	EXPECT_TRUE(list.empty());
}

TEST(Methods, Swap)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	list[4] = "4";
	list[5] = "5";
	list[6] = "6";

	const auto copy1 = list;
	skip_list<int, std::string> list2;
	list2[7] = "7";
	list2[8] = "8";
	list2[9] = "9";
	list2[10] = "10";
	list2[11] = "11";
	list2[12] = "12";
	const auto copy2 = list2;

	list.swap(list2);
	
	EXPECT_EQ(list, copy2);
	EXPECT_EQ(list2, copy1);
}

TEST(Methods, Clear)
{
	skip_list<int, std::string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	list[4] = "4";
	list[5] = "5";
	list[6] = "6";
	list.clear();
	EXPECT_EQ(list.find(1), list.end());
	EXPECT_EQ(list.find(2), list.end());
	EXPECT_EQ(list.find(3), list.end());
	EXPECT_EQ(list.find(4), list.end());
	EXPECT_EQ(list.find(5), list.end());
	EXPECT_EQ(list.find(6), list.end());
	EXPECT_TRUE(list.empty());
	EXPECT_EQ(list.size(), 0);
	
}

TEST(Methods, ClearCustomAlloc)
{
	skip_list<int, std::string, std::less<>, ca_pair_int_string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	list[4] = "4";
	list[5] = "5";
	list[6] = "6";
	list.clear();
	EXPECT_EQ(list.find(1), list.end());
	EXPECT_EQ(list.find(2), list.end());
	EXPECT_EQ(list.find(3), list.end());
	EXPECT_EQ(list.find(4), list.end());
	EXPECT_EQ(list.find(5), list.end());
	EXPECT_EQ(list.find(6), list.end());
	EXPECT_TRUE(list.empty());
	EXPECT_EQ(list.size(), 0);

}

TEST(Operators, CopyAssignmentoperator)
{
	skip_list<int, std::string> l1;
	l1[1] = "one";
	auto l2 = l1;
	ASSERT_EQ(l2, l1);
}

TEST(Operators, MoveAssignmentOperator)
{
	skip_list<int, std::string> l1;
	l1[1] = "one";
	auto l2= l1;
	auto l3 = std::move(l1);
	ASSERT_EQ(l3, l2);
}

TEST(Operators, EqualOperator)
{
	skip_list<int, std::string> l1;
	l1[1] = "1";
	l1[1245] = "1245";
	l1[2] = "2";
	skip_list<int, std::string> l2;
	l2[1] = "1";
	l2[1245] = "1245";
	l2[2] = "2";
	EXPECT_TRUE(l1 == l2);

	const skip_list<int, std::string> empty1;
	const skip_list<int, std::string> empty2;
	EXPECT_TRUE(empty1 == empty2);
}
TEST(Operators, NotEqualOperator)
{
	skip_list<int, std::string> l1;
	l1[1] = "1";
	l1[1245] = "1245";
	l1[2] = "2";
	skip_list<int, std::string> l2;
	l2[1] = "1";
	l2[1245] = "1245";
	l2[2] = "3";
	EXPECT_TRUE(l1 != l2);
}

TEST(Operators, BraketsOperator)
{
	skip_list<int, std::string> l1;
	l1[1] = "1";
	l1[1245] = "1245";
	l1[2] = "2";
	EXPECT_EQ(l1.at(1), "1");
	EXPECT_EQ(l1.at(1245), "1245");
	EXPECT_EQ(l1.at(2), "2");
}

TEST(Operators, BraketsOperatorCustomAlloc)
{
	skip_list<int, std::string, std::less<>, ca_pair_int_string> l1;
	l1[1] = "1";
	l1[1245] = "1245";
	l1[2] = "2";
	EXPECT_EQ(l1.at(1), "1");
	EXPECT_EQ(l1.at(1245), "1245");
	EXPECT_EQ(l1.at(2), "2");
}

TEST(Iterator, EqualOperator)
{
	skip_list<int, std::string> l1;
	l1[1] = "1";
	l1[1245] = "1245";
	l1[2] = "2";
	auto l2 = l1;
	EXPECT_EQ(l1, l1);
	EXPECT_EQ(l1.begin(), l1.begin());
	EXPECT_EQ(++l1.begin(), ++l1.begin());
	EXPECT_NE(l1.begin(), l2.begin());
	EXPECT_NE(++l1.begin(), ++l2.begin());
}

TEST(Iterator, NotEqualOperator)
{
	skip_list<int, std::string> l1;
	l1[1] = "1";
	l1[1245] = "1245";
	l1[2] = "2";
	auto l2 = l1;
	EXPECT_NE(l1.begin(), l2.begin());
	EXPECT_NE(++l1.begin(), ++l2.begin());
}

TEST(Iterator, OperatorPostfPlusPlus)
{
	skip_list<int, std::string> l1;
	l1[1] = "1";
	l1[2] = "2";
	l1[4] = "4";
	l1[5] = "5";
	l1[1245] = "1245";
	skip_list<int, std::string> l2;
	l2[1] = "1";
	auto l1_begin = l1.begin();
	auto fake_last_l1 = (((l1_begin++)++)++)++;
	EXPECT_NE(fake_last_l1, l2.begin());
	EXPECT_EQ(*fake_last_l1, *l2.begin());
	EXPECT_EQ(l1_begin, ++l1.begin());
}

TEST(Iterator, OperatorPrefPlusPlus)
{
	skip_list<int, std::string> l1;
	l1[1] = "1";
	l1[2] = "2";
	l1[4] = "4";
	l1[5] = "5";
	l1[1245] = "1245";
	skip_list<int, std::string> l2;
	l2[1245] = "1245";
	const auto last_l1 = ++(++(++(++l1.begin())));
	const auto third_l1 = ++(++l1.begin());
	EXPECT_EQ(l1.find(4), third_l1);
	EXPECT_EQ(l1.begin(), l1.begin());
	EXPECT_EQ(++l1.begin(), ++l1.begin());
	EXPECT_NE(last_l1, l2.begin());
	EXPECT_EQ(*last_l1, *l2.begin());
}

TEST(Iterator, Std—ompatibility)
{
	skip_list<int, std::string, std::less<>, ca_pair_int_string> list;
	list[1] = "1";
	list[2] = "2";
	list[3] = "3";
	list[4] = "4";
	list[5] = "5";
	list[6] = "6";
	auto to_add = "add";
	std::for_each(list.begin(), list.end(), [to_add](std::pair<const int, std::string>& elem) {elem.second += to_add; });
	for(auto it = list.begin(); it != list.end(); ++it) {
		EXPECT_EQ(it->second, std::to_string(it->first) + to_add);
	}
}
