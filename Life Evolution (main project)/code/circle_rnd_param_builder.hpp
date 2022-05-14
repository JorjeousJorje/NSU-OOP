#pragma once
#include <memory>
#include "circle_particle_parameters.hpp"
#include "rnd_param_builder.hpp"

class circle_rnd_param_builder final : public rnd_param_builder
{
public:
	circle_rnd_param_builder(const float min_coord, const float max_coord,
							 const float min_speed, const float max_speed,
							 const uint8_t min_type, const uint8_t max_type,
							 const float min_radius, const float max_radius)
		: rnd_param_builder{ min_coord, max_coord, min_speed, max_speed, min_type, max_type }, radius_random_{ min_radius, max_radius } {}

	param_uptr construct(std::size_t id, const std::size_t universe_width, const std::size_t universe_height) override {

		return std::make_unique<circle_particle_parameters>(spatial_parameters(universe_width, universe_height), type_random_(), id, radius_random_());
	}

private:
	uniform_randomizer<float> radius_random_;
};