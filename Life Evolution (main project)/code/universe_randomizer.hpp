#pragma once
#include "uniform_randomizer.hpp"
#include "normal_randomizer.hpp"

enum class rnd_type : uint8_t
{
	attraction,
	minr,
	maxr
};

class universe_randomizer
{
public:
	universe_randomizer(const float attract_mean, const float attract_std, 
						const float minr_lower, const float minr_upper, 
						const float maxr_lower, const float maxr_upper)
	:	attraction_random_{attract_mean, attract_std}, minr_random_{ minr_lower ,minr_upper }, maxr_random_{ maxr_lower , maxr_upper } {}

	float operator()(const rnd_type type)
	{
		if(type == rnd_type::attraction) {
			return attraction_random_();
		}
		if (type == rnd_type::minr) {
			return minr_random_();
		}
		return maxr_random_();
	}

	
private:
	normal_randomizer<float> attraction_random_;
	uniform_randomizer<float> minr_random_;
	uniform_randomizer<float> maxr_random_;
};