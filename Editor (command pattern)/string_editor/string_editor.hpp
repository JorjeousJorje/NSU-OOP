#pragma once
#include "custom_commands.hpp"
#include <memory>
#include <stack>
#include <iostream>

class string_editor final {
	using command_ptr = std::unique_ptr<editor_command>;
public:
	string_editor() = delete;
	explicit string_editor(std::istream& str_to_edit) : doc_{ str_to_edit } {}
	explicit string_editor(const std::string& str_to_edit) : doc_{ str_to_edit } {}

	void copy(std::size_t idx1, std::size_t idx2);
	void paste(std::size_t idx);
	void insert(const std::string& in_str, std::size_t idx);
	void delete_elements(std::size_t idx1, std::size_t idx2);
	void save(std::ostream& out) const;

	void redo();
	void undo();
	[[nodiscard]] const document& doc() const { return doc_; }
	
private:
	document doc_;
	std::stack<command_ptr> undo_stack_{};
	std::stack<command_ptr> execute_stack_{};

	void push_command(command_ptr&& cmd);
};
