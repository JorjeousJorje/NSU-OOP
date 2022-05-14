#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
class vec2f final : public sf::Vector2f
{
public:
	vec2f() : sf::Vector2f(0.f, 0.f) {}
	vec2f(const float x_, const float y_) : sf::Vector2f(x_, y_) {}
	explicit vec2f(const sf::Vector2f& another) : sf::Vector2f(another) {}


	[[nodiscard]] float dot(const vec2f another) const {
		return x * another.x + y * another.y;
	}

	[[nodiscard]] float squared_module() const {
		return x * x + y * y;
	}
	
	[[nodiscard]] float module() const {
		return std::sqrt(x * x + y * y);
	}

	vec2f  operator +  (const vec2f& another) const { return { x + another.x, y + another.y }; }
	vec2f  operator +  (const float num) const { return { x + num, y + num }; }
	vec2f  operator -  (const vec2f& another) const { return { x - another.x, y - another.y }; }
	vec2f  operator -  (const float num) const { return { x - num, y - num }; }
	vec2f  operator *  (const float num) const { return { x * num, y * num }; }
	vec2f  operator /  (const float num) const { return { x / num, y / num }; }
	vec2f& operator += (const vec2f& another) { x += another.x; y += another.y; return *this; }
	vec2f& operator -= (const vec2f& another) { x -= another.x; y -= another.y; return *this; }
	vec2f& operator *= (const float num) { x *= num; y *= num; return *this; }
	vec2f& operator /= (const float num) { x /= num; y /= num; return *this; }
};