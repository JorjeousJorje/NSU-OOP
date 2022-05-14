#pragma once
#include "normal_randomizer.hpp"
#include "particle_parameters.hpp"
#include "uniform_randomizer.hpp"


class rnd_param_builder
{
public:
	rnd_param_builder(const float min_coord, const float max_coord,
					  const float min_speed, const float max_speed,
					  const uint8_t min_type, const uint8_t max_type)
		: coord_random_{ min_coord , max_coord }, speed_random_{ min_speed, max_speed }, type_random_{ min_type , max_type } {}

	virtual param_uptr construct(std::size_t id, std::size_t universe_width, std::size_t universe_height) = 0;

	spatial_params spatial_parameters(const std::size_t universe_width, const std::size_t universe_height)
	{
		auto coord_vector = coord_random_.random_vector() * 0.5f + 0.25f;
		coord_vector.x *= static_cast<float>(universe_width);
		coord_vector.y *= static_cast<float>(universe_height);
		
		return { speed_random_.random_vector() * 0.2f, coord_vector };
	}

	void change_type_range(const uint8_t max_type) {
		type_random_ = uniform_randomizer<int>{ 0,max_type };
	}

	virtual ~rnd_param_builder() = default;
protected:
	uniform_randomizer<float> coord_random_;
	normal_randomizer<float> speed_random_;
	uniform_randomizer<int> type_random_;
};
