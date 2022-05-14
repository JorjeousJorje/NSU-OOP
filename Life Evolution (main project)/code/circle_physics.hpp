#pragma once
#include "particle_physics.hpp"
#include "circle_particle.hpp"

class circle_physics final : public particle_physics
{
public:
	explicit circle_physics(const float friction) : particle_physics{ friction } {}
	
	bool horizontal_wall_collision(particle& particle) const override;
	bool vertical_wall_collision(particle& particle) const override;
};
