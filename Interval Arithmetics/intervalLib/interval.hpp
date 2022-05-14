#pragma once
#include <iomanip>
#include <iostream>
#include <regex>
#include <algorithm>

#include "details.hpp"

template<typename T>
class interval  final
{
	static_assert(std::is_arithmetic_v<T>, "Wrong type: this class doesn't support this type");
public:
	interval() = default;
	interval(T value);
	interval(T lower, T upper);


	[[nodiscard]] T width() const;
	[[nodiscard]] T upper() const { return up; }
	[[nodiscard]] T lower() const { return low; }

	template<typename T1>
	[[nodiscard]] bool in(T1 value) const;

	[[nodiscard]] bool zeroIn() const;
	[[nodiscard]] bool overlap(const interval& interval) const;
	[[nodiscard]] bool isValid() const;

	void assign(T lower, T upper);

	interval& operator += (const interval& right);
	interval& operator -= (const interval& right);
	interval& operator *= (const interval& right);
	interval& operator /= (const interval& right);

	friend interval operator+(const interval& left, const interval& right)
	{
		if (left.isValid() && right.isValid()) {
			interval temporary{ left };
			return temporary += right;
		}
		return interval{ T{1}, T{-1} };
	}
	friend interval operator-(const interval& left, const interval& right)
	{
		if (left.isValid() && right.isValid()) {
			interval temporary{ left };
			return temporary -= right;
		}
		return interval{ T{1}, T{-1} };
	}
	friend interval operator*(const interval& left, const interval& right)
	{
		if (left.isValid() && right.isValid()) {
			interval temporary{ left };
			return temporary *= right;
		}
		return interval{ T{1}, T{-1} };
	}
	friend interval operator/(const interval& left, const interval& right)
	{
		if (left.isValid() && right.isValid()) {
			interval temporary{ left };
			return temporary /= right;
		}
		return interval{ T{1}, T{-1} };
	}


	friend bool operator<(const interval& left, const interval& right)
	{
		return left.isValid() && right.isValid() && left.up < right.low;
	}
	friend bool operator>(const interval& left, const interval& right)
	{
		return left.isValid() && right.isValid() && left.low > right.up;
	}
	friend bool operator<=(const interval& left, const interval& right)
	{
		return left.isValid() && right.isValid() && (std::islessequal(left.up, right.low) || left == right);
	}
	friend bool operator>=(const interval& left, const interval& right)
	{
		return left.isValid() && right.isValid() && (std::isgreaterequal(left.low, right.up) || left == right);
	}
	friend bool operator!=(const interval& left, const interval& right)
	{
		return !(left == right);
	}
	friend bool operator==(const interval& left, const interval& right)
	{
		return left.isValid() && right.isValid() && (details::equal(left.up, right.up) && details::equal(left.low, right.low));
	}

	interval operator-() const;

	friend std::ostream& operator<<(std::ostream& out, const interval& i_val)
	{
		if constexpr (std::is_floating_point_v<T>) {
			return out << std::setprecision(3) << '[' << i_val.low << ',' << i_val.up << ']';
		}
		else {
			return out << '[' << i_val.low << ',' << i_val.up << ']';
		}
	}
	friend std::istream& operator>>(std::istream& in, interval& i_val)
	{
		std::string buffer{};
		std::smatch match{};
		std::getline(in >> std::ws, buffer);

		while (!std::regex_search(buffer, match, regex)) {
			std::cout << "wrong form of an interval." << std::endl
				<< "supposed to be: (num, num) or less numbers in brackets" << std::endl;

			buffer.clear();
			std::getline(in >> std::ws, buffer);
		}

		const auto matched = match.str();

		std::array<T, 2> found_numbers{};
		size_t comma_pos;
		auto num_ch = matched.find_first_not_of(" \n\f\r\t\v", 1);
		size_t index{ 0 };

		if ((comma_pos = matched.find_first_of(',', num_ch)) != std::string::npos) {
			details::insert_number<T>(found_numbers, matched.substr(num_ch, comma_pos - num_ch), num_ch, index);
			num_ch = matched.find_first_not_of(" \n\f\r\t\v", comma_pos + 1);
			++index;
		}
		details::insert_number<T>(found_numbers, matched.substr(num_ch, comma_pos - num_ch), num_ch, index);

		if (index == 1) {
			i_val = interval{ found_numbers[0], found_numbers[1] };
		}
		else {
			i_val = interval{ found_numbers[0] };
		}
		return in;
	}

private:
	T low{ 0 };
	T up{ 0 };

	static const inline std::regex regex{ R"(\s*?[(](\s*?-?\+?inf\s*?,?\s*?|\s*?-?\d+\s*?\.?\s*?\d*\s*?,?\s*?){1,2}[)]\s*?)" };
};

template <typename T>
interval<T>::interval(T value) : low(value), up(value) {}

template <typename T>
interval<T>::interval(T lower_, T upper_) : low(lower_), up(upper_) {}

template<typename T>
T interval<T>::width() const
{
	if (isValid()) {
		return static_cast<T>(std::fabs(up - low));
	}
	return -1;
}

template <typename T>
template <typename T1>
bool interval<T>::in(T1 value) const
{
	if (isValid()) {
		if constexpr (std::is_floating_point_v<T>) {
			if (std::isnan(value) || std::isinf(value)) {
				return false;
			}
		}
		return std::islessequal(value, up) && std::isgreaterequal(value, low);
	}
	return false;
}

template <typename T>
bool interval<T>::zeroIn() const
{
	if (isValid()) {
		return in(T{ 0 });
	}
	return false;
}

template <typename T>
bool interval<T>::overlap(const interval& interval) const
{
	if (isValid() && interval.isValid()) {
		return in(interval.up) || in(interval.low);
	}
	return false;
}

template<typename T>
bool interval<T>::isValid() const
{
	if constexpr (std::is_floating_point_v<T>) {
		if (std::isnan(up) || std::isinf(up) || std::isnan(low) || std::isinf(low) || low > up) {
			return false;
		}
	}
	else {
		if (low > up) {
			return false;
		}
	}
	return true;
}

template <typename T>
void interval<T>::assign(T lower_, T upper_)
{
	up = upper_;
	low = lower_;
}

template <typename T>
interval<T>& interval<T>::operator+=(const interval& right)
{
	if (isValid() && right.isValid()) {
		up += right.up;
		low += right.low;
		return *this;
	}
	this->assign(T(-1), T(1));
	return *this;
}

template <typename T>
interval<T>& interval<T>::operator-=(const interval& right)
{
	if (isValid() && right.isValid()) {
		up -= right.low;
		low -= right.up;
		return *this;
	}
	this->assign(T(-1), T(1));
	return *this;
}

template <typename T>
interval<T>& interval<T>::operator*=(const interval& right)
{
	if (isValid() && right.isValid()) {
		if (low > 0 && right.lower() > 0) {
			up *= right.upper();
			low *= right.lower();
			return *this;
		}
		const auto swaps = { up * right.upper(), up * right.lower(), low * right.upper(), low * right.lower() };
		std::tie(low, up) = std::minmax(swaps);

		return *this;
	}
	this->assign(T(-1), T(1));
	return  *this;
}

template<typename T>
interval<T> interval<T>::operator-() const
{
	if (isValid()) {
		return *this * T{ -1 };
	}
	return *this;
}

template <typename T>
interval<T>& interval<T>::operator/=(const interval& right)
{
	if (isValid() && right.isValid()) {
		if (!right.zeroIn()) {
			const auto swaps = { up / right.up, up / right.low, low / right.up, low / right.low };
			std::tie(low, up) = std::minmax(swaps);

			return *this;
		}
	}
	this->assign(T(1), T(-1));
	return *this;
}


namespace constants
{
	template <class T>
	constexpr decltype(auto) pi() {
		if constexpr (std::is_floating_point_v<T>) {
			return interval{ std::acos(-1), std::acos(-1) };
		}
		else {
			return interval{ static_cast<T>(std::floor(std::acos(-1))), static_cast<T>(std::ceil(std::acos(-1))) };
		}
	}

	template <class T>
	constexpr decltype(auto) pi_twice() {
		if constexpr (std::is_floating_point_v<T>) {
			return interval{ 2 * std::acos(-1), 2 * std::acos(-1) };
		}
		else {
			return interval{ static_cast<T>(std::floor(2 * std::acos(-1))), static_cast<T>(std::ceil(2 * std::acos(-1))) };
		}
	}

	template <class T>
	constexpr decltype(auto) pi_half() {
		if constexpr (std::is_floating_point_v<T>) {
			return interval{ std::acos(-1) / 2, std::acos(-1) / 2 };
		}
		else {
			return interval{ static_cast<T>(std::floor(std::acos(-1) / 2)), static_cast<T>(std::ceil(std::acos(-1) / 2)) };
		}
	}
}


template<typename T>
interval<T> fmod(const interval<T>& x, const interval<T>& y)
{
	if (x.isValid()) {
		auto yb = x.lower() < T{ 0 } ? y.lower() : y.upper();
		auto n = static_cast<T>(std::floor((x.lower() / yb)));
		return x - n * y;
	}
	return interval{ T{1}, T{-1} };
}

template<typename T>
interval<T> cos(const interval<T>& x)
{
	if (x.isValid()) {
		const interval x_real{ static_cast<double>(x.lower()), static_cast<double>(x.upper()) };
		auto pi2 = constants::pi_twice<double>();
		auto pi = constants::pi<double>();
		
		auto res = fmod(x_real, pi2);

		if (std::isgreaterequal(res.width(), pi2.lower())) {
			return interval{ T{-1}, T{1} };
		}
		if (std::isgreaterequal(res.lower(), pi.upper())) {
			const auto res_int = res - pi;
			return -cos(interval{ static_cast<T>(res_int.lower()), static_cast<T>(res_int.upper()) });
		}
		auto low = res.lower();
		auto up = res.upper();

		if (std::islessequal(up, pi.lower())) {
			return interval{ static_cast<T>(std::cos(up)), static_cast<T>(std::cos(low)) };
		}
		if (std::islessequal(up, pi2.lower())) {
			return interval{ T{-1}, static_cast<T>(std::cos(std::min((pi2.lower() - up), low))) };
		}
		return interval{ T{-1}, T{1} };
	}
	return interval{ T{1}, T{-1} };
}


template<typename T>
interval<T> sin(const interval<T>& x)
{
	if (x.isValid()) {
		return cos(x - constants::pi_half<T>());
	}
	return interval{ T{1}, T{-1} };
}

template<typename T>
interval<T> abs(const interval<T>& x)
{
	if (x.isValid()) {
		auto abs_up = std::abs(x.upper());
		auto abs_low = std::abs(x.lower());
		if (x.zeroIn()) {
			return interval{ T{0} , std::max(abs_up, abs_low) };
		}
		if (x < T{ 0 }) {
			return interval{ abs_up , abs_low };
		}
	}
	return x;
}

template<typename T>
interval<T> sqrt(const interval<T>& x)
{
	if (x.isValid() && (std::isgreaterequal(x.lower(), 0))) {
		return interval{ static_cast<T>(std::sqrt(x.lower())), static_cast<T>(std::sqrt(x.upper())) };
	}
	return interval{ T{1}, T{-1} };
}

template<typename T>
interval<T> square(const interval<T>& x)
{
	if (x.isValid()) {
		auto low = x.lower();
		auto up = x.upper();
		if (x.zeroIn()) {
			return interval{ T{0}, std::max(low * low, up * up) };
		}
		if (x < T{ 0 }) {
			return interval{ up * up, low * low };
		}
		return x * x;
	}
	return interval{ T{1}, T{-1} };
}