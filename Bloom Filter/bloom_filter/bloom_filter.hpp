#pragma once
#include <memory>
#include <vector>
#include <array>
#include <bitset>


inline constexpr std::size_t number_of_unique_hashes = 64;

template<class Key,
		 size_t TableSize = 50,
		 size_t NumFunctions = number_of_unique_hashes,
		 class Hash = std::hash<Key>,
		 std::enable_if_t<NumFunctions <= number_of_unique_hashes, bool> = true
>
class bloom_filter
{
	using key_type = Key;
	using value_type = Key;
	using const_reference = const value_type&;
	using hash = Hash;

public:
	
	void insert(const_reference value) {
		const auto current_hash = static_cast<unsigned>(hash_(value));

		for (auto salt = salts_.begin(); salt != std::next(salts_.begin(), NumFunctions); ++salt) {

			const auto sub_hash = current_hash ^ *salt;
			const auto index = sub_hash % TableSize;

			bitset_[index] = true;

		}
	}
	
	bool query(const_reference value) {
		const auto current_hash = static_cast<unsigned>(hash_(value));

		for (auto salt = salts_.begin(); salt != std::next(salts_.begin(), NumFunctions); ++salt) {

			const auto sub_hash = current_hash ^ *salt;
			const auto index = sub_hash % TableSize;

			if (bitset_[index] == false) {
				return false;
			}
		}

		return true;
	}

	[[nodiscard]] const std::bitset<TableSize>& read() const {
		return bitset_;
	}
	
	void load(const std::bitset<TableSize>& table) {
		bitset_ = table;
	} 

	friend bool operator==(const bloom_filter& x, const bloom_filter& y) {
		return x.bitset_ == y.bitset_;
	}
	friend bool operator!=(const bloom_filter& x, const bloom_filter& y) {
		return !(x == y);
	}


	bloom_filter operator& (const bloom_filter& another) {
		if (this == &another) {
			return *this;
		}

		bloom_filter union_{};
		union_.bitset_ = bitset_ & another.bitset_;
		return union_;
	}

	bloom_filter operator| (const bloom_filter& another) {
		if (this == &another) {
			return *this;
		}

		bloom_filter intersection{};
		intersection.bitset_ = bitset_ | another.bitset_;
		return intersection;
	}

private:
	std::bitset<TableSize> bitset_;
	hash hash_{ Hash() };


	const std::array<unsigned, number_of_unique_hashes> salts_ = {
		0x1953c322, 0x588ccf17, 0x64bf600c, 0xa6be3f3d,
		0x341a02ea, 0x15b03217, 0x3b062858, 0x5956fd06,
		0x18b5624f, 0xe3be0b46, 0x20ffcd5c, 0xa35dfd2b,
		0x1fc4a9bf, 0x57c45d5c, 0xa8661c4a, 0x4f1b74d2,
		0x5a6dde13, 0x3b18dac6, 0x05a8afbf, 0xbbda2fe2,
		0xa2520d78, 0xe7934849, 0xd541bc75, 0x09a55b57,
		0x9b345ae2, 0xfc2d26af, 0x38679cef, 0x81bd1e0d,
		0x654681ae, 0x4b3d87ad, 0xd5ff10fb, 0x23b32f67,
		0xafc7e366, 0xdd955ead, 0xe7c34b1c, 0xfeace0a6,
		0xeb16f09d, 0x3c57a72d, 0x2c8294c5, 0xba92662a,
		0xcd5b2d14, 0x743936c8, 0x2489beff, 0xc6c56e00,
		0x74a4f606, 0xb244a94a, 0x5edfc423, 0xf1901934,
		0x24af7691, 0xf6c98b25, 0xea25af46, 0x76d5f2e6,
		0x5e33cdf2, 0x445eb357, 0x88556bd2, 0x70d1da7a,
		0x54449368, 0x381020bc, 0x1c0520bf, 0xf7e44942,
		0xa27e2a58, 0x66866fc5, 0x12519ce7, 0x437a8456,
	};
};

