#pragma once
#include <algorithm>
#include <functional>
#include <array>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>

template <	typename Key,
            typename Value,
            typename Compare = std::less<Key>,
            typename Allocator = std::allocator<std::pair<const Key, Value>>>
class skip_list final {
	struct node;
	
	template <bool const_check>
	class skip_list_iterator;

public:
	using key_type		  = Key;
	using mapped_type	  = Value;
	using value_type	  = std::pair<const Key, Value>;
	using size_type		  = std::size_t;
	using key_compare     = Compare;
	using allocator_type  = Allocator;
	using reference		  = value_type&;
	using const_reference = const value_type&;
	using pointer		  = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer	  = typename std::allocator_traits<Allocator>::const_pointer;
	using difference_type = std::ptrdiff_t;

private:
	static constexpr size_type max_level{ 5 };
	
	using value_ptr = std::unique_ptr<value_type, std::function<void(pointer)>>;
	using node_ptr = std::shared_ptr<node>;
	using node_container = std::array<node_ptr, max_level + 1>;
	
	size_type size_{};
	size_type level_{};
	
	const std::uniform_int_distribution<size_type> coin_flip{ size_type(0), size_type(1) };
	std::mt19937 engine{ unsigned(level_) };

	key_compare comp_{ Compare() };
	allocator_type alloc_{ Allocator() };

	node_ptr head{ create_node(key_type(), mapped_type()) };
	
	node_ptr create_node(key_type&& key, mapped_type&& value) {
		auto ptr = alloc_.allocate(1);
		auto v_ptr = value_ptr( new(ptr) value_type(key, value), [this](pointer ptr1) { ptr1->~value_type(); 
																						alloc_.deallocate(ptr1, 1); } );
		return std::make_shared<node>(std::move(v_ptr));
	}
	std::pair<node_ptr, node_container> find_n_update(const key_type& key)
	{
		node_container update_nodes{};
		update_nodes.fill(head);
		
		auto current_node = head;
		for (size_type i = 0; i <= level_; ++i) {
			while (current_node->next[level_ - i] && comp_(current_node->next[level_ - i]->data->first, key)) {
				current_node = current_node->next[level_ - i];
			}
			update_nodes[level_ - i] = current_node;
		}
		return std::make_pair(current_node->next[0], update_nodes);
	}

	struct node {
		value_ptr data;
		node_container next;
		node() = default;
		explicit node(value_ptr&& data) : data(std::move(data)) {}
	};

	template <bool const_check>
	class skip_list_iterator final {
	public:
		using value_type		= std::conditional_t<const_check, const value_type, value_type>;
		using reference		    = std::conditional_t<const_check, const_reference, reference>;
		using pointer			= std::conditional_t<const_check, const_pointer, pointer>;
		using distance_type		= difference_type;
		using difference_type	= difference_type;
		using iterator_category = std::forward_iterator_tag;

	public:
		skip_list_iterator() = default;

		skip_list_iterator& operator++()
		{
			node_ptr node_shared = node_.lock();
			if (!node_shared) {
				throw std::out_of_range("iterator is out of range");
			}
			node_ = node_shared->next[0];
			return *this;
		}
		skip_list_iterator operator++(int) { auto copy = *this; ++*this; return copy; }

		reference operator*() const
		{
			node_ptr node_shared = node_.lock();
			if (!node_shared) {
				throw std::out_of_range("iterator is empty");
			}
			return *(node_shared->data);
		}
		pointer operator->() const
		{
			node_ptr node_shared = node_.lock();
			if (!node_shared) {
				throw std::out_of_range("iterator is empty");
			}
			return node_shared->data.get();
		}

		bool operator==(const skip_list_iterator& another) const
		{
			node_ptr node = node_.lock(), another_node = another.node_.lock();
			if (node && another_node) {
				return node == another_node;
			}
			return !node && !another_node ? true : false;
		}
		bool operator!=(const skip_list_iterator& another) const { return !(*this == another); }
		
	protected:
		friend class skip_list;
		std::weak_ptr<node> node_{};

		explicit skip_list_iterator(const node_ptr& node) : node_(node) {}
	};

public:
	using iterator		 = skip_list_iterator<false>;
	using const_iterator = skip_list_iterator<true>;
	
	skip_list() = default;
	explicit skip_list(const key_compare& comp, const allocator_type& alloc = Allocator()) : comp_(comp), alloc_(alloc) {}

	skip_list(const skip_list& another)				          { *this = another; }
	skip_list(skip_list&& another) noexcept			          { *this = std::move(another); }

	[[nodiscard]] iterator begin()							  { return iterator(head->next[0]); }
	[[nodiscard]] const_iterator begin() const				  { return const_iterator(head->next[0]); }
	
	[[nodiscard]] iterator end()							  { return iterator{}; }
	[[nodiscard]] const_iterator end() const				  { return const_iterator{}; }

	[[nodiscard]] bool empty() const				          { return size_ == 0; }
	[[nodiscard]] size_type size() const			          { return size_; }

	skip_list& operator=(const skip_list& another);
	skip_list& operator=(skip_list&& another) noexcept;
	friend bool operator==(const skip_list& x, const skip_list& y)
	{
		if (x.size() != y.size()) {
			return false;
		}
		
		for (auto x_it = x.begin(), y_it = y.begin(); x_it != x.end(); ++x_it, ++y_it) {
			if (*x_it != *y_it) {
				return false;
			}
		}
		return true;
	}
	friend bool operator!=(const skip_list& x, const skip_list& y)
	{
		return !(x == y);
	}
	
	[[nodiscard]] iterator find(const key_type& key);
	[[nodiscard]] const_iterator find(const key_type& key) const;
	
	mapped_type& operator[](const key_type& key);
	mapped_type& at(const key_type& key);
	[[nodiscard]] const mapped_type& at(const key_type& key) const;

	std::pair<iterator, bool> insert(const_reference val);

	void erase(iterator position);
	void erase(iterator first, iterator last);
	size_type erase(const key_type& key);

	void swap(skip_list& another) noexcept;
	void clear();
	~skip_list() = default;
};

template <typename K, typename V, typename C, typename A>
void skip_list<K, V, C, A>::clear()
{
	size_ = {}, level_ = {};
	head = create_node(key_type(), mapped_type());
}

template <typename K, typename V, typename C, typename A>
skip_list<K, V, C, A>& skip_list<K, V, C, A>::operator=(skip_list&& another) noexcept
{
	if (this != &another) {
		size_ = another.size_, level_ = another.level_;
		std::swap(head, another.head);
		std::swap(comp_, another.comp_);
		std::swap(alloc_, another.alloc_);
		another.clear();
	}
	return *this;
}

template <typename K, typename V, typename C, typename A>
skip_list<K, V, C, A>& skip_list<K, V, C, A>::operator=(const skip_list& another)
{
	if(this != &another) {
		clear();
		level_ = another.level_;
		comp_ = another.comp_; alloc_ = another.alloc_;
		for (auto i = another.begin(); i != another.end(); ++i) {
			(*this).insert(std::make_pair(i->first, i->second));
		}
	}
	return *this;
}

template <typename K, typename V, typename C, typename A>
typename skip_list<K, V, C, A>::mapped_type& skip_list<K, V, C, A>::operator[](const key_type& key)
{
	auto it = find(key);
	if (it == end()) {
		return insert(std::make_pair(key, mapped_type())).first->second;
	}
	return it->second;
}

template <typename K, typename V, typename C, typename A>
typename skip_list<K, V, C, A>::mapped_type& skip_list<K, V, C, A>::at(const key_type& key)
{
	auto it = find(key);
	if(it == end()) {
		throw std::out_of_range("invalid s_list<K, T> key");
	}
	return it->second;
}

template <typename K, typename V, typename C, typename A>
const typename skip_list<K, V, C, A>::mapped_type& skip_list<K, V, C, A>::at(const key_type& key) const
{
	auto it = find(key);
	if (it == end()) {
		throw std::out_of_range("invalid s_list<K, T> key");
	}
	return it->second;
}

template <typename K, typename V, typename C, typename A>
typename skip_list<K, V, C, A>::iterator skip_list<K, V, C, A>::find(const key_type& key)
{
	auto current_node = head;
	for (size_type i = 0; i <= level_; ++i) {
		while (current_node->next[level_ - i] && comp_(current_node->next[level_ - i]->data->first, key)) {
			current_node = current_node->next[level_ - i];
		}
	}
	if (current_node->next[0] && current_node->next[0]->data->first == key) {
		return iterator(current_node->next[0]);
	}
	return end();
}

template <typename K, typename V, typename C, typename A>
typename skip_list<K, V, C, A>::const_iterator skip_list<K, V, C, A>::find(const key_type& key) const
{
	auto current_node = head;
	for (size_type i = 0; i <= level_; ++i) {
		while (current_node->next[level_ - i] && comp_(current_node->next[level_ - i]->data->first, key)) {
			current_node = current_node->next[level_ - i];
		}
	}
	if (current_node->next[0] && current_node->next[0]->data->first == key) {
		return const_iterator(current_node->next[0]);
	}
	return end();
}

template <typename K, typename V, typename C, typename A>
std::pair<typename skip_list<K, V, C, A>::iterator, bool> skip_list<K, V, C, A>::insert(const_reference val)
{
	auto [current_node, update_nodes] = find_n_update(val.first);
	
	if (current_node && current_node->data->first == val.first) {
		return std::make_pair(iterator(current_node), false);
	}
	
	++size_;
	
	size_type node_level;
	for (node_level = 0; coin_flip(engine) != 1 && node_level <= max_level; ++node_level) {}
	level_ = node_level > level_ ? node_level : level_;

	auto new_node = create_node(std::move(key_type(val.first)), std::move(mapped_type(val.second)));
	
	for (size_type i = 0; i <= node_level; ++i) {
		new_node->next[i] = update_nodes[i]->next[i];
		update_nodes[i]->next[i] = new_node;
	}
	return std::make_pair(iterator(new_node), true);
}

template <typename K, typename V, typename C, typename A>
void skip_list<K, V, C, A>::swap(skip_list& another) noexcept
{
	auto temporary = std::move(*this);
	*this = std::move(another);
	another = std::move(temporary);
}

template <typename K, typename V, typename C, typename A>
void skip_list<K, V, C, A>::erase(iterator position)
{
	if (position != end()) {
		erase(position->first);
	}
}

template <typename K, typename V, typename C, typename A>
typename skip_list<K, V, C, A>::size_type skip_list<K, V, C, A>::erase(const key_type& key)
{
	auto [current_node, update_nodes] = find_n_update(key);
	if (!current_node) {
		return size_type(0);
	}

	for (size_type i = 0; i <= level_; ++i) {
		if (update_nodes[i]->next[i] != current_node) {
			break;
		}
		update_nodes[i]->next[i] = current_node->next[i];
	}
	--size_;

	while (level_ > 0 && !head->next[level_]) {
		--level_;
	}
	return size_type(1);
}

template <typename K, typename V, typename C, typename A>
void skip_list<K, V, C, A>::erase(iterator first, iterator last)
{
	for (auto it = first; it != last;) {
		auto prev_iterator = it++;
		erase(prev_iterator);
	}
}