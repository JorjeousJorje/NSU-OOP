#include "normal_force.hpp"

float normal_force::calculate_force(const particle& left, const particle& right, const attribute_holder& holder, const float r) const
{
	const auto particles_minr = holder.minr(left.type(), right.type());
	const auto particles_maxr = holder.maxr(left.type(), right.type());

	const auto numer = 2.0f * std::abs(r - 0.5f * (particles_maxr + particles_minr));
	const auto denom = particles_maxr - particles_minr;
	return holder.attraction(left.type(), right.type()) * (1.0f - numer / denom);
}
