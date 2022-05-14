#include "universe.hpp"

void universe::reseed(universe_randomizer& randomizer, const float friction, physical_force& force)
{
	holder_.set_random_attributes(randomizer);
	born_random_particles();
	physics_.change_friction(friction);
	current_force_ = force;
}

void universe::set_population(const uint8_t types, const std::size_t particle_num)
{
	particles_.reserve(particle_num);
	holder_.resize_attributes(types);
	builder_.change_type_range(types - 1);
	particle_num_ = particle_num;
}

const std::vector<particle_uptr>& universe::universe_particles() const
{
	return particles_;
}