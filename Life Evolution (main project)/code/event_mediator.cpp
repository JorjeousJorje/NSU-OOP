#include "event_mediator.hpp"
#include "event_manager.hpp"

void event_mediator::balance() const
{
	universe_.set_population(9, 400);
	universe_randomizer rnd{ -0.02f, 0.06f, 0.0f, 20.0f, 20.0f, 70.0f };
	normal_force force{};
	universe_.reseed(rnd, 0.05f, force);
}

void event_mediator::diversity() const
{
	universe_.set_population(12, 400);
	universe_randomizer rnd{ -0.01f, 0.04f, 0.0f, 20.0f, 10.0f, 60.0f };
	flat_force force{};
	universe_.reseed(rnd, 0.05f, force);
}

void event_mediator::gliders() const
{
	universe_.set_population(6, 400);
	universe_randomizer rnd{ 0.0f, 0.06f, 0.0f, 20.0f, 10.0f, 50.0f };
	flat_force force{};
	universe_.reseed(rnd, 0.1f, force);
}

void event_mediator::homogeneity() const
{
	universe_.set_population(4, 400);
	universe_randomizer rnd{ 0.0f, 0.04f, 10.0f, 10.0f, 10.0f, 80.0f };
	flat_force force{};
	universe_.reseed(rnd, 0.05f, force);
}

void event_mediator::large_clusters() const
{
	universe_.set_population(6, 400);
	universe_randomizer rnd{ 0.025f, 0.02f, 0.0f, 30.0f, 30.0f, 100.0f };
	normal_force force{};
	universe_.reseed(rnd, 0.2f, force);
}

void event_mediator::medium_clusters() const
{
	universe_.set_population(6, 400);
	universe_randomizer rnd{ 0.02f, 0.05f, 0.0f, 20.0f, 20.0f, 50.0f };
	normal_force force{};
	universe_.reseed(rnd, 0.05f, force);
}

void event_mediator::quiescence() const
{
	universe_.set_population(6, 300);
	universe_randomizer rnd{ -0.02f, 0.1f, 10.0f, 20.0f, 20.0f, 60.0f };
	normal_force force{};
	universe_.reseed(rnd, 0.2f, force);
}

void event_mediator::small_clusters() const
{
	universe_.set_population(6, 600);
	universe_randomizer rnd{ -0.005f, 0.01f, 10.0f, 10.0f, 20.0f, 50.0f };
	normal_force force{};
	universe_.reseed(rnd, 0.01f, force);
}
