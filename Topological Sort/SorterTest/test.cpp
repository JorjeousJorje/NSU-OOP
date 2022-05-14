#include "pch.h"
#include "TopologicalSort.hpp"
#include <fstream>
#include <algorithm>
#include <functional>
#include <iostream>

TEST(cyclicgraph, graphtest) {
	graph graph;
	std::ifstream in("test_cyclicgraph.txt");
	std::vector<vertex> topo_sorted;

	graph_parser p(in);
	p.parse(graph);
	try {
		topo_sorted = topological_sort::sort(graph);
	}
	catch (std::bad_function_call& ex) {
		assert_true(topo_sorted.empty());
		std::cout << "your graph is cyclic." << std::endl;
	}
}
TEST(back_edge_ver_cyclicgraph, graphtest) {
	graph graph;
	std::ifstream in("one_branch_test_cyclicgraph.txt");

	graph_parser p(in);
	p.parse(graph);
	size_t size = graph.get_adj_list().size();
	//assert_eq(size, 5);
	std::vector<std::string> sorted = topological_sort::sort(graph);
	assert_true(sorted.empty());
}
TEST(emptylist_of_dependencies, graphtest) {
	graph graph;
	std::ifstream in("empty_dependencies.txt");

	graph_parser p(in);
	p.parse(graph);
	auto adj_list = graph.get_adj_list();
	auto bucket_size = adj_list.bucket_count();
	for (auto it = 0; it < bucket_size; ++it) {
		assert_le(adj_list.bucket_size(it), 2);
	}
	std::vector<std::string> sorted = topological_sort::sort(graph);
}
TEST(undependent_dependancies, graphtest) {
	graph graph;
	std::ifstream in("undependent_dependancies.txt");
	std::vector<std::string> adj_vertexes;

	graph_parser p(in);
	p.parse(graph);
	auto adj_list = graph.get_adj_list();
	for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
		adj_vertexes.emplace_back(it->first);
	}
	size_t num_of_vers = graph.vertex_count();
	std::vector<std::string> sorted = topological_sort::sort(graph);
	for (auto itt = adj_vertexes.begin(); itt != adj_vertexes.end(); ++itt) {
		auto pos = distance(sorted.begin(), std::find(sorted.begin(), sorted.end(), *itt));
		assert_ge(pos, num_of_vers - adj_vertexes.size());

	}
}
TEST(commongraph, graphtest) {
	graph graph;
	std::ifstream in("common_case.txt");
	std::vector<std::string> adj_vertexes;

	graph_parser p(in);
	p.parse(graph);
	auto adj_list = graph.get_adj_list();
	for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
		adj_vertexes.emplace_back(it->first);
	}
	size_t num_of_vers = graph.vertex_count();
	std::vector<std::string> sorted = topological_sort::sort(graph);
	for (auto itt = adj_vertexes.begin(); itt != adj_vertexes.end(); ++itt) {
		auto pos = distance(sorted.begin(), std::find(sorted.begin(), sorted.end(), *itt));
		assert_ge(pos, num_of_vers - adj_vertexes.size());

	}
}

TEST(noncyclicgraph, empty) {
    const graph g;
    std::vector<vertex> topo_sorted;
    topo_sorted = topological_sort::sort(g);
    expect_true(topo_sorted.empty());
}

TEST(noncyclicgraph, onevertex) {
    graph g;
    std::vector<vertex> topo_sorted;
    const vertex ver = "vertex";
    g.insert_vertex(ver);
    topo_sorted = topological_sort::sort(g);
    expect_eq(topo_sorted.front(), "vertex");
    topo_sorted.pop_back();
    expect_true(topo_sorted.empty());
}

TEST(noncyclicgraph, twovertices) {
    graph g;
    std::vector<vertex> topo_sorted;
    const vertex ver1 = "first", ver2 = "second";
    g.insert_vertex(ver1);
    g.insert_vertex(ver2);
    topo_sorted = topological_sort::sort(g);
    expect_eq(topo_sorted.back(), "second");
    topo_sorted.pop_back();
    expect_eq(topo_sorted.back(), "first");
    topo_sorted.pop_back();
    expect_true(topo_sorted.empty());
}

TEST(noncyclicgraph, simpletest) {
    graph g;
    std::vector<vertex> topo_sorted;
	
    g.insert_vertex("1");
    g.insert_edge("1", "4");
    g.insert_edge("1", "3");

    g.insert_vertex("2");
    g.insert_edge("2", "3");
    g.insert_edge("2", "4");

    topo_sorted = topological_sort::sort(g);
    expect_eq(topo_sorted.back(), "2");
    topo_sorted.pop_back();
    expect_eq(topo_sorted.back(), "1");
    topo_sorted.pop_back();
    expect_eq(topo_sorted.back(), "3");
    topo_sorted.pop_back();
    expect_eq(topo_sorted.back(), "4");
    topo_sorted.pop_back();
    expect_true(topo_sorted.empty());
}

TEST(cyclicgraph, simpletest) {
    graph g;
    std::vector<vertex> topologically_sorted;

    g.insert_vertex("4");
    g.insert_edge("4", "1");

    g.insert_vertex("3");
    g.insert_edge("3", "4");

    g.insert_vertex("2");
    g.insert_edge("2", "3");
    g.insert_edge("2", "4");

    g.insert_vertex("1");
    g.insert_edge("1", "2");
	try {
		topologically_sorted = topological_sort::sort(g);
	}
	catch (std::bad_function_call& ex) {
		assert_true(topo_sorted.empty());
		std::cout << "your graph is cyclic." << std::endl;
	}
}