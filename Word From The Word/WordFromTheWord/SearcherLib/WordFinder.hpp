#pragma once
#include <vector>
#include <string>
#include "Trie.hpp"
#include "Parser.hpp"

class word_finder final : public parser_client 
{
public:
	void process_row(const std::string& parsed_data) override;
	word_finder() = default;
	std::vector<std::string> find_subword(const std::string& word);
	
private:
	trie dictionary_;
};