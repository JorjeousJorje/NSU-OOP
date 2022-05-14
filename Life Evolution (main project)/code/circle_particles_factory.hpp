#pragma once
#include "particles_factory.hpp"
#include "circle_particle.hpp"



class circle_particles_factory final : public particles_factory
{
public:

    [[nodiscard]] std::optional<particle_uptr> create_particle(param_uptr& params) const override {
        auto* circle_params = dynamic_cast<circle_particle_parameters*>(params.release());

        if (circle_params == nullptr) {
            return std::nullopt;
        }

        return std::make_unique<circle_particle>(*circle_params);
	}
};