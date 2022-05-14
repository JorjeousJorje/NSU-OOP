#include "pch.h"
#include "interval.hpp"
#include "math.h"


namespace integer_types
{
	namespace method_in_test
	{
		TEST(In_test, Int) {
			const interval<int> a(1, 3);
			EXPECT_TRUE(a.in(1.2));
			EXPECT_TRUE(a.in(1.2));
			EXPECT_TRUE(a.in(a.lower()));
			EXPECT_TRUE(a.in(a.upper()));
			EXPECT_TRUE(!a.in(0.999));
			EXPECT_TRUE(a.in(1.456));
			EXPECT_TRUE(a.in(1.999));
			EXPECT_TRUE(!a.zeroIn());
			EXPECT_TRUE(a.in(2.325));
		}

		TEST(In_test, Short) {
			const interval<short> a(1, 3);
			EXPECT_TRUE(a.in(2));
			EXPECT_TRUE(a.in(a.lower()));
			EXPECT_TRUE(a.in(a.upper()));
			EXPECT_TRUE(!a.in(0.999));
			EXPECT_TRUE(a.in(1.456));
			EXPECT_TRUE(a.in(1.999));
			EXPECT_TRUE(!a.zeroIn());
			EXPECT_TRUE(a.in(2.325));
		}

		TEST(In_test, Long) {
			const interval <long> a(1, 3);
			EXPECT_TRUE(a.in(1.2));
			EXPECT_TRUE(a.in(a.lower()));
			EXPECT_TRUE(a.in(a.upper()));
			EXPECT_TRUE(!a.in(0.999));
			EXPECT_TRUE(a.in(1.456));
			EXPECT_TRUE(a.in(1.999));
			EXPECT_TRUE(!a.zeroIn());
			EXPECT_TRUE(a.in(2.325));
		}

	}
	namespace method_zero_in_test
	{
		TEST(zero_In_test, Int) {
			const interval<int> a(1, 3);
			EXPECT_TRUE(!a.zeroIn());
			const interval<int> b(-1, 3);
			EXPECT_TRUE(b.zeroIn());
			const interval<int> c(0, 0);
			EXPECT_TRUE(c.zeroIn());
		}

		TEST(zero_In_test, Short) {
			const interval<short> a(1, 3);
			EXPECT_TRUE(!a.zeroIn());
			const interval<short> b(-1, 3);
			EXPECT_TRUE(b.zeroIn());
			const interval<short> c(0, 0);
			EXPECT_TRUE(c.zeroIn());
		}

		TEST(zero_In_test, Long) {
			const interval<long> a(1, 3);
			EXPECT_TRUE(!a.zeroIn());
			const interval<long> b(-1, 3);
			EXPECT_TRUE(b.zeroIn());
			const interval<long> c(0, 0);
			EXPECT_TRUE(c.zeroIn());
		}

	}
	namespace method_overlap_test
	{
		TEST(Overlap, Int) {
			const interval<int> a(1, 3);
			const interval<int> b(-1, 3);
			const interval<int> c(0, 0);
			EXPECT_TRUE(a.overlap(b));
			EXPECT_TRUE(!a.overlap(c));
			EXPECT_TRUE(b.overlap(a));
			EXPECT_TRUE(b.overlap(c));
		}

		TEST(Overlap, Short) {
			const interval<short> a(1, 3);
			const interval<short> b(-1, 3);
			const interval<short> c(0, 0);
			EXPECT_TRUE(a.overlap(b));
			EXPECT_TRUE(!a.overlap(c));
			EXPECT_TRUE(b.overlap(a));
			EXPECT_TRUE(b.overlap(c));
		}

		TEST(Overlap, Long) {
			const interval<long> a(1, 3);
			const interval<long> b(-1, 3);
			const interval<long> c(0, 0);
			EXPECT_TRUE(a.overlap(b));
			EXPECT_TRUE(!a.overlap(c));
			EXPECT_TRUE(b.overlap(a));
			EXPECT_TRUE(b.overlap(c));
		}
	}

	namespace arithmetic_operators_tests
	{
		TEST(test_operator_plus_equal, Int) {
			interval<int> a(1, 3);
			const interval<int> b(-1, 3);
			a += b;
			EXPECT_TRUE(a.lower() == -1 + 1);
			EXPECT_TRUE(a.upper() == 3 + 3);
		}

		TEST(test_operator_plus_equal, Short) {
			interval<short> a(1, 3);
			const interval<short> b(-1, 3);
			a += b;
			EXPECT_TRUE(a.lower() == -1 + 1);
			EXPECT_TRUE(a.upper() == 3 + 3);
		}

		TEST(test_operator_plus_equal, Long) {
			interval<long> a(1, 3);
			const interval<long> b(-1, 3);
			a += b;
			EXPECT_TRUE(a.lower() == -1 + 1);
			EXPECT_TRUE(a.upper() == 3 + 3);
		}

		TEST(test_operator_minus_equal, Int) {
			interval<int> a(1, 3);
			const interval<int> b(-1, 3);
			a -= b;
			EXPECT_TRUE(a.lower() == 1 - 3);
			EXPECT_TRUE(a.upper() == 3 - -1);
		}

		TEST(test_operator_minus_equal, Short) {
			interval<short> a(1, 3);
			const interval<short> b(-1, 3);
			a -= b;
			EXPECT_TRUE(a.lower() == 1 - 3);
			EXPECT_TRUE(a.upper() == 3 - -1);
		}

		TEST(test_operator_minus_equal, Long) {
			interval<long> a(1, 3);
			const interval<long> b(-1, 3);
			a -= b;
			EXPECT_TRUE(a.lower() == 1 - 3);
			EXPECT_TRUE(a.upper() == 3 - -1);
		}

		TEST(test_operator_mul_equal, Int) {
			interval<int> a(1, 3);
			const interval<int> b(-1, 3);
			a *= b;
			EXPECT_EQ(a.lower(), -3);
			EXPECT_EQ(a.upper(), 9);
		}

		TEST(test_operator_mul_equal, Short) {
			interval<short> a(1, 3);
			const interval<short> b(-1, 3);
			a *= b;
			EXPECT_EQ(a.lower(), -3);
			EXPECT_EQ(a.upper(), 9);
		}

		TEST(test_operator_mul_equal, Long) {
			interval<long> a(1, 3);
			const interval<long> b(-1, 3);
			a *= b;
			EXPECT_EQ(a.lower(), -3);
			EXPECT_EQ(a.upper(), 9);
		}

		TEST(test_operator_div_equal, Int) {
			interval<int> a(1, 3);
			const interval<int> b(2, 3);
			a /= b;
			EXPECT_EQ(a.lower(), 0);
			EXPECT_EQ(a.upper(), 1);
		}

		TEST(test_operator_div_equal, Short) {
			interval<short> a(1, 3);
			const interval<short> b(2, 3);
			a /= b;
			EXPECT_EQ(a.lower(), 0);
			EXPECT_EQ(a.upper(), 1);
		}

		TEST(test_operator_div_equal, Long) {
			interval<long> a(1, 3);
			const interval<long> b(2, 3);
			a /= b;
			EXPECT_EQ(a.lower(), 0);
			EXPECT_EQ(a.upper(), 1);
		}
	}
	namespace comparison_operators_tests
	{
		TEST(operator_less, Int) {
			interval<int> a(4, 5);
			const interval<int> b(-1, 3);
			EXPECT_TRUE(b < a);
		}

		TEST(operator_less, Short) {
			interval<short> a(4, 5);
			const interval<short> b(-1, 3);
			EXPECT_TRUE(b < a);
		}

		TEST(operator_less, Long) {
			interval<long> a(4, 5);
			const interval<long> b(-1, 3);
			EXPECT_TRUE(b < a);
		}

		TEST(operator_greater, Int) {
			interval<int> a(4, 5);
			const interval<int> b(-1, 3);
			EXPECT_TRUE(a > b);
		}

		TEST(operator_greater, Short) {
			interval<short> a(4, 5);
			const interval<short> b(-1, 3);
			EXPECT_TRUE(a > b);
		}

		TEST(operator_greater, Long) {
			interval<long> a(4, 5);
			const interval<long> b(-1, 3);
			EXPECT_TRUE(a > b);
		}

		TEST(operator_less_equal, Int) {
			interval<int> a(4, 5);
			const interval<int> b(-1, 3);
			EXPECT_TRUE(b <= a);
		}

		TEST(operator_less_equal, Short) {
			interval<short> a(4, 5);
			const interval<short> b(-1, 3);
			EXPECT_TRUE(b <= a);
		}

		TEST(operator_less_equal, Long) {
			interval<long> a(4, 5);
			const interval<long> b(-1, 3);
			EXPECT_TRUE(b <= a);
		}

		TEST(operator_greater_equal, Int) {
			interval<int> a(4, 5);
			const interval<int> b(-1, 3);
			EXPECT_TRUE(a >= b);
		}

		TEST(operator_greater_equal, Short) {
			interval<short> a(4, 5);
			const interval<short> b(-1, 3);
			EXPECT_TRUE(a >= b);
		}

		TEST(operator_greater_equal, Long) {
			interval<long> a(4, 5);
			const interval<long> b(-1, 3);
			EXPECT_TRUE(a >= b);
		}
		TEST(operator_not_equal, Int) {
			interval<int> a(4, 5);
			const interval<int> b(-1, 3);
			EXPECT_TRUE(a != b);
		}

		TEST(operator_not_equal, Short) {
			interval<short> a(4, 5);
			const interval<short> b(-1, 3);
			EXPECT_TRUE(a != b);
		}

		TEST(operator_not_equal, Long) {
			interval<long> a(4, 5);
			const interval<long> b(-1, 3);
			EXPECT_TRUE(a != b);
		}
		TEST(operator_equal, Int) {
			interval<int> a(4, 5);
			interval<int> b(4, 5);
			EXPECT_TRUE(a == b);
		}

		TEST(operator_equal, Short) {
			interval<short> a(4, 5);
			interval<short> b(4, 5);
			EXPECT_TRUE(a == b);
		}

		TEST(operator_equal, Long) {
			interval<long> a(4, 5);
			interval<long> b(4, 5);
			EXPECT_TRUE(a == b);
		}
	}
	namespace functions_test
	{
		TEST(Abs, Int) {
			EXPECT_EQ(abs(interval(-2, 3)), interval(0, 3));
		}
		TEST(Cos, Int) {
			EXPECT_EQ(cos(interval(-2, 3)), interval(static_cast<int>(std::min(cos(-2), cos(3))), 1));
		}
		TEST(Sin, Int) {
			EXPECT_EQ(sin(interval(-2, 3)), interval(-1, 1));
		}
		TEST(Square, Int) {
			EXPECT_EQ(square(interval(2, 3)), interval(2 * 2, 3 * 3));
		}
		TEST(Square_negative, Int) {
			EXPECT_EQ(square(interval(-2, 3)), interval(0, 3 * 3));
		}

	}
	namespace unary_minus_test
	{
		TEST(Unary_minus, Int) {
			EXPECT_EQ(-interval(-2, 3), -1 * interval(-2, 3));
		}
		TEST(Unary_minus, Long) {
			EXPECT_EQ(-interval(-2l, 3l), -1 * interval(-2l, 3l));
		}

	}
	
}

namespace float_types
{
	namespace method_in_test
	{
		TEST(In_test, Float) {
			const interval<float> a(1.45f, 2.324f);
			EXPECT_TRUE(!a.in(1.2));
			EXPECT_TRUE(a.in(a.lower()));
			EXPECT_TRUE(a.in(a.upper()));
			EXPECT_TRUE(!a.in(1.346));
			EXPECT_TRUE(a.in(1.456));
			EXPECT_TRUE(a.in(2.323));
			EXPECT_TRUE(!a.zeroIn());
			EXPECT_TRUE(!a.in(2.325));
		}

		TEST(In_test, Double) {
			const interval<double> a(1.45, 2.324);
			EXPECT_TRUE(!a.in(1.2));
			EXPECT_TRUE(a.in(a.lower()));
			EXPECT_TRUE(a.in(a.upper()));
			EXPECT_TRUE(!a.in(1.346));
			EXPECT_TRUE(a.in(1.456));
			EXPECT_TRUE(a.in(2.323));
			EXPECT_TRUE(!a.zeroIn());
			EXPECT_TRUE(!a.in(2.325));
		}

		TEST(In_test, LongDouble) {
			const interval <long double> a(1.45, 2.324);
			EXPECT_TRUE(!a.in(1.2));
			EXPECT_TRUE(a.in(a.lower()));
			EXPECT_TRUE(a.in(a.upper()));
			EXPECT_TRUE(!a.in(1.346));
			EXPECT_TRUE(a.in(1.456));
			EXPECT_TRUE(a.in(2.323));
			EXPECT_TRUE(!a.zeroIn());
			EXPECT_TRUE(!a.in(2.325));
		}
	}
	namespace method_zero_in_test
	{
		TEST(zero_In_test, Float) {
			const interval<float> a(1.4f, 3.45f);
			EXPECT_TRUE(!a.zeroIn());
			const interval<float> b(-1.4f, 3.45f);
			EXPECT_TRUE(b.zeroIn());
			const interval<float> c(0.f, 0.f);
			EXPECT_TRUE(c.zeroIn());
		}

		TEST(zero_In_test, Double) {
			const interval<double> a(1.4, 3.24);
			EXPECT_TRUE(!a.zeroIn());
			const interval<double> b(-1.4, 3.24);
			EXPECT_TRUE(b.zeroIn());
			const interval<double> c(0.0, 0.0);
			EXPECT_TRUE(c.zeroIn());
		}

		TEST(zero_In_test, LongDouble) {
			const interval<long double> a(1.4, 3.24);
			EXPECT_TRUE(!a.zeroIn());
			const interval<long double> b(-1.4, 3.24);
			EXPECT_TRUE(b.zeroIn());
			const interval<long double> c(0.0, 0.0);
			EXPECT_TRUE(c.zeroIn());
		}

	}
	namespace method_overlap_test
	{
		TEST(Overlap, Float) {
			const interval<float> a(1, 3);
			const interval<float> b(-1, 3);
			const interval<float> c(0, 0);
			EXPECT_TRUE(a.overlap(b));
			EXPECT_TRUE(!a.overlap(c));
			EXPECT_TRUE(b.overlap(a));
			EXPECT_TRUE(b.overlap(c));
		}

		TEST(Overlap, Double) {
			const interval<double> a(1, 3);
			const interval<double> b(-1, 3);
			const interval<double> c(0, 0);
			EXPECT_TRUE(a.overlap(b));
			EXPECT_TRUE(!a.overlap(c));
			EXPECT_TRUE(b.overlap(a));
			EXPECT_TRUE(b.overlap(c));
		}

		TEST(Overlap, LongDouble) {
			const interval<long double> a(1, 3);
			const interval<long double> b(-1, 3);
			const interval<long double> c(0, 0);
			EXPECT_TRUE(a.overlap(b));
			EXPECT_TRUE(!a.overlap(c));
			EXPECT_TRUE(b.overlap(a));
			EXPECT_TRUE(b.overlap(c));
		}
	}

	namespace arithmetic_operators_tests
	{
		TEST(test_operator_plus_equal, Float) {
			interval<float> a(1, 3);
			const interval<float> b(-1, 3);
			a += b;
			EXPECT_TRUE(a.lower() == -1 + 1);
			EXPECT_TRUE(a.upper() == 3 + 3);
		}

		TEST(test_operator_plus_equal, Double) {
			interval<double> a(1, 3);
			const interval<double> b(-1, 3);
			a += b;
			EXPECT_TRUE(a.lower() == -1 + 1);
			EXPECT_TRUE(a.upper() == 3 + 3);
		}

		TEST(test_operator_plus_equal, LongDouble) {
			interval<long double> a(1, 3);
			const interval<long double> b(-1, 3);
			a += b;
			EXPECT_TRUE(a.lower() == -1 + 1);
			EXPECT_TRUE(a.upper() == 3 + 3);
		}

		TEST(test_operator_minus_equal, Float) {
			interval<float> a(1, 3);
			const interval<float> b(-1, 3);
			a -= b;
			EXPECT_TRUE(a.lower() == 1 - 3);
			EXPECT_TRUE(a.upper() == 3 - -1);
		}

		TEST(test_operator_minus_equal, Double) {
			interval<double> a(1, 3);
			const interval<double> b(-1, 3);
			a -= b;
			EXPECT_TRUE(a.lower() == 1 - 3);
			EXPECT_TRUE(a.upper() == 3 - -1);
		}

		TEST(test_operator_minus_equal, LongDouble) {
			interval<long double> a(1, 3);
			const interval<long double> b(-1, 3);
			a -= b;
			EXPECT_TRUE(a.lower() == 1 - 3);
			EXPECT_TRUE(a.upper() == 3 - -1);
		}

		TEST(test_operator_mul_equal, Float) {
			interval<float> a(1, 3);
			const interval<float> b(-1, 3);
			a *= b;
			EXPECT_EQ(a.lower(), -3);
			EXPECT_EQ(a.upper(), 9);
		}

		TEST(test_operator_mul_equal, Double) {
			interval<double> a(1, 3);
			const interval<double> b(-1, 3);
			a *= b;
			EXPECT_EQ(a.lower(), -3);
			EXPECT_EQ(a.upper(), 9);
		}

		TEST(test_operator_mul_equal, LongDouble) {
			interval<long double> a(1, 3);
			const interval<long double> b(-1, 3);
			a *= b;
			EXPECT_EQ(a.lower(), -3);
			EXPECT_EQ(a.upper(), 9);
		}

		TEST(test_operator_div_equal, Float) {
			interval<float> a(1, 3);
			const interval<float> b(2, 3);
			a /= b;
			EXPECT_TRUE(details::equal(a.lower(), float(1.0 / 3.0)));
			EXPECT_EQ(a.upper(), 1.5);
		}

		TEST(test_operator_div_equal, Double) {
			interval<double> a(1, 3);
			const interval<double> b(2, 3);
			a /= b;
			EXPECT_TRUE(details::equal(a.lower(), double(1.0 / 3.0)));
			EXPECT_EQ(a.upper(), 1.5);
		}

		TEST(test_operator_div_equal, LongDouble) {
			interval<long double> a(1, 3);
			const interval<long double> b(2, 3);
			a /= b;
			EXPECT_TRUE(details::equal(a.lower(), long double(1.0 / 3.0)));
			EXPECT_EQ(a.upper(), 1.5);
		}
	}
	namespace comparison_operators_tests
	{
		TEST(operator_less, Float) {
			interval<float> a(4, 5);
			const interval<float> b(-1, 3);
			EXPECT_TRUE(b < a);
		}

		TEST(operator_less, Double) {
			interval<double> a(4, 5);
			const interval<double> b(-1, 3);
			EXPECT_TRUE(b < a);
		}

		TEST(operator_less, LongDouble) {
			interval<long double> a(4, 5);
			const interval<long double> b(-1, 3);
			EXPECT_TRUE(b < a);
		}

		TEST(operator_greater, Float) {
			interval<float> a(4, 5);
			const interval<float> b(-1, 3);
			EXPECT_TRUE(a > b);
		}

		TEST(operator_greater, Double) {
			interval<double> a(4, 5);
			const interval<double> b(-1, 3);
			EXPECT_TRUE(a > b);
		}

		TEST(operator_greater, LongDouble) {
			interval<long double> a(4, 5);
			const interval<long double> b(-1, 3);
			EXPECT_TRUE(a > b);
		}

		TEST(operator_less_equal, Float) {
			interval<float> a(4, 5);
			const interval<float> b(-1, 3);
			EXPECT_TRUE(b <= a);
		}

		TEST(operator_less_equal, Double) {
			interval<double> a(4, 5);
			const interval<double> b(-1, 3);
			EXPECT_TRUE(b <= a);
		}

		TEST(operator_less_equal, LongDouble) {
			interval<long double> a(4, 5);
			const interval<long double> b(-1, 3);
			EXPECT_TRUE(b <= a);
		}

		TEST(operator_greater_equal, Float) {
			interval<float> a(4, 5);
			const interval<float> b(-1, 3);
			EXPECT_TRUE(a >= b);
		}

		TEST(operator_greater_equal, Double) {
			interval<double> a(4, 5);
			const interval<double> b(-1, 3);
			EXPECT_TRUE(a >= b);
		}

		TEST(operator_greater_equal, LongDouble) {
			interval<long double> a(4, 5);
			const interval<long double> b(-1, 3);
			EXPECT_TRUE(a >= b);
		}
		TEST(operator_not_equal, Float) {
			interval<float> a(4, 5);
			const interval<float> b(-1, 3);
			EXPECT_TRUE(a != b);
		}

		TEST(operator_not_equal, Double) {
			interval<double> a(4, 5);
			const interval<double> b(-1, 3);
			EXPECT_TRUE(a != b);
		}

		TEST(operator_not_equal, LongDouble) {
			interval<long double> a(4, 5);
			const interval<long double> b(-1, 3);
			EXPECT_TRUE(a != b);
		}
		TEST(operator_equal, Float) {
			interval<float> a(4, 5);
			interval<float> b(4, 5);
			EXPECT_TRUE(a == b);
		}

		TEST(operator_equal, Double) {
			interval<double> a(4, 5);
			interval<double> b(4, 5);
			EXPECT_TRUE(a == b);
		}

		TEST(operator_equal, LongDouble) {
			interval<long double> a(4, 5);
			interval<long double> b(4, 5);
			EXPECT_TRUE(a == b);
		}
	}

	namespace functions_test
	{
		TEST(Abs, Double) {
			EXPECT_EQ(abs(interval(-2.0, 3.0)), interval(0.0, 3.0));
		}
		TEST(Cos, Double) {
			EXPECT_EQ(cos(interval(-2.0, 3.0)), interval(std::min(cos(-2.0), cos(3.0)), 1.0));
		}
		TEST(Sin, Double) {
			EXPECT_EQ(sin(interval(-2.0, 3.0)), interval(-1.0, 1.0));
		}
		TEST(Square, Double) {
			EXPECT_EQ(square(interval(2.0, 3.0)), interval(2.0 * 2.0, 3.0 * 3.0));
		}
		TEST(Square_negative, Double) {
			EXPECT_EQ(square(interval(-2.0, 3.0)), interval(0.0, 3.0 * 3.0));
		}

	}
	namespace precision_test
	{
		TEST(Precision, Double) {
			EXPECT_TRUE(details::equal(sin(2.0), sin(2.0 + 20 * M_PI)));
		}
		TEST(Precision, Float) {
			EXPECT_TRUE(details::equal(static_cast<float>(sin(2.0)), static_cast<float>(sin(2.0 + 20 * M_PI))));
		}
	}

	namespace unary_minus_test
	{
		TEST(Unary_minus, Float) {
			EXPECT_EQ(-interval(-2.f, 3.f), -1 * interval(-2.f, 3.f));
		}
		TEST(Unary_minus, Double) {
			EXPECT_EQ(-interval(-2.0, 3.0), -1 * interval(-2.0, 3.0));
		}

	}
}

