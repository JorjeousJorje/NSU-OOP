#include "Test_WFTW.hpp"

#include <iterator>

#include "WordFinder.hpp"

void start_test(std::istream& in, std::ostream& out)
{
	/*std::string word;
	std::cout << "enter the word:" << std::endl;
	std::cin >> word;*/
	
	words_parser parser(in);
	word_finder finder;
	
	parser.parse(finder);
	auto founded_words = finder.find_subword("prupyputell");//thhighteh prupypuell
	std::copy(founded_words.begin(), founded_words.end(), std::ostream_iterator<std::string>{out, "\n"});
	
}
