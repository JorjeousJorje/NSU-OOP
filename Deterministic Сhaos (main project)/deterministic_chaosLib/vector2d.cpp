#include "vector2d.hpp"
#include <cmath>
constexpr float PI = 3.141592f;

float vector2d::dot(vector2d& vec) const
{
	return vector.x * vec.x() + vector.y * vec.y();
}

void vector2d::rotate(float degrees)
{
	const auto& prev_x = vector.x;
	vector.x = vector.x * cos(degrees * PI / 180) - vector.y * sin(degrees * PI / 180);
	vector.y = prev_x * sin(degrees * PI / 180) + vector.y * cos(degrees * PI / 180);
}

float vector2d::module() const
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

float vector2d::squared_module() const
{
	return vector.x * vector.x + vector.y * vector.y;
}

float vector2d::angle(vector2d& vec) const
{
	const auto& radians = static_cast<float>(acos(this->dot(vec) / (this->module() * vec.module())));
	return radians * 180 / PI;
}

