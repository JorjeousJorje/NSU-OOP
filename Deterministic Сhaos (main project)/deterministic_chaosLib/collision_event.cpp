#include "collision_event.hpp"

collision_event::collision_event(particle* par1_, particle* par2_, float event_time_) : event_time(event_time_), par1(par1_), par2(par2_)
{
	if (par1 != nullptr) {
		count_par1 = par1->get_collisions();
	}
	else {
		count_par1 = -1;
	}
	if (par2 != nullptr) {
		count_par2 = par2->get_collisions();
	}
	else {
		count_par2 = -1;
	}
}

bool collision_event::is_valid() const
{

	if (par1 != nullptr && par1->get_collisions() != count_par1)
	{
		return false;
	}
	if (par2 != nullptr && par2->get_collisions() != count_par2)
	{
		return false;
	}
	return true;
}
