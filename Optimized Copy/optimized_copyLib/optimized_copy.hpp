#pragma once
#include <type_traits>
#include <vector>


namespace optimized
{
    template<class Iterator>
    using iterator_value = typename std::iterator_traits<Iterator>::value_type;

    template<class T>
    using vector_iterator = typename std::vector<T>::iterator;

    template<typename Iterator1, typename Iterator2>
    inline constexpr bool is_same_value = std::is_same_v<iterator_value<Iterator1>, iterator_value<Iterator2>>;

    template<typename Iterator>
    inline constexpr bool is_pointer_or_vector_it = std::is_same_v<Iterator, vector_iterator<iterator_value<Iterator>>> || std::is_pointer_v<Iterator>;

    template <class InputIterator, class OutputIterator>
    inline constexpr bool is_trivial_copy =
        std::is_trivial_v<iterator_value<InputIterator>> &&
        is_same_value<InputIterator, OutputIterator> &&
        is_pointer_or_vector_it<InputIterator> &&
        is_pointer_or_vector_it<OutputIterator>;

    template <class InputIterator, class OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result,
        std::enable_if_t<!is_trivial_copy<InputIterator, OutputIterator>, int> = 0)
    {
        while (first != last) {
            //next line of code may cause WARNING if you copy different types, but
			//this copy function is designed with awareness of this WARNING
            *result = *first;
            ++result; ++first;
        }
        return result;
    }

    template <class InputIterator, class OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result,
        std::enable_if_t<is_trivial_copy<InputIterator, OutputIterator>, int> = 0)
    {
        std::memmove(&*result, &*first, sizeof(*first) * std::distance(first, last));
        return result;
    }
}