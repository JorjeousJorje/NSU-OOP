//#include <iostream>
//#include <map>
//#include <numeric>
//#include "skip_list.hpp"
//
//template<class T>
//class custom_alloc {
//public:
//	using value_type = T;
//	using pointer = T*;
//	using const_pointer = const T*;
//	using void_pointer = void*;
//	using const_void_pointer = const void*;
//
//	using size_type = std::size_t;
//	using difference_type = std::ptrdiff_t;
//
//public:
//	pointer allocate(size_type num_of_objects)
//	{
//		num_alloc += num_of_objects;
//		return static_cast<pointer>(operator new(sizeof(T) * num_of_objects));
//	}
//
//	pointer allocate(size_type num_of_objects, const_void_pointer hint)
//	{
//		return allocate(num_of_objects);
//	}
//
//	void deallocate(pointer p, size_type num_of_objects)
//	{
//		operator delete(p);
//	}
//
//	[[nodiscard]] size_type get_allocations() const
//	{
//		return num_alloc;
//	}
//private:
//	size_type num_alloc{};
//};
//
//using ca_pair_int_string = custom_alloc<std::pair<const int, std::string>>;
//
//int main()
//{
//	std::greater<> comp{};
//	ca_pair_int_string alloc{};
//	skip_list<int, std::string, std::greater<>, ca_pair_int_string> l1(comp, alloc);
//	l1[1] = "1";
//	l1[12] = "12";
//	l1[4] = "4";
//	l1[124] = "124";
//	
//
//	return 0;
//}