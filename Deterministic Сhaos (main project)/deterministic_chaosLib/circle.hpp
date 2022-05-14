#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics.hpp>
#include "particle.hpp"


class circle final : public particle
{
public:
	circle(sf::Shape* shape, std::mt19937& rnd_gen, float x, float y, float vx, float vy, float mass_, float radius, bool random_mass);
	circle(sf::Shape* shape_, std::mt19937& rnd_gen, float min_vel, float max_vel, float min_x, float min_y, float max_x, float max_y, bool rnd_radius, bool random_mass);
	circle(sf::Shape* shape_, std::mt19937& rnd_gen, bool rnd_radius, bool random_mass);

	const float get_radius() const { return circle_->getRadius(); }

	sf::Shape* get_shape() const                                         override;
	void move(float dt)                                                  override;
	void draw(sf::RenderWindow& window) const                            override;
	float time_to_hit(particle* par)                                     override;
	float time_to_hit_vertical_wall()                                    override;
	float time_to_hit_horizontal_wall()                                  override;
	void particles_collision(particle* par)                              override;
	void horizontal_wall_collision()                                     override;
	void vertical_wall_collision()                                       override;

	virtual ~circle() { delete circle_; }

private:
	sf::CircleShape* circle_;
};
