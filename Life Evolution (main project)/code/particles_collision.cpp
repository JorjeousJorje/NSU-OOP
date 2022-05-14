#include "particles_collision.hpp"
#include "event_mediator.hpp"


void particles_collision::process(event_mediator& mediator) const
{
	mediator.particle_collisions(left_, right_);
}
