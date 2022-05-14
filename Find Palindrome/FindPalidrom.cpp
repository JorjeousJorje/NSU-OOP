#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include "FindPalindrom.hpp"

std::vector<std::string> palindrom::parse(const std::string& str) {
	std::istringstream input(str);
	std::vector<std::string> output;
	std::string word;
	while (input >> word)
	{
		output.push_back(word);
	}
	return output;
}

void palindrom::prepare(std::string& str)
{
	str.erase(remove_if(str.begin(), str.end(), ispunct), str.end());
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}

void palindrom::words_as_literals(std::vector<std::string>& v)
{
	std::cout << ' ' << "Sentence as a word set:" << std::endl;

	std::string start_str;
	for (auto i = v.rbegin(); i != v.rend(); ++i) {
		start_str += *i + ' ';
	}
	start_str.erase(start_str.end() - 1);
	for (size_t i = 0; i < v.size(); ++i) {
		if (v[i] != v[v.size() - i - 1]) {
			std::cout << ' ' << start_str << " - no" << std::endl;
			return;
		}
	}
	std::cout << ' ' << start_str << " - yes" << std::endl;
}

bool palindrom::is_palindrom(const std::string& str)
{
	if (str[0] != str[str.length() - 1]) {
		return false;
	}
	if (str.length() < 2) {
		return true;
	}
	std::string rev(str);
	std::reverse(rev.begin(), rev.end());
	return rev == str;
}

void palindrom::whole_sentence(std::string& str)
{
	std::cout << ' ' << "Sentence as a word:" << std::endl;

	if (is_palindrom(str)) {
		std::reverse(str.begin(), str.end());
		std::cout << ' ' << str << " - yes" << std::endl;
	}
	else {
		std::reverse(str.begin(), str.end());
		std::cout << ' ' << str << " - no" << std::endl;
	}
}

void palindrom::single_words(std::vector<std::string>& v)
{
	std::cout << ' ' << "Words:" << std::endl;

	for (auto i = v.begin(); i != v.end(); ++i) {
		if (is_palindrom(*i)) {
			std::reverse((*i).begin(), (*i).end());
			std::cout << ' ' << *i << " - yes" << std::endl;
		}
		else {
			std::reverse((*i).begin(), (*i).end());
			std::cout << ' ' << *i << " - no" << std::endl;
		}
	}
}

bool palindrom::read(std::istream& in)
{
	if (in.peek() != EOF) {
		std::getline(in, str_);
		std::cout << " Input string:\n " << str_ << '\n' << std::endl;
		return true;
	}
	return false;
}

void palindrom::write_result()
{
	prepare(str_);
	v_ = parse(str_);
	str_.erase(remove_if(str_.begin(), str_.end(), isspace), str_.end());
	whole_sentence(str_);
	std::cout << '\n';
	words_as_literals(v_);
	std::cout << '\n';
	single_words(v_);
}
