#include "particle.hpp"

vec2f& particle::speed()
{
	return params_.spat_params_.speed_;
}

vec2f particle::speed() const
{
	return params_.spat_params_.speed_;
}

vec2f& particle::coordinates()
{
	return params_.spat_params_.coordinates_;
}

vec2f particle::coordinates() const
{
	return params_.spat_params_.coordinates_;
}

uint8_t particle::type() const
{
	return params_.type_;
}

std::size_t particle::id() const
{
	return params_.id_;
}
