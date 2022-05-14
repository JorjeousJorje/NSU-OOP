#include "TopologicalSort.hpp"
#include <stack>
#include <algorithm>
#include <functional>
#include <iterator>

namespace
{
	typedef std::unordered_map<vertex, colors> visited;
	
	std::pair<vertex, colors> add_color(const vertex& edge)
	{
		return std::make_pair(edge, colors::WHITE);
	}
	
	std::pair<vertex, colors> add_vertexes(const std::pair<vertex, edges>& pair, std::insert_iterator<visited>& inserter)
	{
		auto& [adj_vertex, edges] = pair;
		
		std::transform(edges.begin(), edges.end(), inserter, add_color);
		return std::make_pair(adj_vertex, colors::WHITE);
	}

	void dfs(const vertex& node, const graph& g, std::vector<vertex>& topological_order, visited& visited_vertexes)
	{
		std::stack<vertex> stack;
		auto adj_list = g.get_adj_list();
		stack.push(node);

		while (!stack.empty()) {
			auto& string = stack.top();
			const vertex& v = static_cast<vertex>(string);
			stack.pop();
			
			if(visited_vertexes[v] == colors::BLACK) {
				continue;
			}
			if (visited_vertexes[v] == colors::GRAY) {
				topological_order.clear();
				throw std::bad_function_call();
			}
			
			visited_vertexes[v] = colors::GRAY;
			auto edges = g.get_edges(v);

			for (const auto& edge : edges) {
				const auto& neighbor_id = edge;
				stack.push(neighbor_id);
			}
			
			if(edges.empty()) {
				visited_vertexes[v] = colors::BLACK;
			}

			topological_order.emplace_back(v);
		}
	}
}
std::vector<vertex> topological_sort::sort(const graph& graph)
{
	std::vector<vertex> topological_order;
	visited visited_vertexes;
	auto adj_list = graph.get_adj_list();

	std::transform(adj_list.begin(), adj_list.end(), std::inserter(visited_vertexes, visited_vertexes.begin()),
		std::inserter(visited_vertexes, visited_vertexes.begin()), add_vertexes);
	
	for (auto& [node, edges] : adj_list) {

		auto& [ver, color] = *visited_vertexes.find(node);
		if (color == colors::WHITE) {
			dfs(node, graph, topological_order, visited_vertexes);
		}
	}
	std::reverse(topological_order.begin(), topological_order.end());

	return topological_order;
}
		
	