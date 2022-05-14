#include "circle_particle.hpp"

const sf::CircleShape& circle_particle::shape() const
{
	return shape_;
}

void circle_particle::set_position(const float x, const float y)
{
	shape_.setPosition(x, y);
}

void circle_particle::set_position(const vec2f vector)
{
	shape_.setPosition(vector.x, vector.y);
}

float circle_particle::radius() const
{
	return shape_.getRadius();
}

void circle_particle::move(const float friction)
{
	params_.spat_params_.coordinates_ += params_.spat_params_.speed_;
	shape_.move(params_.spat_params_.speed_);
	params_.spat_params_.speed_ *= 1.f - friction;
}
