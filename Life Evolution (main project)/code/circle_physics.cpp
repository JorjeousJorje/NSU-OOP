#include "circle_physics.hpp"

bool circle_physics::horizontal_wall_collision(particle& particle) const
{
	auto& circle_p = dynamic_cast<circle_particle&>(particle);
	auto happened = false;

	if (circle_p.coordinates().y < 0) {
		circle_p.coordinates().y += screen_height;
		circle_p.set_position(circle_p.coordinates());
		happened = true;
	}
	else if (circle_p.coordinates().y >= screen_height) {
		circle_p.coordinates().y -= screen_height;
		circle_p.set_position(circle_p.coordinates());

		happened = true;
	}

	return happened;
}

bool circle_physics::vertical_wall_collision(particle& particle) const
{
	auto& circle_p = dynamic_cast<circle_particle&>(particle);
	auto happened = false;

	if (circle_p.coordinates().x < 0) {
		circle_p.coordinates().x += screen_width;
		circle_p.set_position(circle_p.coordinates());
		happened = true;
	}
	else if (circle_p.coordinates().x >= screen_width) {
		circle_p.coordinates().x -= screen_width;
		circle_p.set_position(circle_p.coordinates());
		happened = true;
	}

	return happened;
}
