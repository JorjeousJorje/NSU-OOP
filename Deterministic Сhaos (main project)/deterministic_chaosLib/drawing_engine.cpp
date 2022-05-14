#include "drawing_engine.hpp"
#include <functional>
#include <SFML/Window/Event.hpp>

drawing_engine::drawing_engine()
{
	window.create(sf::VideoMode(screen_weight, screen_height), "Simulation");
	if (!window.isOpen()) {
		throw std::bad_function_call();
	}
	window.setKeyRepeatEnabled(false);
}

void drawing_engine::draw_particles(std::vector<particle*> particles, size_t num_of_particles)
{
	window.clear(sf::Color::Black);
	for (size_t i = 0; i < num_of_particles; i++)
	{
		particles[i]->draw(window);
	}
}

void drawing_engine::draw_trace(std::vector<particle*> particles, std::vector<sf::Vertex>& trace, bool& redraw_line)
{
	if (!particles.empty()) {
		srand(static_cast<unsigned int>(time(nullptr)));
		sf::Vertex point(particles.front()->get_coordinate(), sf::Color::Red);
		if (redraw_line) {
			point.color = sf::Color::Blue;
		}
		trace.emplace_back(point);
		for (auto& points : trace) {
			window.draw(&points, 1, sf::Points);
		}
	}
}
