#include "flat_force.hpp"

float flat_force::calculate_force(const particle& left, const particle& right, const attribute_holder& holder, const float r = 0) const
{
	return holder.attraction(left.type(), right.type());
}
