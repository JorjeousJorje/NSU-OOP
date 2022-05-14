#pragma once
#include <cstdint>
#include <SFML/Graphics/Color.hpp>


class hsv_converter
{
public:
	hsv_converter() = delete;

	static sf::Color convert(const float h, const float s, const float v)
	{
		const auto i = static_cast<int>(h * 6);
		const auto f = h * 6 - static_cast<float>(i);
		const auto p = v * (1 - s);
		const auto q = v * (1 - f * s);
		const auto t = v * (1 - (1 - f) * s);
		
		float r = 0, g = 0, b = 0;
		switch (i % 6) {
		case 1: 
			r = q;
			g = v;
			b = p;
			break;
		case 2: 
			r = p;
			g = v;
			b = t;
			break;
		case 3: 
			r = p;
			g = q;
			b = v;
			break;
		case 4: 
			r = t;
			g = p;
			b = v;
			break;
		case 5: 
			r = v;
			g = p;
			b = q;
			break;
		default:
			r = v;
			g = t;
			b = p;
			break;
		}
		return sf::Color{ static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255) };
	}
	
};
