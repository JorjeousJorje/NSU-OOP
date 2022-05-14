#include "event_manager.hpp"

void event_manager::handle_event(sf::Event& event) const
{
	if (event.type == sf::Event::Closed) {
		window_.close();
	}
	else if (event.type == sf::Event::KeyPressed) {
		const auto keycode = event.key.code;
		if (keycode == sf::Keyboard::B) {
			mediator_.balance();
		}
		else if (keycode == sf::Keyboard::D) {
			mediator_.diversity();
		}
		else if (keycode == sf::Keyboard::H) {
			mediator_.homogeneity();
		}
		else if (keycode == sf::Keyboard::G) {
			mediator_.gliders();
		}
		else if (keycode == sf::Keyboard::L) {
			mediator_.large_clusters();
		}
		else if (keycode == sf::Keyboard::M) {
			mediator_.medium_clusters();
		}
		else if (keycode == sf::Keyboard::Q) {
			mediator_.quiescence();
		}
		else if (keycode == sf::Keyboard::S) {
			mediator_.small_clusters();
		}
	}
}
