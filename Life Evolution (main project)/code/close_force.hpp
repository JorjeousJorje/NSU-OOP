#pragma once
#include "physical_force.hpp"
constexpr float r_smooth = 2.0f;


class close_force final : public physical_force
{
public:

	float calculate_force(const particle& left, const particle& right, const attribute_holder& holder, const float r) const override
	{
		const auto particles_minr = holder.minr(left.type(), right.type());
		return r_smooth * particles_minr * (1.0f / (particles_minr + r_smooth) - 1.0f / (r + r_smooth));
	}
};
