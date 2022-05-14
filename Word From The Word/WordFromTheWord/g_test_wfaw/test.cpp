#include "pch.h"
#include <fstream>
#include "WordFinder.hpp"


TEST(empty_word, word_find) {


	std::stringstream in("tight puppy");
	
	words_parser parser(in);
	word_finder finder;

	parser.parse(finder);
	const auto found_words = finder.find_subword("");
	ASSERT_TRUE(found_words.empty());
}
TEST(one_word, word_find) {


	std::stringstream in("tight puppy");

	words_parser parser(in);
	word_finder finder;

	parser.parse(finder);
	std::string str("tight");
	auto found_words = finder.find_subword(str);
	ASSERT_TRUE(!found_words.empty());
	ASSERT_EQ(found_words[0], str);
}
TEST(two_words, word_find) {


	std::stringstream in("tight puppy");

	words_parser parser(in);
	word_finder finder;

	parser.parse(finder);
	std::string str("tightpuppy");
	auto found_words = finder.find_subword(str);
	ASSERT_TRUE(!found_words.empty());
	std::string str1("tight");
	std::string str2("puppy");
	ASSERT_EQ(found_words[0], str1);
	ASSERT_EQ(found_words[1], str2);
}
TEST(alotof_words, word_find) {


	std::stringstream in("tight puppy decay wall parrot call attenuation visual");

	words_parser parser(in);
	word_finder finder;

	parser.parse(finder);
	std::string str("attenuapuppytiondecayvisual");
	auto found_words = finder.find_subword(str);
	ASSERT_TRUE(!found_words.empty());
	std::string str1("attenuation");
	std::string str2("puppy");
	std::string str3("decay");
	std::string str4("visual");
	ASSERT_EQ(found_words[0], str1);
	ASSERT_EQ(found_words[1], str2);
	ASSERT_EQ(found_words[2], str3);
	ASSERT_EQ(found_words[3], str4);
}
TEST(wrong_input, word_find) {


	std::stringstream in("aaaaaaaaaaaaaaaaaa");

	words_parser parser(in);
	word_finder finder;

	parser.parse(finder);
	std::string str("attenuapuppytiondecayvisual");
	auto found_words = finder.find_subword(str);
	ASSERT_TRUE(found_words.empty());
}
TEST(double_letters_word, word_find) {

	std::stringstream in("tight");

	words_parser parser(in);
	word_finder finder;

	parser.parse(finder);
	std::string str("ttighht");
	auto found_words = finder.find_subword(str);
	ASSERT_TRUE(!found_words.empty());
	ASSERT_EQ(found_words[0], "tight");
}
TEST(insert_in_trie, trie_test)
{
	trie trie_;

	std::string test("simpletest");
	trie_.insert(test);
	auto Trie = trie_.get_trie();

	const auto it = Trie[0].next.find(test[0]);
	ASSERT_TRUE(it != Trie[0].next.end());
	auto [ch, node_id] = *it;
	ASSERT_TRUE(node_id == 1);
	ASSERT_TRUE(ch == test[0]);
	const auto it1 = Trie[1].next.find(test[1]);
	ASSERT_TRUE(it1 != Trie[1].next.end());
	auto [ch1, node_id1] = *it1;
	ASSERT_TRUE(node_id1 == 2);
	ASSERT_TRUE(ch1 == test[1]);

	const auto it_last = Trie[9].next.find(test[9]);
	ASSERT_TRUE(it_last != Trie[9].next.end());
	auto [last_ch, last_node_id] = *it_last;
	ASSERT_TRUE(last_node_id == 10);
	ASSERT_TRUE(last_ch == test[9]);
	
	std::string test1("simpletest");
	Trie = trie_.get_trie();
	ASSERT_TRUE(Trie.size() == test.size() + 1);
}
TEST(insert_existing_word, trie_test)
{
	trie trie_;

	std::string test("simpletest");
	trie_.insert(test);
	auto Trie = trie_.get_trie();

	auto size1 = Trie.size();


	std::string test1("simpletest");
	trie_.insert(test1);
	Trie = trie_.get_trie();

	auto size2 = Trie.size();
	ASSERT_TRUE(size1 == size2);
}
TEST(insert_two_words, trie_test)
{
	trie trie_;

	std::string test("simpletest");
	trie_.insert(test);
	
	auto Trie = trie_.get_trie();

	auto it = Trie[0].next.find(test[0]);
	ASSERT_TRUE(it != Trie[0].next.end());
	auto [ch, node_id] = *it;
	ASSERT_TRUE(node_id == 1);
	ASSERT_TRUE(ch == test[0]);
	it = Trie[1].next.find(test[1]);
	ASSERT_TRUE(it != Trie[1].next.end());
	auto [ch1, node_id1] = *it;
	ASSERT_TRUE(node_id1 == 2);
	ASSERT_TRUE(ch1 == test[1]);

	it = Trie[9].next.find(test[9]);
	ASSERT_TRUE(it != Trie[9].next.end());
	auto [last_ch, last_node_id] = *it;
	ASSERT_TRUE(last_node_id == 10);
	ASSERT_TRUE(last_ch == test[9]);

	std::string test1("anotherstr");
	trie_.insert(test1);
	
	Trie = trie_.get_trie();
	
	auto it1 = Trie[0].next.find(test1[0]);
	ASSERT_TRUE(it1 != Trie[0].next.end());
	auto [chr1, nd_id] = *it1;
	ASSERT_TRUE(nd_id == 11);
	ASSERT_TRUE(chr1 == test1[0]);
	it1 = Trie[11].next.find(test1[1]);
	ASSERT_TRUE(it1 != Trie[11].next.end());
	auto [chr2, nd_id1] = *it1;
	ASSERT_TRUE(nd_id1 == 12);
	ASSERT_TRUE(chr2 == test1[1]);

	auto it_last = Trie[19].next.find(test1[9]);
	ASSERT_TRUE(it_last != Trie[19].next.end());
	auto [last_ch1, last_node_id1] = *it_last;
	ASSERT_TRUE(last_node_id1 == 20);
	ASSERT_TRUE(last_ch1 == test1[9]);
}
TEST(next_test, trie_test)
{
	trie trie_;

	std::string test("simpletest");
	trie_.insert(test);

	auto Trie = trie_.get_trie();

	auto it = Trie[0].next.find(test[0]);
	ASSERT_TRUE(it != Trie[0].next.end());
	auto [ch, node_id] = *it;
	ASSERT_TRUE(node_id == 1);
	ASSERT_TRUE(ch == test[0]);
	it = Trie[1].next.find(test[1]);
	ASSERT_TRUE(it != Trie[1].next.end());
	auto [ch1, node_id1] = *it;
	ASSERT_TRUE(node_id1 == 2);
	ASSERT_TRUE(ch1 == test[1]);

	it = Trie[9].next.find(test[9]);
	ASSERT_TRUE(it != Trie[9].next.end());
	auto [last_ch, last_node_id] = *it;
	ASSERT_TRUE(last_node_id == 10);
	ASSERT_TRUE(last_ch == test[9]);

	auto id = trie_.next(0, test[0]);
	ASSERT_TRUE(id == 1);
	auto id1 = trie_.next(8, test[8]);
	ASSERT_TRUE(id1 == 9);
	auto id2 = trie_.next(10, test[9]);
	ASSERT_TRUE(id2 == 0);
}
TEST(leaf_test, trie_test)
{
	trie trie_;

	std::string test("simpletest");
	trie_.insert(test);

	auto Trie = trie_.get_trie();

	auto it = Trie[0].next.find(test[0]);
	ASSERT_TRUE(it != Trie[0].next.end());
	auto [ch, node_id] = *it;
	ASSERT_TRUE(node_id == 1);
	ASSERT_TRUE(ch == test[0]);
	it = Trie[1].next.find(test[1]);
	ASSERT_TRUE(it != Trie[1].next.end());
	auto [ch1, node_id1] = *it;
	ASSERT_TRUE(node_id1 == 2);
	ASSERT_TRUE(ch1 == test[1]);

	it = Trie[9].next.find(test[9]);
	ASSERT_TRUE(it != Trie[9].next.end());
	auto [last_ch, last_node_id] = *it;
	ASSERT_TRUE(last_node_id == 10);
	ASSERT_TRUE(last_ch == test[9]);

	ASSERT_TRUE(trie_.leaf(10));
	ASSERT_TRUE(!trie_.leaf(2));
	ASSERT_TRUE(!trie_.leaf(1));
	ASSERT_TRUE(!trie_.leaf(5));
	
	std::string test1("anotherstr");
	trie_.insert(test1);

	Trie = trie_.get_trie();

	auto it1 = Trie[0].next.find(test1[0]);
	ASSERT_TRUE(it1 != Trie[0].next.end());
	auto [chr1, nd_id] = *it1;
	ASSERT_TRUE(nd_id == 11);
	ASSERT_TRUE(chr1 == test1[0]);
	it1 = Trie[11].next.find(test1[1]);
	ASSERT_TRUE(it1 != Trie[11].next.end());
	auto [chr2, nd_id1] = *it1;
	ASSERT_TRUE(nd_id1 == 12);
	ASSERT_TRUE(chr2 == test1[1]);

	auto it_last = Trie[19].next.find(test1[9]);
	ASSERT_TRUE(it_last != Trie[19].next.end());
	auto [last_ch1, last_node_id1] = *it_last;
	ASSERT_TRUE(last_node_id1 == 20);
	ASSERT_TRUE(last_ch1 == test1[9]);

	ASSERT_TRUE(trie_.leaf(20));
	ASSERT_TRUE(!trie_.leaf(12));
	ASSERT_TRUE(!trie_.leaf(11));
	ASSERT_TRUE(!trie_.leaf(5));
	ASSERT_TRUE(trie_.leaf(10));
}