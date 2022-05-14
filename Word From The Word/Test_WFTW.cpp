#include "Test_WFTW.hpp"
#include "WordFinder.hpp"

void start_test(std::istream& in, std::ostream& out)
{
	std::string word;
	std::cout << "enter the word:" << std::endl;
	std::cin >> word;
	
	Words_Parser parser(in);
	word_finder finder;
	
	parser.parse(finder);
	finder.find_subword(word);
	
	auto founded_words = finder.get_found_words();
	for (auto it = founded_words.begin(); it != founded_words.end(); ++it) {
		out << *it << '\n';
	}
	
}
