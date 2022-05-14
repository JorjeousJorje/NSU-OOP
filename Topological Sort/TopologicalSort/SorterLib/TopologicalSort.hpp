#pragma once
#include "Graph.hpp"
enum class colors
{
	WHITE,
	GRAY,
	BLACK
};

class topological_sort final
{
public:
	topological_sort() = delete;
	static std::vector<vertex> sort(const graph&);
};