#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "particle.hpp"


class drawing_engine final
{
public:
	drawing_engine();

	sf::RenderWindow& get_render_window() { return window; }
	void draw_particles(std::vector<particle*> particles, size_t num_of_particles);
	void draw_trace(std::vector<particle*> particles, std::vector<sf::Vertex>& trace, bool& redraw_line);

	static const inline float frequency = 270;
	static const inline unsigned int screen_weight = 1800;
	static const inline unsigned int screen_height = 900;

	~drawing_engine() = default;
private:
	sf::RenderWindow window;
};
