#pragma once
#include <memory>
#include <optional>
#include "particle.hpp"

class particles_factory
{
public:
	[[nodiscard]] virtual std::optional<particle_uptr> create_particle(param_uptr&) const = 0;

	virtual ~particles_factory() = default;
};
