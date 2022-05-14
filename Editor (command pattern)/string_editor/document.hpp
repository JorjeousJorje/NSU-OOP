#pragma once
#include <string>
#include <istream>
#include <utility>

class document final
{
public:
	document() = delete;
	explicit document(std::istream& in){
		std::getline(in, str_);
	}
	explicit document(std::string in) : str_(std::move(in)) {}
	
	void insert(std::size_t pos, const std::string& in_str);
	void delete_elements(std::size_t idx1, std::size_t idx2);
	
	[[nodiscard]] const std::string& data() const { return str_; }
	[[nodiscard]] const std::string& buf() const { return buffer_; }
	[[nodiscard]] size_t size() const { return str_.size(); }

	void copy_in_buffer(std::size_t idx1, size_t idx2);
	void reset_buffer(const std::string& prev_buf) { buffer_ = prev_buf; }

private:
	std::string str_{};
	std::string buffer_{};
};
