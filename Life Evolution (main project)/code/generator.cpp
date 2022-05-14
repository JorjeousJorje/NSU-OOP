#include "generator.hpp"

std::array<std::mt19937::result_type, std::mt19937::state_size> random_data()
{
	std::array<std::mt19937::result_type, std::mt19937::state_size> data{};
	std::random_device rd;
	std::generate(std::begin(data), std::end(data), std::ref(rd));

	return data;
}

std::mt19937 random_generator()
{
	auto const& data = random_data();
	std::seed_seq seeds(std::begin(data), std::end(data));
	const std::mt19937 generator{ seeds };

	return generator;
}
