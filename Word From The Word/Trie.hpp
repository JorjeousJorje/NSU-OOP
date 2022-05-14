#pragma once
#include <map>
#include <string>
#include <unordered_map>

typedef struct tNode
{
	char character;
	bool is_complete_word;
	std::map<char, tNode*> children;
} Node;

class Trie
{
public:
	Trie();
	void insert(const std::string& word);
	Node* get_trie_root() const;
	~Trie();
	
private:
	Node* trie_root;
	Node* create_node(char ch);
};