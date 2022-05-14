#pragma once
#include <SFML/System/Vector2.hpp>

class vector2d final
{
public:
	vector2d() : vector(0, 0) {}
	vector2d(float x, float y) : vector(x, y) {}
	vector2d(const sf::Vector2f& vector_) : vector(vector_) {}

	vector2d normal() const { return vector2d(-vector.y, vector.x); }
	float dot(vector2d& vec) const;
	void rotate(float degrees);
	float module() const;
	float squared_module() const;
	float angle(vector2d& vec) const;


	float& x() { return vector.x; }
	float& y() { return vector.y; }

	vector2d  operator +  (vector2d& vec) const { return vector2d(vector.x + vec.x(), vector.y + vec.y()); }
	vector2d  operator -  (vector2d& vec) const { return vector2d(vector.x - vec.x(), vector.y - vec.y()); }
	vector2d  operator *  (const float num) const { return vector2d(vector.x * num, vector.y * num); }
	vector2d  operator /  (const float num) const { return vector2d(vector.x / num, vector.y / num); }
	vector2d& operator += (vector2d& vec) { vector.x += vec.x(); vector.y += vec.y(); return *this; }
	vector2d& operator -= (vector2d& vec) { vector.x -= vec.x(); vector.y -= vec.y(); return *this; }
	vector2d& operator *= (const float num) { vector.x *= num; vector.y *= num; return *this; }
	vector2d& operator /= (const float num) { vector.x /= num; vector.y /= num; return *this; }


	const sf::Vector2f& get_sf_vector() const { return vector; }
private:
	sf::Vector2f vector;
};


