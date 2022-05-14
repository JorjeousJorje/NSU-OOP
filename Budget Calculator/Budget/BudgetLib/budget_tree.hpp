#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "date.hpp"
#include "tree_parcer.hpp"


class budget_tree final : public  parser_client
{
	struct node
	{
		node() = default;
		explicit node(double money_, const date& date_);
		const std::vector <std::pair<date, double>>& get_history() const;
		void update_history(const double money_, const date& date_);
		std::unordered_map<std::string, size_t> next{};
	private:
		std::vector<std::pair<date, double>> history;
	};

public:
	budget_tree();

	void process_row(const std::string& root, const std::string& node, const date& date, double money) override;
	void insert_root(const std::string& root, const date& date);
	void insert_node(const std::string& root, const std::string& node, const date& date, const double money);
	size_t next(size_t node_id, const std::string& word) const;
	~budget_tree() = default;

	const std::vector<node>& get_trie() const;

private:
	std::vector<node> trie_;
};