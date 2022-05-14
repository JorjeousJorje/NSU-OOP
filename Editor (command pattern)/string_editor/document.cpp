#include "document.hpp"

void document::insert(const std::size_t pos, const std::string& in_str)
{
	if (pos >= str_.size()) {
		str_.insert(str_.size(), in_str);
	}
	else {
		str_.insert(pos, in_str);
	}
}

void document::delete_elements(const std::size_t idx1, const std::size_t idx2)
{
	if(idx1 >= str_.size() || idx1 > idx2) {
		return;
	}
	str_.erase(idx1, idx2 - idx1);
}


void document::copy_in_buffer(const std::size_t idx1, const size_t idx2)
{
	if(idx1 >= str_.size() || idx1 > idx2) {
		return;
	}
	buffer_ = str_.substr(idx1, idx2 - idx1);
}



