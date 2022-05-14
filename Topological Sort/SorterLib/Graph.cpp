#include "Graph.hpp"
#include <iterator>

void graph::process_row(const std::vector<std::string>& parsed_vertexes)
{
	const auto adj_list_vertex = parsed_vertexes.begin();
	
	insert_adj_vertex(*adj_list_vertex);
	for (auto edges = ++parsed_vertexes.begin(); edges != parsed_vertexes.end(); ++edges) {
		insert_depend_vertex(*adj_list_vertex, *edges);
	}
	
}

const std::vector<vertex>& graph::get_edges(const vertex& v) const
{
	if (adj_list.find(v) == adj_list.end()) {
		return empty;
	}
	return adj_list.at(v);
}

std::unordered_map<std::string, edges> graph::get_adj_list() const
{
	return adj_list;
}

void graph::insert_adj_vertex(const vertex& v)
{
	adj_list.emplace(v, edges{});
}

void graph::insert_depend_vertex(const vertex& adj_vertex, const vertex& dependent_vertex)
{
	const auto it = adj_list.find(adj_vertex);
	auto& [vertex, edges] = *it;
	if(it == adj_list.end()) {
		return;
	}
	
	edges.emplace_back(dependent_vertex);
}

std::ostream& operator<<(std::ostream& out, const graph& g)
{
	for (auto& pair : g.adj_list)
	{
		auto end_points = pair.second;
		
		out << pair.first << ":";
		for (auto& vertex : end_points)
		{
			out << "-> " << vertex;
		}
		out << std::endl;
	}
	return out;
}

