#include "pch.h"
#include <sstream>
#include "command_parser.hpp"
#include "custom_logger.hpp"

TEST(Copy, TwoCopyInARow)
{
	string_editor editor{ "editor" };
	editor.copy(0, 10);
	editor.copy(0, 10);
	editor.paste(0);
	EXPECT_EQ(editor.doc().data(), "editoreditor");
}

TEST(CopyPaste, EmptyString)
{
	std::istringstream input{};
	string_editor editor { input  };
	EXPECT_THROW(editor.copy(0, 10), std::range_error);
	editor.paste(1);
	EXPECT_TRUE(editor.doc().data().empty());
}

TEST(CopyPaste, NotEmptyString)
{
	std::istringstream input{"editor"};
	string_editor editor{ input };
	editor.copy(0, 10);
	EXPECT_TRUE(!editor.doc().data().empty());
	editor.paste(0);
	EXPECT_EQ(editor.doc().data(), "editoreditor");
	editor.paste(editor.doc().size());
	EXPECT_EQ(editor.doc().data(), "editoreditoreditor");
}

TEST(CopyPaste, WrongIndexes)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	EXPECT_THROW(editor.copy(2, 1), std::invalid_argument);
	EXPECT_TRUE(!editor.doc().data().empty());
	editor.paste(0);
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.paste(editor.doc().size());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Insert, EmptyString)
{
	std::istringstream input{};
	string_editor editor{ input };
	editor.insert("editor", 10);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Save, InsertInEmptyString)
{
	string_editor editor{ "" };
	editor.insert("editor", 10);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
	std::ostringstream os{};
	editor.save(os);
	EXPECT_EQ(os.str(), "editor");
}

TEST(Save, InsertInNonEmptyString)
{
	string_editor editor{ "hello" };
	editor.insert("editor", 10);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	std::ostringstream os{};
	editor.save(os);
	EXPECT_EQ(os.str(), "helloeditor");
}

TEST(Insert, NotEmptyString)
{
	std::istringstream input{"editor"};
	string_editor editor{ input };
	editor.insert("editor", 10);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editoreditor");
}

TEST(Insert, Begin)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.insert("new", 0);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "neweditor");
}

TEST(Insert, End)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.insert("new", editor.doc().data().size());
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editornew");
}

TEST(Insert, OutOfRange)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.insert("new", editor.doc().data().size() + 1000);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editornew");
}

TEST(Insert, Middle)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.insert("new", 3);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "edinewtor");
}

TEST(Delete, EmptyString)
{
	std::istringstream input{};
	string_editor editor{ input };
	EXPECT_THROW(editor.delete_elements(0, 3), std::range_error);
	EXPECT_TRUE(editor.doc().data().empty());
}

TEST(Delete, NonEmptyString)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.delete_elements(0, 3);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "tor");
}

TEST(Delete, Whole)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.delete_elements(0, editor.doc().data().size());
	EXPECT_TRUE(editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "");
}

TEST(Delete, OutOfRangeIdx2)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.delete_elements(0, 1000);
	EXPECT_TRUE(editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "");
}

TEST(Delete, OutOfRangeIdx1)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	EXPECT_THROW(editor.delete_elements(1009, 1010), std::range_error);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Delete, WrongIndexes)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	EXPECT_THROW(editor.delete_elements(2, 1), std::invalid_argument);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Delete, Middle)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.delete_elements(1, 5);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "er");
}

TEST(UndoRedo, CopyPast)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.copy(0, 10);
	EXPECT_TRUE(!editor.doc().data().empty());
	editor.paste(0);
	EXPECT_EQ(editor.doc().data(), "editoreditor");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "editoreditor");
}

TEST(UndoRedo, InsertDelete)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.insert("hello", 0);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	editor.delete_elements(0, 5);
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(UndoRedo, Insert)
{
	std::istringstream input{ "editor" };
	string_editor editor{ input };
	editor.insert("hello", 0);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "helloeditor");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "editor");

	editor.insert("4", 0);
	editor.insert("3", 0);
	editor.insert("2", 0);
	editor.insert("1", 0);
	editor.undo();
	editor.undo();
	editor.undo();
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "editor");
	editor.redo();
	editor.redo();
	editor.redo();
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "1234editor");
	
}

TEST(UndoRedo, Delete)
{
	std::istringstream input{ "123456789delete" };
	string_editor editor{ input };
	std::string buf{"123456789delete"};
	editor.delete_elements(0, 1);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "23456789delete");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "123456789delete");
	editor.redo();
	EXPECT_EQ(editor.doc().data(), "23456789delete");
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "123456789delete");
	for(auto i = 0; i < 9; ++i) {
		editor.delete_elements(0, 1);
		buf.erase(0, 1);
		EXPECT_EQ(editor.doc().data(), buf);
	}
	EXPECT_EQ(editor.doc().data(), "delete");
	buf.clear();
	for (auto i = 0; i < 9; ++i) {
		editor.undo();
		buf.insert(0, std::to_string(9 - i));
		EXPECT_EQ(editor.doc().data(), buf + "delete");
	}
	EXPECT_EQ(editor.doc().data(), "123456789delete");
	for (auto i = 0; i < 9; ++i) {
		editor.redo();
		buf.erase(0, 1);
		EXPECT_EQ(editor.doc().data(), buf + "delete");
	}
	EXPECT_EQ(editor.doc().data(), "delete");
	for (auto i = 0; i < 8; ++i) {
		editor.undo();
		editor.undo();
		editor.redo();
		buf.insert(0, std::to_string(9 - i));
		EXPECT_EQ(editor.doc().data(), buf + "delete");
	}
	editor.undo();
	EXPECT_EQ(editor.doc().data(), "123456789delete");
}


TEST(Parser, CopyPaste)
{
	logger log{ console_log{} };
	std::istringstream cmd{ "copy 0 1\npaste 0" };
	
	string_editor editor{ "editor"  };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));

	EXPECT_EQ(editor.doc().data(), "eeditor");
}

TEST(Parser, Insert)
{
	logger log{ console_log{} };
	std::istringstream cmd{ "insert hello 0\n" };

	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));

	EXPECT_EQ(editor.doc().data(), "helloeditor");
}

TEST(Parser, Delete)
{
	logger log{ console_log{} };
	std::istringstream cmd{ "delete 0 100\n" };

	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_TRUE(editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "");
}

TEST(Parser, Undo)
{
	logger log{ console_log{} };
	std::istringstream cmd{ "delete 0 100\nundo\n" };

	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, Redo)
{
	logger log{ console_log{} };
	std::istringstream cmd{ "insert hello 0\ninsert hello 0\nundo\nundo\nredo\nredo\n" };

	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "hellohelloeditor");
}

TEST(Parser, UndoRedo)
{
	logger log{ console_log{} };
	std::istringstream cmd{ "insert hello 0\ninsert hello 0\nundo\nredo\nundo\nredo\n" };

	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	parser.parse(editor, log);
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "hellohelloeditor");
}

TEST(Parser, ManyDifferentCommands)
{
	logger log{ console_log{} };
	std::istringstream cmd{ "insert hello 0\ncopy 0 5\npaste 0\ndelete 0 5\nundo\nredo\n"};

	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "helloeditor");
}

TEST(Parser, InvalidCmdInsert)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "insert hello hello\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: insert hello hello\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}


TEST(Parser, InvalidCmdDelete)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "delete hello\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: delete hello\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, DeleteWrongindexes)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "delete 3 2\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: delete exception: wrong index order\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, DeleteOutOfRange)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "delete 12 14\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: delete exception: out of range delete\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, InvalidCmdCopy)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "copy hello\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: copy hello\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, CopyWrongindexes)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "copy 3 2\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: copy exception: wrong index order\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, CopyOutOfRange)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "copy 12 14\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: copy exception: out of range copy\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, InvalidCmdPaste)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "paste hello\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: paste hello\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, InvalidCmdRandomLetters)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "wefwewe\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: wefwewe\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, InvalidCmdRandomNumbers)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "12345\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: 12345\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, InvalidCmdAlmostCopy)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "copyy 1 2\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: copyy 1 2\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

TEST(Parser, InvalidCmdAlmostPaste)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "copy 1 2\n pastee 1 2\ninsert hello 0\n\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: pastee 1 2\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "helloeditor");
}


TEST(Parser, InvalidCmdAlmostInsert)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "inserte 1 2\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: inserte 1 2\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}


TEST(Parser, InvalidCmd10)
{
	std::ostringstream err;
	logger log{ file_log{err} };

	std::istringstream cmd{ "deletee 1 2\n" };
	string_editor editor{ "editor" };
	command_parser parser{ cmd };
	EXPECT_NO_THROW(parser.parse(editor, log));
	EXPECT_EQ(err.str(), "ERROR: invalid command: deletee 1 2\n");
	EXPECT_TRUE(!editor.doc().data().empty());
	EXPECT_EQ(editor.doc().data(), "editor");
}

