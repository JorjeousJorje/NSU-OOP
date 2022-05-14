#include "horizontal_wall_collision.hpp"
#include "event_mediator.hpp"



void horizontal_wall_collision::process(event_mediator& mediator) const
{
	mediator.horizontal_wall_collision(particle_);
}
