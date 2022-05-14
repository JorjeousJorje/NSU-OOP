#pragma once
#include "event.hpp"


class vertical_wall_collision final : public event
{
public:
	explicit vertical_wall_collision(particle& particle, const float event_time) :
		event{ event_time}, particle_{ particle }, init_collisions_{ particle.collisions() } {}

	[[nodiscard]] bool is_valid() const override {
		return particle_.collisions() == init_collisions_;
	}
	[[nodiscard]] particle& event_particle() const {
		return particle_;
	}

	void process(event_mediator& mediator) const override;

private:
	particle& particle_;
	std::size_t init_collisions_;
};
