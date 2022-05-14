 #include "budget.hpp"
#include <iomanip>

budget::budget(std::istream& budget, std::istream& expenses) : b_parser(budget), e_parser(expenses)
{
	e_parser.parse(expense_tree);
	budget_ = b_parser.parse();
}

void budget::count_expenses(std::ostream& out, std::istream& date_) const
{
	date start, end;
	date_ >> start;
	date_ >> end;
	if (!date::check_end_date(start, end)) {
		throw std::invalid_argument("wrong date sequence");
	}
	count_expenses(out, start, end);
	
}

void budget::count_expenses(std::ostream& out, const date& start, const date& end) const
{
	std::vector<bool> visited(expense_tree.get_trie().size());

	std::vector<std::pair<std::string, modes>> buffer{};
	double sum = 0;
	double budget_money = 0;
	for (auto it = budget_.begin(); it != budget_.end(); ++it) {
		auto& [root, money, mode] = *it;
		budget_money = money;
		if (mode == modes::PLUS) {
			get_root_money(root, sum, visited, start, end);
			buffer.emplace_back(std::make_pair(root, mode));
			continue;
		}
		if (mode == modes::COLON) {
			auto next = std::next(it, 1);
			buffer.emplace_back(std::make_pair(root, mode));
			auto [node, money, node_mode] = *next;
			buffer.emplace_back(std::make_pair(node, node_mode));
			get_node_money(root, node, sum, visited, start, end);
			++it;
			if (node_mode == modes::EMPTY) {
				print_result(buffer, std::make_pair(budget_money, sum), out);
				buffer.clear();
				sum = 0;
				continue;
			}
		}
		if (mode == modes::EMPTY) {
			get_root_money(root, sum, visited, start, end);
			buffer.emplace_back(std::make_pair(root, mode));
			print_result(buffer, std::make_pair(budget_money, sum), out);
			sum = 0;
			buffer.clear();
		}
	}
	buffer.clear();
	get_rest_of_money(visited, sum, start, end);
	print_result(buffer, std::make_pair(0, sum), out);
}

void budget::print_result(std::vector<std::pair<std::string, modes>>& articles, const std::pair<double, double>& money, std::ostream& out) const
{
	const size_t table_width = 40;
	const size_t num_size = 12;
	const size_t current_stream_pos = static_cast<size_t>(out.tellp());
	const auto& [budget_money, sum] = money;
	if (articles.empty()) {
		out << "<Other expenses>" << std::setw(table_width - 16) << std::setprecision(2) << std::fixed << budget_money << std::setw(num_size)
			<< sum << std::endl;
		return;
	}
	if (articles.size() == 1) {
		out << articles[0].first << std::setw(table_width - articles[0].first.size()) << std::setprecision(2) << std::fixed << budget_money << std::setw(num_size)
			<< sum << std::setw(num_size) << std::setprecision(2) << sum / budget_money * 100 << '%' << std::endl;
		return;
	}
	for (auto it = articles.begin(); it != articles.end(); ++it) {
		auto [article, mode] = *it;
		if (mode == modes::PLUS) {
			out << article + " + ";
			continue;
		}
		if (mode == modes::COLON) {
			auto next = std::next(it, 1);
			auto [next_article, article_mode] = *next;
			article += ':';
			const auto& concat = article + next_article;
			if (article_mode == modes::PLUS) {
				out << concat + " + ";
			}
			if (article_mode == modes::EMPTY) {
				out << concat;
				const size_t written_size = static_cast<size_t>(out.tellp()) - current_stream_pos;
				out << std::setw(table_width - written_size) << std::setprecision(2) << std::fixed << budget_money << std::setw(num_size)
					<< sum << std::setw(num_size) << std::setprecision(2) << sum / budget_money * 100 << '%' << std::endl;
				break;
			}
			++it;
		}
		else if (mode == modes::EMPTY) {
			out << article;
			const size_t written_size = static_cast<size_t>(out.tellp()) - current_stream_pos;
			out << std::setw(table_width - written_size) << std::setprecision(2) << std::fixed << budget_money << std::setw(num_size)
				<< sum << std::setw(num_size) << std::setprecision(2) << sum / budget_money * 100 << '%' << std::endl;
		}
	}
}

void budget::get_root_money(const std::string& root_, double& money, std::vector<bool>& visited, const date& start, const date& end) const
{
	const auto id = expense_tree.next(0, root_);
	if (id) {
		visited[id] = true;
		auto& nodes = expense_tree.get_trie()[id].next;
		auto it = nodes.begin();
		if (it != nodes.end()) {
			while (const auto next_id = expense_tree.next(id, it->first)) {
				auto& node = expense_tree.get_trie()[next_id];
				auto& history = node.get_history();
				for(auto& [date, date_money] : history) {
					if (date::date_cmp(date, start, end)) {
						visited[next_id] = true;
						money += date_money;
					}
				}
				++it;
				if (it == nodes.end()) {
					break;
				}
			}
		}
	}
}

void budget::get_node_money(const std::string& root_, const std::string& node_, double& money, std::vector<bool>& visited, const date& start, const date& end) const
{
	const auto id = expense_tree.next(0, root_);
	if (id) {
		const auto node_id = expense_tree.next(id, node_);
		if (node_id) {
			auto& node = expense_tree.get_trie()[node_id];
			auto& history = node.get_history();
			for (auto& [date, date_money] : history) {
				if (date::date_cmp(date, start, end)) {
					visited[node_id] = true;
					money += date_money;
				}
			}
		}
	}
}

void budget::get_rest_of_money(const std::vector<bool>& visited, double& money, const date& start, const date& end) const
{
	for (size_t i = 0; i < visited.size(); ++i) {
		if (!visited[i]) {
			auto& node = expense_tree.get_trie()[i];
			auto& history = node.get_history();
			for (auto& [date, date_money] : history) {
				if (date::date_cmp(date, start, end)) {
					money += date_money;
				}
			}
		}
	}
}