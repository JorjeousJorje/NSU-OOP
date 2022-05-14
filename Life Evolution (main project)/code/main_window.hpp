#pragma once
#include <functional>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "particle.hpp"

constexpr int screen_width = 1500;
constexpr int screen_height = 800;
constexpr int frame_rate = 120;

class main_window
{
public:
	main_window();

	void draw_particles(const std::vector<particle_uptr>& particles);

	bool is_open() const;
	void close();
	bool pool_event(sf::Event& event);

private:
	sf::RenderWindow window_{ sf::VideoMode{screen_width, screen_height, 24}, "Simulation" };
};