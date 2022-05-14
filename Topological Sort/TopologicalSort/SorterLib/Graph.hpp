#pragma once
#include <unordered_map>
#include "Parser.hpp"

typedef std::string vertex;

typedef std::vector<vertex> edges;

class graph final : public parser_client
{
public:
	void process_row(const std::vector<std::string>& parsed_vertexes) override;
	graph() = default;
	
	void insert_adj_vertex(const vertex& v);
	void insert_depend_vertex(const vertex& adj_vertex, const vertex& dependent_vertex);
	
	friend std::ostream& operator<<(std::ostream& out, const graph& g);
	
	const std::vector<vertex>& get_edges(const vertex& v) const;
	std::unordered_map<vertex, edges> get_adj_list() const;
	
private:
	std::unordered_map<vertex, edges> adj_list{};
	static inline edges empty{};
};
