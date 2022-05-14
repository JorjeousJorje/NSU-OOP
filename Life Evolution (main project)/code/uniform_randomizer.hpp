#pragma once
#include "generator.hpp"

template<class T, class Enable = void>
class uniform_randomizer {};

template<class T>
class uniform_randomizer<T, std::enable_if_t<std::is_integral_v<T>>>
{
public:
	uniform_randomizer(const T a, const T b) : dist_{ a, b } {}

	T operator()() const
	{
		auto& gen = generator::get_instance();
		return dist_(gen.engine());
	}
private:
	std::uniform_int_distribution<T> dist_;
};


template<class T>
class uniform_randomizer<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
public:
	uniform_randomizer(const T a, const T b) : dist_{ a, b } {}

	T operator()() const
	{
		auto& gen = generator::get_instance();
		return dist_(gen.engine());
	}

	vec2f random_vector() const
	{
		return vec2f{ operator()(), operator()() };
	}
	
private:
	std::uniform_real_distribution<T> dist_;
};