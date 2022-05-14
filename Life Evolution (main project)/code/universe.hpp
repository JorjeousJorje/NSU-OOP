#pragma once
#include <memory>
#include <queue>
#include "particles_factory.hpp"
#include "particle_physics.hpp"
#include "rnd_param_builder.hpp"
#include "task_dispather.hpp"

class universe
{
public:
	universe(const particles_factory& factory, particle_physics& physics, rnd_param_builder& builder, physical_force& force)
		: physics_{ physics }, factory_{ factory }, builder_{ builder }, current_force_{ force } {}
	
	void simulation_step(task_dispatcher& thread_pool)
	{
		for (auto& current_particle : particles_) {

			thread_pool.insert_task([&] {
				for (auto& other_particle : particles_) {
					physics_.attraction(*current_particle, *other_particle, holder_, current_force_);
				}});


		}

		thread_pool.wait_finished();

		thread_pool.insert_task([&] {for (auto& particle : particles_) {
			particle->move(physics_.system_friction());

			if(physics_.horizontal_wall_collision(*particle)) {
				continue;
			}
			physics_.vertical_wall_collision(*particle);
		}});

		thread_pool.wait_finished();
	}



	void reseed(universe_randomizer& randomizer, float friction, physical_force& force);
	void set_population(uint8_t types, std::size_t particle_num);

	[[nodiscard]] const std::vector<particle_uptr>& universe_particles() const;
	
private:
	particle_physics& physics_;
	const particles_factory& factory_;
	rnd_param_builder& builder_;
	attribute_holder holder_{ 0 };
	physical_force& current_force_;
	std::vector<particle_uptr> particles_{};
	std::size_t particle_num_{ 0 };


	void born_random_particles()
	{
		particles_.clear();
		for(auto i = 0u; i < particle_num_; ++i) {
			auto param = builder_.construct(i, screen_width, screen_height);
			auto option = factory_.create_particle(param);
			if (!option) {
				continue;
			}
			particles_.emplace_back(std::move(option.value()));
			const auto type = particles_.back()->type();
			particles_.back()->set_color(holder_[type]);
		}
	}
};

