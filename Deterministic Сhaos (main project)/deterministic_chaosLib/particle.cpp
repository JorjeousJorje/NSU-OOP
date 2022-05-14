#include "particle.hpp"
#include "drawing_engine.hpp"

particle::particle(sf::Shape* shape_, std::mt19937& rnd_gen, float vx, float vy, float mass_, bool random_mass) : shape(shape_), speed(vx, vy)
{
	if (random_mass) {
		const std::uniform_real_distribution<float> rnd_mass(15.f, 100.f);
		mass = rnd_mass(rnd_gen);
	}
	else {
		mass = mass_;
	}
}

particle::particle(sf::Shape* shape_, std::mt19937& rnd_gen, float min_vel, float max_vel, bool random_mass) : shape(shape_)
{
	if (min_vel > max_vel) {
		throw std::invalid_argument("Minimum velocity cannot be higher than maximum velocity");
	}
	if (min_vel > drawing_engine::screen_weight / 2.f || min_vel > drawing_engine::screen_height / 2.f) {
		throw std::invalid_argument("Speed is too high");
	}
	if (max_vel > drawing_engine::screen_weight / 2.f || max_vel > drawing_engine::screen_height / 2.f) {
		throw std::invalid_argument("Speed is too high");
	}

	const std::uniform_real_distribution<float> rnd_speed(min_vel, max_vel);
	speed = sf::Vector2f(rnd_speed(rnd_gen), rnd_speed(rnd_gen));
	
	const std::uniform_int_distribution<std::mt19937::result_type> rnd_direction(0, 1);
	if (rnd_direction(rnd_gen) == 0) {
		speed *= 1;
	}
	else {
		speed *= -1;
	}

	shape->setFillColor(sf::Color(100, 250, 50));
	if (random_mass) {
		const std::uniform_real_distribution<float> rnd_mass(15.f, 100.f);
		mass = rnd_mass(rnd_gen);
	}
	else {
		mass = 15;
	}
}

particle::particle(sf::Shape* shape_, std::mt19937& rnd_gen, bool random_mass) : shape(shape_)
{
	shape->setFillColor(sf::Color(100, 250, 50));
	const std::uniform_real_distribution<float> rnd_speed(50.f, 145.f);
	
	speed = sf::Vector2f(rnd_speed(rnd_gen), rnd_speed(rnd_gen));

	const std::uniform_int_distribution<std::mt19937::result_type> rnd_direction(0, 1);
	if (rnd_direction(rnd_gen) == 0) {
		speed *= 1;
	}
	else {
		speed *= -1;
	}
	if (random_mass) {
		const std::uniform_real_distribution<float> rnd_mass(15.f, 100.f);
		mass = rnd_mass(rnd_gen);
	}
	else {
		mass = 15;
	}
}
