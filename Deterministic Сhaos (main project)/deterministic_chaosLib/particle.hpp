#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <random>
#include "vector2d.hpp"


class particle
{
public:
	particle() = delete;

	size_t& get_collisions() { return collisions; }
	const sf::Vector2f& get_coordinate() const { return shape->getPosition(); }
	vector2d& get_speed() { return speed; }
	const float& get_mass()              const { return mass; }


	virtual sf::Shape* get_shape() const = 0;

	virtual void move(float dt) = 0;
	virtual void draw(sf::RenderWindow& window) const = 0;

	virtual float time_to_hit(particle* par) = 0;
	virtual float time_to_hit_vertical_wall() = 0;
	virtual float time_to_hit_horizontal_wall() = 0;
	virtual void particles_collision(particle* par) = 0;
	virtual void horizontal_wall_collision() = 0;
	virtual void vertical_wall_collision() = 0;

	virtual ~particle() = default;

protected:
	particle(sf::Shape* shape_, std::mt19937& rnd_gen, float vx, float vy, float mass_, bool random_mass);
	particle(sf::Shape* shape_, std::mt19937& rnd_gen, float min_vel, float max_vel, bool random_mass);
	particle(sf::Shape* shape_, std::mt19937& rnd_gen, bool random_mass);

	sf::Shape* shape;
	size_t collisions{ 0 };
	vector2d speed;
	float mass;
};
