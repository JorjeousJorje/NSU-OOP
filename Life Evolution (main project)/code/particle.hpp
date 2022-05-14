#pragma once
#include <memory>
#include <SFML/Graphics/Shape.hpp>
#include "particle_parameters.hpp"

class particle
{
public:
	particle() = delete;

	explicit particle(particle_parameters& params) : params_{ params } {}


	[[nodiscard]] virtual const sf::Shape& shape() const = 0;
	virtual void set_color(sf::Color color) = 0;
	
	virtual void move(float friction) = 0;

	[[nodiscard]] std::size_t id() const;
	
				  vec2f& speed();
	[[nodiscard]] vec2f speed() const;

				  vec2f& coordinates();
	[[nodiscard]] vec2f coordinates() const;

	[[nodiscard]] uint8_t type() const;

	virtual ~particle() = default;
protected:
	particle_parameters& params_;
	std::size_t collisions_{ 0 };
};


using particle_uptr = std::unique_ptr<particle>;