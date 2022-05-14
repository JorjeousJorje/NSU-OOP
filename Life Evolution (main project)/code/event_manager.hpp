#pragma once
#include "event_mediator.hpp"
#include "universe.hpp"
#include "timer.hpp"

class event_manager
{
	friend class event_mediator;
public:
	explicit event_manager(main_window& window, universe& universe) : window_{ window }, universe_{ universe } {}


	void handle_event(sf::Event& event) const;
	
protected:
	main_window& window_;
	universe& universe_;
	event_mediator mediator_{ universe_ };
};
