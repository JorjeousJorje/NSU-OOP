#include "particle_physics.hpp"

void particle_physics::attraction(particle& left, const particle& right, const attribute_holder& holder, const physical_force& force) const
{
	if (left.id() == right.id()) {
		return;
	}
	auto dr = right.coordinates() - left.coordinates();
	const auto r_squared = dr.squared_module();

	const auto particles_minr = holder.minr(left.type(), right.type());
	const auto particles_maxr = holder.maxr(left.type(), right.type());

	if (r_squared > particles_maxr * particles_maxr) {
		return;
	}

	const auto r = std::sqrt(r_squared);
	auto result_force = 0.0f;
	if (r > particles_minr) {
		result_force = force.calculate_force(left, right, holder, r);
	}
	else {
		result_force = r_smooth * particles_minr * (1.0f / (particles_minr + r_smooth) - 1.0f / (r + r_smooth));
	}

	left.speed() += result_force * dr / r;
}
