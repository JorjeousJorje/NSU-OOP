#pragma once
#include "particle.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include "circle_particle_parameters.hpp"


class circle_particle final : public particle
{
public:
	circle_particle() = delete;

	explicit circle_particle(circle_particle_parameters& params) : particle{ params }, shape_{ params.rad_ }
	{
		set_position(params_.spat_params_.coordinates_.x, params_.spat_params_.coordinates_.y);
		shape_.setOrigin(params.rad_, params.rad_);
	}

	
	const sf::CircleShape& shape() const override;

	void set_color(const sf::Color color) override
	{
		shape_.setFillColor(color);
	}

	void  set_position(float x, float y);
	void  set_position(vec2f vector);
	
	float radius() const;
	void  move(float friction) override;
	
private:
	sf::CircleShape shape_;
};