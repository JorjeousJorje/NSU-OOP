#include "WordFinder.hpp"
#include <algorithm>
#include <stack>

namespace
{
    void dfs(size_t id, const std::string& src_str, const char ch, trie& trie_, std::vector<bool>& visited, std::vector<std::string>& found_words)
    {
        std::set<size_t> used_id;
        std::set<size_t> banned_id;
        std::stack<std::pair<size_t, size_t>> stack;
        std::string buffer;
        buffer += ch;
        size_t last_marked = 0;
    	
        stack.push(std::make_pair(id, 0));

    	while(!stack.empty()) {
            auto [current_id, current_depth] = stack.top();
    		
    		for(size_t i = 0; i < src_str.size(); ++i) {
    			
    			if(!visited[i]) {
                    auto children_id= trie_.next(current_id, src_str[i]);
                    if (children_id && used_id.find(children_id) == used_id.end() && banned_id.find(children_id) == banned_id.end()) {
                        visited[i] = true;
                        stack.push(std::make_pair(children_id, ++current_depth));
                        last_marked = i;
                        buffer += src_str[i];
                        break;
                    }	
    			}
                if (trie_.leaf(current_id)) {
                    found_words.push_back(buffer);
                    buffer.erase(buffer.end() - 1);
                    std::replace(visited.begin(), visited.end(), true, false);
                    visited[last_marked] = true;
                    used_id.insert(current_id);
                    stack.pop();
                    break;
                }
                if (i + 1 == src_str.size()) {
                    std::replace(visited.begin(), visited.end(), true, false);
                    buffer.erase(buffer.end() - 1);
                    used_id.clear();
                    banned_id.insert(current_id);
                    stack.pop();
                }
    		}
    	}
    }
	
}

void word_finder::process_row(const std::string& parsed_data)
{
	dictionary_.insert(parsed_data);
}

std::vector<std::string> word_finder::find_subword(const std::string& word)
{
    std::vector<std::string> found_words;
    if (word.empty()) {
        return found_words;
    }
    std::vector<bool> visited(word.size());
    std::fill(visited.begin(), visited.end(), false);

    std::set<char> used_ch;
    const size_t root_id{ 0 };
	
	
    for (size_t i = 0; i < word.size(); ++i) {

        const auto next_id = dictionary_.next(root_id, word[i]);
        if(next_id && used_ch.find(word[i]) == used_ch.end()) {
            visited[i] = true;
            dfs(next_id, word, word[i], dictionary_, visited, found_words);
            used_ch.insert(word[i]);
        }
    }
	
    return found_words;
}


