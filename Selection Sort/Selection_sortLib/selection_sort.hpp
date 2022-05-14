#pragma once
#include <algorithm>

namespace selection_sort
{
	template <class Iterator>
	using value_type = typename std::iterator_traits<Iterator>::value_type;

	template<class Iterator>
	using is_rai = std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>;


	
	template <class RandomAccessIterator,
			  class Compare = std::less<value_type<RandomAccessIterator>>,
		      std::enable_if_t<is_rai<RandomAccessIterator>::value, int> = 0
	>
	void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp = Compare{})
	{
		for (auto i = first; i != last; ++i) {

			auto min = i;
			for (auto j = i + 1; j != last; ++j) {

				if (comp(*j, *min)) {
					min = j;
				}
			}
			std::swap(*i, *min);
		}
	}
}
