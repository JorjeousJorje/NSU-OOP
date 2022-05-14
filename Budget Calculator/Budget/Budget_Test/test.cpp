#include "pch.h"
#include <sstream>
#include <Windows.h>
#include "budget.hpp"
#include "date.hpp"

TEST(date_test, create_date)
{
	EXPECT_NO_THROW(date(1, 11, 2014));
	EXPECT_THROW(date(-1, 11, 2014), std::invalid_argument);
	EXPECT_THROW(date(-1, -11, 2014), std::invalid_argument);
	EXPECT_THROW(date(-1, -11, -2014), std::invalid_argument);
	EXPECT_THROW(date(1, -11, -2014), std::invalid_argument);
	EXPECT_THROW(date(1, -11, 2014), std::invalid_argument);
	EXPECT_THROW(date(1, 11, -2014), std::invalid_argument);
}
TEST(date_test, cmp_date_day)
{
	date start_date(25, 11, 2014);
	date end_date(29, 11, 2014);
	date cur_date(24, 11, 2014);

	ASSERT_TRUE(!date::date_cmp(cur_date, start_date, end_date));

	date cur_date1(25, 11, 2014);
	ASSERT_TRUE(date::date_cmp(cur_date1, start_date, end_date));
	date cur_date2(29, 11, 2014);
	ASSERT_TRUE(date::date_cmp(cur_date2, start_date, end_date));
	date cur_date3(30, 11, 2014);
	ASSERT_TRUE(!date::date_cmp(cur_date3, start_date, end_date));
}
TEST(date_test, cmp_date_month)
{
	date start_date(25, 11, 2014);
	date end_date(29, 11, 2014);
	date cur_date(24, 12, 2014);

	ASSERT_TRUE(!date::date_cmp(cur_date, start_date, end_date));
}
TEST(date_test, cmp_date_year)
{
	date start_date(25, 11, 2014);
	date end_date(29, 11, 2014);
	date cur_date(24, 11, 2015);

	ASSERT_TRUE(!date::date_cmp(cur_date, start_date, end_date));
}
TEST(tree_test, insert_element) {
	budget_tree tree{};
	tree.insert_root("1", date{ 1,1,1 });
	auto trie = tree.get_trie();
	ASSERT_TRUE(trie[0].next.find("1")->first == "1");
	ASSERT_TRUE(tree.next(0, "1") == 1);
	tree.insert_node("1", "4", date{ 1,1,1 }, 1000);
	trie = tree.get_trie();
	ASSERT_TRUE(tree.next(1, "4") == 2);
	tree.insert_root("4", date{ 1,1,1 });
	ASSERT_TRUE(tree.next(0, "4") == 3);
}
TEST(budget_test, create_budget)
{
	bool check = false;
	std::stringstream budget_("Car 5000\n");
	std::stringstream expenses("2.12.2015 Car:Wheels 4000\n");
	std::stringstream date("2.12.2015\n4.12.2015\n");
	EXPECT_NO_THROW(const budget b(budget_, expenses));
}
TEST(budget_test, wrong_BUDGET_EXPESES_sequence)
{
	std::stringstream budget_("Car 5000\n");
	std::stringstream expenses("2.12.2015 Car:Wheels 4000\n");
	std::stringstream date("2.12.2015\n4.12.2015\n");
	EXPECT_THROW(const budget b(expenses, budget_), std::invalid_argument);
}
TEST(budget_test, budget_rus_small)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::stringstream budget_(
		"Авто               6000\n"
		"Продукты + Общепит 10000\n"
		"Алкоголь:Пиво      900");

	std::stringstream expenses(
		"24.11.2014 Авто:Бензин  1400\n"
		"23.11.2014 Продукты:Мясо 500\n"
		"23.11.2014 Продукты:Хлеб 90.33\n"
		"14.11.2014 Общепит:Столовка 190");

	std::stringstream date("14.11.2014\n24.11.2014");

	std::stringstream out;

	const budget b(budget_, expenses);

	b.count_expenses(out, date);

	auto expect =
		"Авто                             6000.00     1400.00       23.33%\n"
		"Продукты + Общепит              10000.00      780.33        7.80%\n"
		"Алкоголь:Пиво                     900.00        0.00        0.00%\n"
		"<Other expenses>                    0.00        0.00\n";
	auto res = out.str();

	EXPECT_EQ(res, expect);
}
TEST(budget_test, budget_rus_huge)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::stringstream budget_(
		"Авто                6000\n"
		"Книги               2000\n"
		"Цветы               1000\n"
		"Продукты + Общепит  10000\n"
		"Алкоголь:Пиво       900\n"
		"Компьютер           60000\n"
		"Полотенце:Вафельное 300");

	std::stringstream expenses(
		"24.11.2014 Авто:Бензин  1700\n"
		"24.11.2014 Авто:Зеркала 5400\n"
		"23.11.2014 Продукты:Мясо 54.23\n"
		"23.11.2014 Продукты:Хлеб 77.33\n"
		"14.11.2014 Общепит:Столовка 1956\n"
		"14.11.2014 Общепит:Столовка 334\n"
		"14.11.2014 Общепит:Столовка 123.90\n"
		"20.11.2014 Книги:УрокиШитья 500\n"
		"17.11.2014 Цветы:Георгины 70.67\n"
		"17.11.2014 Цветы:Розы 300.6\n"
		"17.11.2014 Цветы:Фиалки 85.50\n"
		"12.11.2014 Общепит:Столовка 1400\n"
		"15.11.2014 Компьютер:Процессор 15000\n"
		"15.11.2014 Компьютер:Видеокарта 25000\n"
		"15.11.2014 Компьютер:Оперативка 10000\n"
		"16.11.2014 Алкоголь:Пиво 700\n"
		"15.11.2014 Компьютер:Материнка 13000\n"
		"26.11.2014 Общепит:Столовка 5000\n"
		"26.11.2014 Общепит:Столовка 10000\n"
		"21.11.2014 Растения:Кактус 10000\n"
		"21.11.2014 Растения:МиниПальма 10000\n"
		"13.11.2014 Одежда:Ботинки 2345\n"
		"18.11.2014 Одежда:Ботинки 7000\n"
		"21.11.2014 Звук:Колонки 1432");

	std::stringstream date("11.11.2014\n25.11.2014");

	std::stringstream out;

	const budget b(budget_, expenses);

	b.count_expenses(out, date);

	auto expect =
		"Авто                             6000.00     7100.00      118.33%\n"
		"Книги                            2000.00      500.00       25.00%\n"
		"Цветы                            1000.00      456.77       45.68%\n"
		"Продукты + Общепит              10000.00     3945.46       39.45%\n"
		"Алкоголь:Пиво                     900.00      700.00       77.78%\n"
		"Компьютер                       60000.00    63000.00      105.00%\n"
		"Полотенце:Вафельное               300.00        0.00        0.00%\n"
		"<Other expenses>                    0.00    30777.00\n";

	auto res = out.str();

	EXPECT_EQ(res, expect);
}
TEST(budget_test, budget_rus_huge_wrong_date)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::stringstream budget_(
		"Авто                6000\n"
		"Книги               2000\n"
		"Цветы               1000\n"
		"Продукты + Общепит  10000\n"
		"Алкоголь:Пиво       900\n"
		"Компьютер           60000\n"
		"Полотенце:Вафельное 300");

	std::stringstream expenses(
		"24.11.2014 Авто:Бензин  1700\n"
		"24.11.2014 Авто:Зеркала 5400\n"
		"23.11.2014 Продукты:Мясо 54.23\n"
		"23.11.2014 Продукты:Хлеб 77.33\n"
		"14.11.2014 Общепит:Столовка 1956\n"
		"14.11.2014 Общепит:Столовка 334\n"
		"14.11.2014 Общепит:Столовка 123.90\n"
		"20.11.2014 Книги:УрокиШитья 500\n"
		"17.11.2014 Цветы:Георгины 70.67\n"
		"17.11.2014 Цветы:Розы 300.6\n"
		"17.11.2014 Цветы:Фиалки 85.50\n"
		"12.11.2014 Общепит:Столовка 1400\n"
		"15.11.2014 Компьютер:Процессор 15000\n"
		"15.11.2014 Компьютер:Видеокарта 25000\n"
		"15.11.2014 Компьютер:Оперативка 10000\n"
		"16.11.2014 Алкоголь:Пиво 700\n"
		"15.11.2014 Компьютер:Материнка 13000\n"
		"26.11.2014 Общепит:Столовка 5000\n"
		"26.11.2014 Общепит:Столовка 10000\n"
		"21.11.2014 Растения:Кактус 10000\n"
		"21.11.2014 Растения:МиниПальма 10000\n"
		"13.11.2014 Одежда:Ботинки 2345\n"
		"18.11.2014 Одежда:Ботинки 7000\n"
		"21.11.2014 Звук:Колонки 1432");

	std::stringstream date("30.11.2014\n30.11.2014");

	std::stringstream out;

	const budget b(budget_, expenses);

	b.count_expenses(out, date);

	auto expect =
		"Авто                             6000.00        0.00        0.00%\n"
		"Книги                            2000.00        0.00        0.00%\n"
		"Цветы                            1000.00        0.00        0.00%\n"
		"Продукты + Общепит              10000.00        0.00        0.00%\n"
		"Алкоголь:Пиво                     900.00        0.00        0.00%\n"
		"Компьютер                       60000.00        0.00        0.00%\n"
		"Полотенце:Вафельное               300.00        0.00        0.00%\n"
		"<Other expenses>                    0.00        0.00\n";

	auto res = out.str();

	EXPECT_EQ(res, expect);
}
TEST(budget_test, budget_rus_small_expenses)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::stringstream budget_(
		"Авто                6000\n"
		"Книги               2000\n"
		"Цветы               1000\n"
		"Продукты + Общепит  10000\n"
		"Алкоголь:Пиво       900\n"
		"Компьютер           60000\n"
		"Полотенце:Вафельное 300");

	std::stringstream expenses(
		"24.11.2014 Авто:Бензин  1700");

	std::stringstream date("24.11.2014\n25.11.2014");

	std::stringstream out;

	const budget b(budget_, expenses);

	b.count_expenses(out, date);

	auto expect =
		"Авто                             6000.00     1700.00       28.33%\n"
		"Книги                            2000.00        0.00        0.00%\n"
		"Цветы                            1000.00        0.00        0.00%\n"
		"Продукты + Общепит              10000.00        0.00        0.00%\n"
		"Алкоголь:Пиво                     900.00        0.00        0.00%\n"
		"Компьютер                       60000.00        0.00        0.00%\n"
		"Полотенце:Вафельное               300.00        0.00        0.00%\n"
		"<Other expenses>                    0.00        0.00\n";

	auto res = out.str();

	EXPECT_EQ(res, expect);
}
TEST(budget_test, budget_rus_out_of_date)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::stringstream budget_(
		"Авто                6000");

	std::stringstream expenses(
		"24.11.2014 Авто:Бензин  1700");

	std::stringstream date("26.11.2014\n26.11.2014");

	std::stringstream out;

	const budget b(budget_, expenses);

	b.count_expenses(out, date);

	auto expect =
		"Авто                             6000.00        0.00        0.00%\n"
		"<Other expenses>                    0.00        0.00\n";

	auto res = out.str();

	EXPECT_EQ(res, expect);
}
TEST(budget_test, budget_rus_a_lot_of_food)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::stringstream budget_(
		"Продукты + Общепит                6000");

	std::stringstream expenses(
		"24.11.2014 Продукты:Молоко  45\n"
		"24.11.2014 Продукты:Молоко  45\n"
		"24.11.2014 Продукты:Молоко  45\n"
		"24.11.2014 Продукты:Молоко  45\n"
		"24.11.2014 Продукты:Молоко  45\n"
		"24.11.2014 Продукты:Хлеб  25\n"
		"24.11.2014 Продукты:Хлеб  25\n"
		"24.11.2014 Продукты:Хлеб  25\n"
		"24.11.2014 Продукты:Хлеб  25\n"
		"24.11.2014 Продукты:Каша  100\n"
		"21.11.2014 Общепит:Столовка  150\n"
		"21.11.2014 Общепит:Столовка  100\n"
		"21.11.2014 Общепит:Столовка  460");
	

	std::stringstream date("21.11.2014\n30.11.2014");

	std::stringstream out;

	const budget b(budget_, expenses);

	b.count_expenses(out, date);

	auto expect =
		"Продукты + Общепит               6000.00     1135.00       18.92%\n"
		"<Other expenses>                    0.00        0.00\n";

	auto res = out.str();

	EXPECT_EQ(res, expect);
}