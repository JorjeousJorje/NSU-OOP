#pragma once
#include "circle.hpp"

class collision_event final
{

public:
	collision_event(particle* par1_, particle* par2_, float event_time_);

	const float& get_event_time() const { return event_time; }
	bool is_valid()               const;

	particle* first_particle()  const { return par1; }
	particle* second_particle() const { return par2; }

	~collision_event() = default;

private:
	float event_time;
	particle* par1;
	particle* par2;
	size_t count_par1 = 0;
	size_t count_par2 = 0;
};

