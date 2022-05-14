#pragma once
#include "physical_force.hpp"

class normal_force final : public physical_force
{
public:
	[[nodiscard]] float calculate_force(const particle& left, const particle& right, const attribute_holder& holder, float r) const override;

};
