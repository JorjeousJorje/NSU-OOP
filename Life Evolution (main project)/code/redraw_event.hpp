#pragma once
#include "event.hpp"

class redraw_event final : public event
{
public:
	explicit redraw_event(const float event_time) : event{ event_time} {}

	[[nodiscard]] bool is_valid() const override {
		return true;
	}

	void process(event_mediator& mediator) const override;
};
