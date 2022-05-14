#pragma once
#include "particle.hpp"

class impact_painter final
{
public:
	impact_painter() = delete;

	static void draw_on_impact(particle* par, std::random_device& rnd_dev, bool pure_random);
};

