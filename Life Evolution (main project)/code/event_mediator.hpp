#pragma once
class universe;

class event_mediator
{
public:
	explicit event_mediator(universe& universe) : universe_{ universe } {}

	void balance() const;
	void diversity() const;
	void gliders() const;
	void homogeneity() const;
	void large_clusters() const;
	void medium_clusters() const;
	void quiescence() const;
	void small_clusters() const;

private:
	universe& universe_;

};