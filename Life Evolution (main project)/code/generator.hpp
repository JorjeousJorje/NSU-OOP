#pragma once
#include <array>
#include <random>
#include "vector2f.hpp"

std::array<std::mt19937::result_type, std::mt19937::state_size> random_data();
std::mt19937 random_generator();


class generator
{
	std::mt19937 engine_{ random_generator() };
	explicit generator() = default;
	
public:
	static generator& get_instance() {
		static generator instance{};
		return instance;
	}

	[[nodiscard]] std::mt19937& engine() {
		return engine_;
	}
};