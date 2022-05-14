#include "impact_painter.hpp"
void impact_painter::draw_on_impact(particle* par, std::random_device& rnd_dev, bool pure_random)
{
	if (pure_random) {
		std::mt19937 rng(rnd_dev());
		std::uniform_int_distribution<std::mt19937::result_type> rand_color(56, 255);
		par->get_shape()->setFillColor(sf::Color(rand_color(rng), rand_color(rng), rand_color(rng)));

		return;
	}
	srand(static_cast<unsigned int>(time(nullptr)));
	par->get_shape()->setFillColor(sf::Color(std::rand() % 255, 0, std::rand() % 255));
}
