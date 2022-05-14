#pragma once
#include <SFML/Graphics.hpp>

#include "hsv_converter.hpp"
#include "square_matrix.hpp"
#include "universe_randomizer.hpp"

constexpr float radius = 5.0f;
constexpr float diameter = 2.0f * radius;



class attribute_holder {
	
public:
	explicit attribute_holder(const uint8_t types_num) : types_attractions_{ types_num }, types_min_r_{ types_num }, types_max_r_{ types_num }
	{
        resize_attributes(types_num);
	}

    void resize_attributes(const std::size_t size) {
	    types_colors_.resize(size);
        types_attractions_.resize(size);
        types_max_r_.resize(size);
        types_min_r_.resize(size);
    }

    sf::Color& operator[](const std::size_t index)
    {
        return types_colors_[index];
    }

    sf::Color operator[](const std::size_t index) const
    {
        return types_colors_[index];
    }

    [[nodiscard]] float attraction(const std::size_t i, const std::size_t j) const{
        return types_attractions_[i][j];
    }

	[[nodiscard]] float minr(const std::size_t i, const std::size_t j) const {
        return types_min_r_[i][j];
    }
    [[nodiscard]] float maxr(const std::size_t i, const std::size_t j) const {
        return types_max_r_[i][j];
    }

	void set_random_attributes(universe_randomizer& randomizer)
    {
	    for(auto i = 0u; i < types_colors_.size(); ++i) {
            types_colors_[i] = hsv_converter::convert(static_cast<float>(i) / static_cast<float>(types_colors_.size()), 1.0f, static_cast<float>(i % 2) * 0.5f + 0.5f);

	    	for(auto j = 0u; j < types_colors_.size(); ++j) {
	    		if(i == j) {
                    types_attractions_[i][j] = -std::abs(randomizer(rnd_type::attraction));
                    types_min_r_[i][j] = diameter; //TODO: global variable
	    		} else {
                    types_attractions_[i][j] = randomizer(rnd_type::attraction);
                    types_min_r_[i][j] = std::max(randomizer(rnd_type::minr), diameter); //TODO: global variable
	    		}

                types_max_r_[i][j] = std::max(randomizer(rnd_type::maxr) , types_min_r_[i][j]);
	    		
                types_max_r_[j][i] = types_max_r_[i][j];
                types_min_r_[j][i] = types_min_r_[i][j];
	    	}
	    }
    }
	

private:
    square_matrix types_attractions_;
    square_matrix types_min_r_;
    square_matrix types_max_r_;
    std::vector<sf::Color> types_colors_;
};