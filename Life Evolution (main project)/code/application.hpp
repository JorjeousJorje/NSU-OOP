#pragma once
#include "attributes_holder.hpp"
#include "circle_particles_factory.hpp"
#include "circle_physics.hpp"
#include "circle_rnd_param_builder.hpp"
#include "event_manager.hpp"
#include "universe.hpp"


class application
{

public:
	application() = default;

	void start_program(const std::size_t number_of_threads)
	{
		task_dispatcher thread_pool{ number_of_threads };
		timer time{ 0.01 };
		while (window_.is_open()) {
			sf::Event event{};
			while (window_.pool_event(event)) {
				manager_.handle_event(event);
			}

			universe_.simulation_step(thread_pool);
			if (time.is_timer_out()) {
				window_.draw_particles(universe_.universe_particles());
			}
		}
	}

private:
	circle_rnd_param_builder builder_{ 0.0f, 1.0f, 0.0f, 1.0f, 0, 1, radius, radius };
	const circle_particles_factory factory_{};
	circle_physics physics_{ 0.05f };

	normal_force force{};
	universe universe_{ factory_, physics_, builder_, force };
	main_window window_{};
	event_manager manager_{ window_, universe_ };
};
