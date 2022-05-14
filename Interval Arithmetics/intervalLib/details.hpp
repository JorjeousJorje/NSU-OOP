#pragma once
#include <array>
#include <string>


namespace details
{
	template <class T>
	constexpr decltype(auto) infinity() {
		if constexpr (std::is_floating_point_v<T>) {
			return std::numeric_limits<T>::infinity();
		}
		else {
			return std::numeric_limits<T>::max();
		}
	}

	template <class T>
	constexpr decltype(auto) epsilon() {
		if constexpr (std::is_same_v<T, double> || std::is_same_v<T, long double>) {
			return 1e-13;
		}
		else if constexpr (std::is_same_v<T, float>) {
			return FLT_EPSILON;
		}
	}

	template <typename T1, typename T2>
	constexpr bool equal(T1 x, T2 y) {
		if constexpr (std::is_floating_point_v<T1> && std::is_floating_point_v<T2>) {
			return std::fabs(x - y) <= epsilon<T1>();
		}
		else {
			return x == y;
		}
	}

	template<typename T>
	void insert_number(std::array<T, 2>& found_numbers, const std::string& str, const size_t num_ch, const size_t index)
	{
		const auto is_inf = str.find('i', num_ch);
		if (is_inf == std::string::npos) {
			if constexpr (std::is_floating_point_v<T>) {
				found_numbers[index] = std::stold(str);
			}
			else {
				found_numbers[index] = std::stoll(str);
			}
		}
		else {
			found_numbers[index] = '-' == str[is_inf - 1] ?
				-infinity<T>() : infinity<T>();
		}
	}

}
