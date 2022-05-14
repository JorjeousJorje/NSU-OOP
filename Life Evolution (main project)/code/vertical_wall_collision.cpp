#include "vertical_wall_collision.hpp"
#include "event_mediator.hpp"


void vertical_wall_collision::process(event_mediator& mediator) const
{
	mediator.vertical_wall_collision(particle_);
}
