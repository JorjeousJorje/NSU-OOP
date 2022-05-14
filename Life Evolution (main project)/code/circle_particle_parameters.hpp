#pragma once
#include "particle_parameters.hpp"

struct circle_particle_parameters final : particle_parameters
{
	circle_particle_parameters(spatial_params spat_params, const uint8_t type, const std::size_t id, const float rad) :
		particle_parameters{ spat_params, type, id}, rad_{ rad }  {}

	float rad_;
protected:
	void dummy() const override {}
};
