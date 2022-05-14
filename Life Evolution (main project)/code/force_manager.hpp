#pragma once
#include "attributes_holder.hpp"
#include "particle.hpp"

constexpr float r_smooth = 2.0f;

class force_manager
{
public:
	explicit force_manager(const attribute_holder& holder) : holder_{holder} {}



	float normal_force(const particle& left, const particle& right, const vec2f dr = { 0,0 }) const {
		const auto particles_minr = holder_.minr(left.type(), right.type());
		const auto particles_maxr = holder_.maxr(left.type(), right.type());
		const auto r = dr.squared_module();

		const auto numer = 2.0f * std::abs(r - 0.5f * (particles_maxr + particles_minr));
		const auto denom = particles_maxr - particles_minr;
		return holder_.attraction(left.type(), right.type()) * (1.0f - numer / denom);
	}

	float flat_force(const particle& left, const particle& right) const
	{
		return holder_.attraction(left.type(), right.type());
	}


	float close_force(const particle& left, const particle& right, const vec2f dr = { 0,0 }) const
	{
		const auto r = dr.squared_module();
		const auto particles_minr = holder_.minr(left.type(), right.type());
		return r_smooth * particles_minr * (1.0f / (particles_minr + r_smooth) - 1.0f / (r + r_smooth));
	}
	
	
private:
	const attribute_holder& holder_;
};
