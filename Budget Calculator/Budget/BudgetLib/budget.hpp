#pragma once
#include "budget_tree.hpp"
#include <vector>
#include "budget_parser.hpp"

class budget final
{
public:
	budget(std::istream& budget, std::istream& expenses);
	void count_expenses(std::ostream& out, std::istream& date_) const;
	void count_expenses(std::ostream& out, const date& start, const date& end) const;

private:
	void print_result(std::vector<std::pair<std::string, modes>>& articles, const std::pair<double, double>& money, std::ostream& out) const;
	void get_root_money(const std::string& root_, double& money, std::vector<bool>& visited, const date& start, const date& end) const;
	void get_node_money(const std::string& root, const std::string& node, double& money, std::vector<bool>& visited, const date& start, const date& end) const;
	void get_rest_of_money(const std::vector<bool>& visited, double& money, const date& start, const date& end) const;

	budget_tree expense_tree{};
	budget_parser b_parser;
	expenses_parser e_parser;
	std::vector<std::tuple<std::string, double, modes>> budget_{};
};
