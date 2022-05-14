#include "budget_tree.hpp"

#include <functional>

budget_tree::node::node(double money_, const date& date_)
{
    history.emplace_back(std::make_pair(date_, money_));
}

const std::vector<std::pair<date, double>>& budget_tree::node::get_history() const
{
    return history;
}

void budget_tree::node::update_history(const double money_, const date& date_)
{
    history.emplace_back(std::make_pair(date_, money_));
}

budget_tree::budget_tree()
{
    trie_.emplace_back(node{});
}

void budget_tree::process_row(const std::string& root, const std::string& node, const date& date_, double money)
{
    insert_root(root, date_);
    insert_node(root, node, date_, money);
}

void budget_tree::insert_root(const std::string& root, const date& date_)
{
    const size_t root_id = 0;
    const auto next_node = trie_[root_id].next.find(root);
    if (next_node == trie_[root_id].next.end()) {
        const size_t new_node_id = trie_.size();
        trie_[root_id].next[root] = new_node_id;
        trie_.emplace_back(node{});
    }
}

void budget_tree::insert_node(const std::string& root, const std::string& node_, const date& date_, const double money)
{
    const size_t root_id = 0;
    const auto next_node = trie_[root_id].next.find(root);
    if (next_node == trie_[root_id].next.end()) {
        throw std::bad_function_call();
    }
    const auto cur_node = trie_[next_node->second].next.find(node_);
	if(cur_node != trie_[next_node->second].next.end()) {
        auto& node = trie_[cur_node->second];
        node.update_history(money, date_);
        return;
	}
    const size_t new_node_id = trie_.size();
    trie_[next_node->second].next[node_] = new_node_id;
    trie_.emplace_back(node{ money, date_ });
}

size_t budget_tree::next(const size_t node_id, const std::string& word) const
{
    const auto node = trie_[node_id].next.find(word);
    return trie_[node_id].next.end() == node ? static_cast<size_t>(0) : node->second;
}

const std::vector<budget_tree::node>& budget_tree::get_trie() const
{
    return trie_;
}