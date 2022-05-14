#pragma once
#include "vector2f.hpp"

struct spatial_params
{
	spatial_params(const vec2f speed, const vec2f coordinates) : speed_{ speed }, coordinates_{ coordinates } {}
	vec2f speed_;
	vec2f coordinates_;
};


struct particle_parameters
{
	particle_parameters(spatial_params spat_params, const uint8_t type, const std::size_t id) :
		spat_params_{ spat_params }, type_{ type }, id_{id} {}

	spatial_params spat_params_;
	
	uint8_t type_;
	std::size_t id_;

	virtual ~particle_parameters() = default;
protected:
	virtual void dummy() const = 0;
};

using param_uptr = std::unique_ptr<particle_parameters>;
