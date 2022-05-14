#pragma once
#include "generator.hpp"

template<class T, class Enable = void>
class normal_randomizer {};

template<class T>
class normal_randomizer<T, std::enable_if_t<std::is_integral_v<T>>>
{
public:
	normal_randomizer(const T a, const T b) : dist_{ a, b } {}

	T operator()()
	{
		auto& gen = generator::get_instance();
		return dist_(gen.engine());
	}
	
private:
	std::normal_distribution<T> dist_;
};


template<class T>
class normal_randomizer<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
public:
	normal_randomizer(const T a, const T b) : dist_{ a, b } {}

	T operator()()
	{
		auto& gen = generator::get_instance();
		return dist_(gen.engine());
	}

	vec2f random_vector() 
	{
		return vec2f{ operator()(), operator()() };
	}
private:
	std::normal_distribution<T> dist_;
};