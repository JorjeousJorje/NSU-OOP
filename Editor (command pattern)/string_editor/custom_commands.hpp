#pragma once
#include "document.hpp"
#include "editor_command.hpp"
#include <utility>


class copy_command final : public editor_command {
public:
	copy_command() = delete;
	copy_command(document& doc, const size_t id1, const size_t id2) : doc_(doc), idx1_(id1), idx2_(id2) {}
	void execute() override {
		prev_buf_ = doc_.buf();
		doc_.copy_in_buffer(idx1_, idx2_);
	}
	void undo() override {
		doc_.reset_buffer(prev_buf_);
	}
private:
	document& doc_;
	std::string prev_buf_{};
	const std::size_t idx1_ , idx2_;
	
};


class paste_command final : public editor_command {
public:
	paste_command(document& doc, const size_t id1) : doc_(doc), idx1_(id1) {}
	void execute() override {
		doc_.insert(idx1_, doc_.buf());
		last_buffer_size_ = doc_.buf().size();
	}
	void undo() override {
		doc_.delete_elements(idx1_, last_buffer_size_ + idx1_);
	}
private:
	document& doc_;
	std::size_t last_buffer_size_{};
	const std::size_t idx1_;
};


class insert_command final : public editor_command {
public:
	insert_command(document& doc, std::string in_str, const size_t id1) : doc_(doc), in_str_(std::move(in_str)), idx1_(id1) {}
	void execute() override {
		doc_.insert(idx1_, in_str_);
	}
	void undo() override {
		doc_.delete_elements(idx1_, in_str_.size() + idx1_);
	}
private:
	document& doc_;
	const std::string in_str_{};
	const std::size_t idx1_;
};


class delete_command final : public editor_command {
public:
	delete_command(document& doc, const size_t id1, const size_t id2) : doc_(doc), idx1_(id1), idx2_(id2) {}
	void execute() override {
		erased_str_ = doc_.data().substr(idx1_, idx2_ - idx1_);
		doc_.delete_elements(idx1_, idx2_);
	}
	void undo() override {
		doc_.insert(idx1_, erased_str_);
	}
private:
	document& doc_;
	std::string erased_str_{};
	const std::size_t idx1_, idx2_;
};
