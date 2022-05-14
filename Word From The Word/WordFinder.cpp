#include "WordFinder.hpp"

#include <stack>

namespace
{
    void dfs(std::string::const_iterator& prev_ch, const std::string& word, Node* current_node, std::pair<std::string, bool>& buffer)
	{
        ++prev_ch;
        buffer.first += current_node->character;

        if (current_node->is_complete_word) {
            buffer.second = true;
            return;
        }
    	
        auto ch = prev_ch;
        for (;ch != word.end();) {
        	if (current_node->children.find(*ch) != current_node->children.end()) {
                dfs(ch, word, current_node->children[*ch],buffer);
            }
            if (buffer.second || ch == word.end()) {
                break;
            }
            ++ch;
        }
        if (ch == word.end() && !buffer.second && buffer.first.size() > 1) {
            buffer.first.erase(buffer.first.size() - 1);
        }
	}
}

void word_finder::process_row(const std::vector<std::string>& parsed_data)
{
	for(auto it = parsed_data.begin(); it != parsed_data.end(); ++it) {
		dictionary_.insert(*it);
	}
}

void word_finder::find_subword(const std::string& word)
{
    Node* root_node = dictionary_.get_trie_root();
    Node* current_node = root_node;
    std::vector<char> used_ch;
    std::pair<std::string, bool> buffer;
    buffer.second = false;
	
    for (auto ch = word.begin(); ch != word.end();) {

        if (std::find(used_ch.begin(), used_ch.end(), *ch) != used_ch.end() && buffer.first.empty()) {
            ++ch;
            continue;
        }
    	
        if(current_node->children.find(*ch) != current_node->children.end()) {

            dfs(ch, word, current_node->children[*ch],buffer);
        	
        	if(buffer.second) {
                found_words.push_back(buffer.first);
                current_node = root_node;
                used_ch.emplace(used_ch.begin(),buffer.first[0]);
                ch = word.begin();
                buffer.first.clear();
                buffer.second = false;
        		continue;
        	}
        	if(!buffer.second) {
                ch = word.begin();
                current_node = root_node;
                used_ch.emplace(used_ch.begin(),buffer.first[0]);
                buffer.first.clear();
        		continue;
        	}
        }
        ++ch;
    }
}

std::vector<std::string> word_finder::get_found_words() const
{
	return found_words;
}


