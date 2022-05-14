#pragma once
#include "physical_force.hpp"

class flat_force final : public physical_force
{
public:

	float calculate_force(const particle& left, const particle& right, const attribute_holder& holder, float r) const override;
};
