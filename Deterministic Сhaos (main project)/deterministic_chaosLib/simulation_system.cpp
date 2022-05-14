#include "simulation_system.hpp"
#include <iostream>
#include "circle.hpp"
#include "impact_painter.hpp"


simulation_system::simulation_system(float limit, size_t par_num, bool enable_draw, bool enable_spawn, bool enable_chaos, bool random_mass) :
					enable_drawing_particles(enable_draw),
					enable_spawning_particles(enable_spawn),
					chaos_simulation(enable_chaos),
					time_limit(limit),
					num_of_particles(par_num)
{
	for (size_t i = 0; i < num_of_particles; ++i) {
		auto* circle_ = new circle(new sf::CircleShape, rnd_generator, false, random_mass);
		particles.emplace_back(circle_);
	}
}

simulation_system::simulation_system(float limit, float min_vel, float max_vel, float min_x,
									 float min_y, float max_x, float max_y, bool enable_draw, bool enable_spawn, bool enable_chaos, bool random_mass) :
					enable_drawing_particles(enable_draw),
					enable_spawning_particles(enable_spawn),
					chaos_simulation(enable_chaos),
					time_limit(limit),
					num_of_particles(2000)
{
	for (size_t i = 0; i < num_of_particles; ++i) {
		auto* circle_ = new circle(new sf::CircleShape, rnd_generator, min_vel, max_vel,
			min_x, min_y, max_x, max_y, false, random_mass);
		particles.emplace_back(circle_);
	}
}

simulation_system::simulation_system(float limit, size_t par_num, float x, float y, float vx, float vy,
									 float mass, float radius, bool enable_draw, bool enable_spawn, bool enable_chaos, bool random_mass) :
					enable_drawing_particles(enable_draw),
					enable_spawning_particles(enable_spawn),
					chaos_simulation(enable_chaos),
					time_limit(limit),
					num_of_particles(par_num)
{
	for (size_t i = 0; i < num_of_particles; ++i) {
		auto* circle_ = new circle(new sf::CircleShape, rnd_generator, x, y, vx, vy, mass, radius, random_mass);
		particles.emplace_back(circle_);
	}
}

void simulation_system::spawn_particles(size_t particles_number, float min_vel, float max_vel, float min_x, float min_y, float max_x, float max_y)
{
	num_of_particles += particles_number;
	for (size_t i = 0; i < particles_number; ++i) {
		auto* circle_ = new circle(new sf::CircleShape, rnd_generator, min_vel, max_vel, min_x, min_y, max_x, max_y, false, false);
		circle_->get_shape()->setFillColor(sf::Color::Yellow);
		particles.emplace_back(circle_);
	}
	for (size_t i = 0; i < num_of_particles; ++i) {
		predict(particles[i]);
	}
}

void simulation_system::collisions_processing(const collision_event& event, bool& redraw_line)
{
	if (event.first_particle() && event.second_particle()) {
		event.first_particle()->particles_collision(event.second_particle());
		if (enable_drawing_particles) {
			impact_painter::draw_on_impact(event.first_particle(), rnd_dev, false);
			impact_painter::draw_on_impact(event.second_particle(), rnd_dev, false);
		}
	}
	else if (event.first_particle() && !event.second_particle()) {
		event.first_particle()->vertical_wall_collision();
		if (enable_drawing_particles) {
			impact_painter::draw_on_impact(event.first_particle(), rnd_dev, false);
		}
	}
	else if (!event.first_particle() && event.second_particle()) {
		event.second_particle()->horizontal_wall_collision();
		if (enable_drawing_particles) {
			impact_painter::draw_on_impact(event.second_particle(), rnd_dev, false);
		}
	}
	else if (!event.first_particle() && !event.second_particle()) {
		engine.draw_particles(particles, num_of_particles);
		if (chaos_simulation) {
			engine.draw_trace(particles, trace, redraw_line);
		}
		engine.get_render_window().display();

		if (last_event_time < time_limit) {
			queue.push(collision_event(nullptr, nullptr, last_event_time + 1 / drawing_engine::frequency));
		}
	}
	predict(event.first_particle());
	predict(event.second_particle());
}

simulation_system::~simulation_system()
{
	for (size_t i = 0; i < num_of_particles; ++i) {
		delete particles[i];
	}
}

void simulation_system::predict(particle* par)
{
	if (!par) {
		return;
	}

	const auto& dt_vertical_wall = par->time_to_hit_vertical_wall();
	const auto& dt_horizontal_wall = par->time_to_hit_horizontal_wall();

	if (dt_vertical_wall < dt_horizontal_wall) {
		queue.push(collision_event(par, nullptr, last_event_time + dt_vertical_wall));
	}
	else {
		queue.push(collision_event(nullptr, par, last_event_time + dt_horizontal_wall));
	}
	const auto& min_time = std::min(dt_vertical_wall, dt_horizontal_wall);

	for (size_t i = 0; i < num_of_particles; ++i) {
		const auto& dt = par->time_to_hit(particles[i]);
		if (last_event_time + dt <= time_limit && dt < min_time) {
			queue.push(collision_event(par, particles[i], last_event_time + dt));
		}
	}
}

void simulation_system::simulate()
{
	for (size_t i = 0; i < num_of_particles; ++i) {
		predict(particles[i]);
	}
	sf::Event window_event{};
	auto redraw_line = false;

	queue.push(collision_event(nullptr, nullptr, 0));

	while (!queue.empty() && engine.get_render_window().isOpen() && clock_.getElapsedTime().asSeconds() < time_limit) {

		engine.get_render_window().pollEvent(window_event);
		if (window_event.type == sf::Event::KeyPressed && window_event.key.code == sf::Keyboard::Escape) {
			engine.get_render_window().close();
		}
		
		if (enable_spawning_particles) {
			if (window_event.type == sf::Event::KeyPressed && window_event.key.code == sf::Keyboard::W) {
				const auto& mouse_vector = sf::Mouse::getPosition(engine.get_render_window());
				spawn_particles(100, 4, 6, 0, 0, static_cast<float>(mouse_vector.x), static_cast<float>(mouse_vector.y));
				continue;
			}
		}

		if (chaos_simulation) {
			if (window_event.type == sf::Event::KeyPressed && window_event.key.code == sf::Keyboard::Q) {
				while (!queue.empty()) {
					queue.pop();
				}
				for (auto& particle : particles) {
					particle->get_speed() *= -1;
				}
				last_event_time = 0;
				for (auto& particle : particles) {
					predict(particle);
				}
				queue.push(collision_event(nullptr, nullptr, 0));
				if (!redraw_line) {
					redraw_line = true;
				}
				else {
					redraw_line = false;
				}
				continue;
			}
		}

		auto event = queue.top();
		queue.pop();

		if (!event.is_valid()) {
			continue;
		}

		for (auto& particle : particles) {
			particle->move(event.get_event_time() - last_event_time);
		}

		last_event_time = event.get_event_time();

		collisions_processing(event, redraw_line);
	}
}
