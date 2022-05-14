#pragma once
#include <queue>
#include <vector>
#include "collision_event.hpp"
#include "drawing_engine.hpp"


class simulation_system final
{
	class event_comparator
	{
	public:
		bool operator() (const collision_event& event1, const collision_event& event2) const
		{
			return event1.get_event_time() > event2.get_event_time();
		}
	};
public:
	simulation_system(float limit, size_t par_num, bool enable_draw, bool enable_spawn, bool enable_chaos, bool random_mass);

	simulation_system(float limit, float min_vel, float max_vel, float min_x,
		float min_y, float max_x, float max_y, bool enable_draw, bool enable_spawn, bool enable_chaos, bool random_mass);

	simulation_system(float limit, size_t par_num, float x, float y, float vx, float vy,
		float mass, float radius, bool enable_draw, bool enable_spawn, bool enable_chaos, bool random_mass);

	void simulate();
	~simulation_system();

private:

	void predict(particle* par);
	void spawn_particles(size_t particles_number, float min_vel, float max_vel, float min_x, float min_y, float max_x, float max_y);
	void collisions_processing(const collision_event& event, bool& redraw_line);

	bool enable_drawing_particles;
	bool enable_spawning_particles;
	bool chaos_simulation;
	drawing_engine engine{};

	float last_event_time{ 0 };
	float time_limit;
	sf::Clock clock_;

	std::random_device rnd_dev{};
	std::mt19937 rnd_generator{ rnd_dev() };

	std::priority_queue<collision_event, std::vector<collision_event>, event_comparator> queue{};
	std::vector<particle*> particles{};
	std::vector<sf::Vertex> trace{};

	size_t num_of_particles;

};
