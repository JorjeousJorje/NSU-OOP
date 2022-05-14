#pragma once
#include "attributes_holder.hpp"
#include "main_window.hpp"
#include "forces_header.hpp"

class particle_physics
{
public:
	explicit particle_physics(const float friction) : friction_(friction) {}


	virtual void attraction(particle& left, const particle& right, const attribute_holder& holder, const physical_force& force) const;
	virtual bool vertical_wall_collision(particle& particle) const = 0;
	virtual bool horizontal_wall_collision(particle& particle) const = 0;


	[[nodiscard]] float system_friction() const {
		return friction_;
	}

	void change_friction(const float new_friction) {
		friction_ = new_friction;
	}
	
	virtual ~particle_physics() = default;
private:
	float friction_;
};