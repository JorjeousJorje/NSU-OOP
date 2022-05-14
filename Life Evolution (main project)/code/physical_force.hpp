#pragma once
#include "particle.hpp"
#include "attributes_holder.hpp"

class physical_force
{
public:
	virtual float calculate_force(const particle& left, const particle& right, const attribute_holder& holder, float r) const = 0;
	virtual ~physical_force() = default;
};
