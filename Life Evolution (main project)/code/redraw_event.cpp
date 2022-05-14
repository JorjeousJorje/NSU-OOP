#include "redraw_event.hpp"
#include "event_mediator.hpp"


void redraw_event::process(event_mediator& mediator) const
{
	mediator.redraw();
}
