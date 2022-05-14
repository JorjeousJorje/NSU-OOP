#include "Trie.hpp"
#include <unordered_set>

namespace
{
    void recursive_deleting(Node* node)
    {
        Node* copy_node = node;
        if (!node->is_complete_word) {
            auto begin_node = copy_node->children.begin();
            const auto end_node = copy_node->children.end();
            while (begin_node != end_node) {
                recursive_deleting(begin_node->second);
                ++begin_node;
            }
        }
        delete node;
    }
}

Trie::Trie()
{
	trie_root = new Node;
	trie_root->is_complete_word = false;
}

Node* Trie::create_node(const char ch)
{
    Node* node = new Node;
    node->character = ch;
    node->is_complete_word = false;
    return node;
}

Trie::~Trie()
{
    auto begin = trie_root->children.begin();
    const auto end = trie_root->children.end();
    while (begin != end) {
        recursive_deleting(begin->second);
        ++begin;
    }
    delete trie_root;
}

void Trie::insert(const std::string& word)
{
	Node* current_root = trie_root;
    for(auto it = word.begin(); it != word.end(); ++it)
    {
        if (current_root->children.find(*it) == current_root->children.end()) {
            current_root->children[*it] = create_node(*it);
        }
    	
        current_root = current_root->children[*it];
    }

    current_root->is_complete_word = true;
}

Node* Trie::get_trie_root() const
{
    return trie_root;
}

