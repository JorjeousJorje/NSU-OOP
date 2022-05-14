#include "main_window.hpp"

main_window::main_window()
{
	window_.setFramerateLimit(frame_rate);
}

void main_window::draw_particles(const std::vector<particle_uptr>& particles)
{
	window_.clear();

	for (auto&& particle : particles) {

		window_.draw(particle->shape());
	}

	window_.display();
}

bool main_window::is_open() const
{
	return window_.isOpen();
}

void main_window::close()
{
	window_.close();
}

bool main_window::pool_event(sf::Event& event)
{
	return window_.pollEvent(event);
}
