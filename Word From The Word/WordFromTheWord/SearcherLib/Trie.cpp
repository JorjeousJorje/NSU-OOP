#include "Trie.hpp"
#include <unordered_set>

trie::node::node()
{
    is_complete_word = false;
}

trie::trie()
{
    trie_.emplace_back(node{});
}

void trie::insert(const std::string& word)
{
    size_t node_id = 0;
    for(auto ch : word)
    {
        auto next_node = trie_[node_id].next.find(ch);
        if (next_node == trie_[node_id].next.end()) {
            const size_t new_node_id = trie_.size();
            trie_[node_id].next[ch] = new_node_id;
            node_id = new_node_id;
            trie_.emplace_back(node{});
        }
        else {
            node_id = next_node->second;
        }
    }

    trie_[node_id].is_complete_word = true;
}

size_t trie::next(const size_t node_id, char ch) const
{
    const auto node = trie_[node_id].next.find(ch);
    return trie_[node_id].next.end() == node ? static_cast<size_t>(0) : node->second;
}

bool trie::leaf(size_t node_id) const
{
    return trie_[node_id].is_complete_word;
}

const std::vector<trie::node>& trie::get_trie() const
{
    return trie_;
}
