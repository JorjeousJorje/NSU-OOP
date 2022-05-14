#pragma once
#include "event.hpp"

class particles_collision final : public event
{
public:
	explicit particles_collision(particle& left, particle& right, const float event_time) :
		event{ event_time }, left_{ left }, right_{ right },
		init_collisions_left_{ left.collisions() }, init_collisions_right_{ right.collisions() }  {}

	[[nodiscard]] bool is_valid() const override {
		return left_.collisions() == init_collisions_left_ && right_.collisions() == init_collisions_right_;
	}
	[[nodiscard]] std::pair<particle&, particle&> event_particles() const {
		return { left_, right_ };
	}

	void process(event_mediator& mediator) const override;

private:
	particle& left_;
	particle& right_;

	std::size_t init_collisions_left_;
	std::size_t init_collisions_right_;
};
