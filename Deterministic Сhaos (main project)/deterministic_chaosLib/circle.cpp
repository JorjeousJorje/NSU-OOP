#include "circle.hpp"
#include <cmath>
#include <random>
#include "drawing_engine.hpp"

circle::circle(sf::Shape* shape_, std::mt19937& rnd_gen, float x, float y, float vx,
	float vy, float mass_, float radius, bool random_mass) : particle(shape_, rnd_gen, vx, vy, mass_, random_mass)
{
	circle_ = dynamic_cast<sf::CircleShape*>(shape);
	if (x > drawing_engine::screen_weight || x < 0) {
		throw std::invalid_argument("Your coordinate is more than screen width or negative");

	}
	if (y > drawing_engine::screen_height || y < 0) {
		throw std::invalid_argument("Your coordinate is more than screen height or negative");

	}
	if (radius > drawing_engine::screen_height / 2.f) {
		throw std::invalid_argument("Huge radius");
	}

	circle_->setRadius(radius);
	circle_->setOrigin(radius, radius);
	if (x > drawing_engine::screen_weight / 2.f && y > drawing_engine::screen_height / 2.f) {
		circle_->setPosition(x - circle_->getRadius(), y - circle_->getRadius());
	}
	if (x > drawing_engine::screen_weight / 2.f && y < drawing_engine::screen_height / 2.f) {
		circle_->setPosition(x - circle_->getRadius(), y + circle_->getRadius());
	}
	if (x < drawing_engine::screen_weight / 2.f && y < drawing_engine::screen_height / 2.f) {
		circle_->setPosition(x + circle_->getRadius(), y + circle_->getRadius());
	}
	if (x < drawing_engine::screen_weight / 2.f && y > drawing_engine::screen_height / 2.f) {
		circle_->setPosition(x + circle_->getRadius(), y - circle_->getRadius());
	}
}

circle::circle(sf::Shape* shape_, std::mt19937& rnd_gen, float min_vel, float max_vel, float min_x, float min_y,
	float max_x, float max_y, bool rnd_radius, bool random_mass) : particle(shape_, rnd_gen, min_vel, max_vel, random_mass)
{
	if (min_vel > max_vel) {
		throw std::invalid_argument("Minimum velocity cannot be higher than maximum velocity");
	}
	if (min_x > max_x || min_y > max_y) {
		throw std::invalid_argument("Minimum coordinate cannot be higher than maximum coordinate");
	}
	if (max_x > drawing_engine::screen_weight || max_x < 0 || min_x < 0 || min_x > drawing_engine::screen_weight) {
		throw std::invalid_argument("Your coordinate is more than screen width or negative");

	}
	if (max_y > drawing_engine::screen_height || max_y < 0 || min_y < 0 || min_y > drawing_engine::screen_height) {
		throw std::invalid_argument("Your coordinate is more than screen height or negative");

	}

	circle_ = dynamic_cast<sf::CircleShape*>(shape);
	if (rnd_radius) {
		const std::uniform_real_distribution<float> rnd_rad(2.f, 4.f);
		circle_->setRadius(rnd_rad(rnd_gen));
	}
	else {
		circle_->setRadius(1);
	}
	circle_->setOrigin(circle_->getRadius(), circle_->getRadius());

	const std::uniform_real_distribution<float> rnd_coordinate_x(min_x + circle_->getRadius(), max_x - circle_->getRadius());
	const std::uniform_real_distribution<float> rnd_coordinate_y(min_y + circle_->getRadius(), max_y - circle_->getRadius());
	circle_->setPosition(rnd_coordinate_x(rnd_gen), rnd_coordinate_y(rnd_gen));
}

circle::circle(sf::Shape* shape_, std::mt19937& rnd_gen, bool rnd_radius, bool random_mass) : particle(shape_, rnd_gen, random_mass)
{
	circle_ = dynamic_cast<sf::CircleShape*>(shape);

	if (rnd_radius) {
		const std::uniform_real_distribution<float> rnd_rad(10.f, 15.f);
		circle_->setRadius(rnd_rad(rnd_gen));
	}
	else {
		circle_->setRadius(1);
	}

	circle_->setOrigin(circle_->getRadius(), circle_->getRadius());

	const std::uniform_real_distribution<float> rnd_coordinate_x(circle_->getRadius(), drawing_engine::screen_weight - circle_->getRadius());
	const std::uniform_real_distribution<float> rnd_coordinate_y(circle_->getRadius(), drawing_engine::screen_height - circle_->getRadius());
	shape->setPosition(rnd_coordinate_x(rnd_gen), rnd_coordinate_y(rnd_gen));
}

void circle::draw(sf::RenderWindow& window) const
{
	window.draw(*circle_);
}

sf::Shape* circle::get_shape() const
{
	return circle_;
}

void circle::move(float dt)
{
	circle_->move(speed.get_sf_vector() * dt);
}

float circle::time_to_hit(particle* par)
{
	if (par->get_shape() == circle_) {
		return INFINITY;
	}

	const auto converted_circle = dynamic_cast<circle*>(par);

	auto dr = vector2d(par->get_coordinate() - circle_->getPosition());
	auto dv = par->get_speed() - speed;
	const auto& dvdr = dv.dot(dr);

	if (dvdr >= 0) {
		return INFINITY;
	}

	const auto& dvdv = dv.squared_module();
	if (dvdv == 0) {
		return INFINITY;
	}

	const auto& sigma = circle_->getRadius() + converted_circle->get_radius();

	const auto& d = (dvdr * dvdr) - dvdv * (dr.squared_module() - sigma * sigma);
	if (d < 0) {
		return INFINITY;
	}

	return -(dvdr + std::sqrt(d)) / dvdv;
}

float circle::time_to_hit_vertical_wall()
{
	if (speed.x() > 0) {
		return (drawing_engine::screen_weight - circle_->getPosition().x - circle_->getRadius()) / speed.x();
	}
	if (speed.x() < 0) {
		return (circle_->getRadius() - circle_->getPosition().x) / speed.x();
	}

	return INFINITY;

}

float circle::time_to_hit_horizontal_wall()
{
	if (speed.y() > 0) {
		return (drawing_engine::screen_height - circle_->getPosition().y - circle_->getRadius()) / speed.y();
	}
	if (speed.y() < 0) {
		return (circle_->getRadius() - circle_->getPosition().y) / speed.y();
	}

	return INFINITY;
}

void circle::particles_collision(particle* par)
{
	const auto converted_circle = dynamic_cast<circle*>(par);

	auto dr = vector2d(par->get_coordinate() - circle_->getPosition());
	auto dv = par->get_speed() - speed;

	const auto& distance = circle_->getRadius() + converted_circle->get_radius();
	const auto& dvdr = dv.dot(dr);

	const auto& magnitude = 2 * par->get_mass() * mass * dvdr / ((par->get_mass() + mass) * distance);

	par->get_speed().x() -= magnitude * dr.x() / (distance * par->get_mass());
	par->get_speed().y() -= magnitude * dr.y() / (distance * par->get_mass());

	speed.x() += magnitude * dr.x() / (distance * mass);
	speed.y() += magnitude * dr.y() / (distance * mass);

	++collisions;
	++par->get_collisions();
}

void circle::horizontal_wall_collision()
{
	speed.y() *= -1;
	++collisions;
}

void circle::vertical_wall_collision()
{
	speed.x() *= -1;
	++collisions;
}
