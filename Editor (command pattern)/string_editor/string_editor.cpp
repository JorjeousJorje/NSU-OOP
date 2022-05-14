#include "string_editor.hpp"

void string_editor::push_command(command_ptr&& cmd)
{
	cmd->execute();
	execute_stack_.emplace(std::move(cmd));
	undo_stack_ = {};
}

void string_editor::redo()
{
	if (undo_stack_.empty()) { return; }
	undo_stack_.top()->execute();
	execute_stack_.emplace(std::move(undo_stack_.top()));
	undo_stack_.pop();
}

void string_editor::undo()
{
	if (execute_stack_.empty()) { return; }
	execute_stack_.top()->undo();
	undo_stack_.emplace(std::move(execute_stack_.top()));
	execute_stack_.pop();
}

void string_editor::copy(const std::size_t idx1, const std::size_t idx2)
{
	if (idx1 > idx2) {
		throw std::invalid_argument("copy exception: wrong index order");
	}
	if (idx1 >= doc_.data().size()) {
		throw std::range_error("copy exception: out of range copy");
	}
    push_command(std::make_unique<copy_command>( doc_, idx1, idx2));
}

void string_editor::paste(const std::size_t idx)
{
	push_command(std::make_unique<paste_command>(doc_, idx));
}

void string_editor::insert(const std::string& in_str, const std::size_t idx)
{
	push_command(std::make_unique<insert_command>(doc_, in_str, idx));
}

void string_editor::delete_elements(const std::size_t idx1, const std::size_t idx2)
{
	if (idx1 > idx2) {
		throw std::invalid_argument("delete exception: wrong index order");
	}
	if (idx1 >= doc_.data().size()) {
		throw std::range_error("delete exception: out of range delete");
	}
	push_command(std::make_unique<delete_command>(doc_, idx1, idx2));
}

void string_editor::save(std::ostream& out) const
{
	out << doc_.data();
}

