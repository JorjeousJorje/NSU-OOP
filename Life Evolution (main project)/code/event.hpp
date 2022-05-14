#pragma once
#include "particle.hpp"
class event_mediator;

class event
{
public:
	explicit event(const float event_time) : event_time_{ event_time }{}

	[[nodiscard]] float event_time() const { return event_time_; }
	[[nodiscard]] virtual bool is_valid() const = 0;
	
	void virtual process(event_mediator& mediator) const = 0;

	virtual ~event() = default;
protected:
	float event_time_;
};

using event_uptr = std::unique_ptr<event>;