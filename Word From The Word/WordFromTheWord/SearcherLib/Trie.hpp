#pragma once
#include <map>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

class trie final
{
	struct node
	{
		node();
		
		std::map<char, size_t> next{};
		bool is_complete_word;
	};
	
public:
	trie();
	void insert(const std::string& word);
	size_t next(size_t node_id, char ch) const;
	bool leaf(size_t node_id) const;
	~trie() = default;


	const std::vector<node>& get_trie() const;
	
private:
	std::vector<node> trie_;
};