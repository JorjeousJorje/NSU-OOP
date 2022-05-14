#pragma once
#include <vector>
#include <string>
#include "Trie.hpp"
#include "Parser.hpp"

class word_finder : public ParserClient
{
public:
	virtual void process_row(const std::vector<std::string>& parsed_data);
	word_finder() = default;
	
	void find_subword(const std::string& word);
	std::vector<std::string> get_found_words() const;
	
private:
	std::vector<std::string> found_words;
	Trie dictionary_;
};