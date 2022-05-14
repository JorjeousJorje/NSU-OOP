#include "quick_test.hpp"
#include <functional>
#include "TopologicalSort.hpp"
#include <iostream>

void start_test(std::istream& input, std::ostream& output)
{
	graph graph;

	
	graph_parser p(input);
	try {
		p.parse(graph);
	}
	catch(const std::bad_exception& ex) {
		std::cout << "Impossible to establish the dependence of the vertices." << std::endl;
	}
	std::vector<vertex> topologically_sorted;
	try {
		topologically_sorted = topological_sort::sort(graph);
	}
	catch (const std::bad_function_call& ex) {
		std::cout << "Your graph is cyclic." << std::endl;
	}
	
	for(auto it = topologically_sorted.begin(); it != topologically_sorted.end(); ++it) {
		output << *it << '\n';
	}
}
