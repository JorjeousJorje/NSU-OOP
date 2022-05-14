#include "pch.h"
#include <numeric> 
#include "../MemoryStream/MemoryStream.cpp"


TEST(NotEmptyStream, readWriteMode_readToEoF) {
	const char* content = "Sample text\t\n Sample text\t\n";

	MemoryStream f(content, Read | Write);

	char ptr[10];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	size_t counter = 0;

	while (!f.eof()) {

		EXPECT_EQ(f.state(), Good);
		f.read(ptr, 10);
		EXPECT_EQ(std::string(ptr, ptr + f.count()), std::string(content + counter, content + counter + f.count()));
		counter += f.count();
	}

	EXPECT_EQ(counter, strlen(content));
	EXPECT_EQ(f.state(), EoF);
}
TEST(NotEmptyStream, readWriteMode_WriteSmthToEoF) {
	const char* content = "Sample text\t\n Sample text\t\n";

	MemoryStream f(content, Read | Write);

	char ptr[10];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	size_t counter = 0;

	while (!f.eof()) {

		EXPECT_EQ(f.state(), Good);
		f.read(ptr, 10);
		EXPECT_EQ(std::string(ptr, ptr + f.count()), std::string(content + counter, content + counter + f.count()));
		counter += f.count();
	}

	EXPECT_EQ(counter, strlen(content));
	EXPECT_EQ(f.state(), EoF);

	counter = 0;
	f.write(content, strlen(content));
	counter += f.count();

	const std::string str1(content, content + counter);
	const std::string result_str = str1 + str1;
	EXPECT_EQ(std::string(f.str(), f.str() + 2 * f.count()), result_str);

	EXPECT_EQ(f.pos(), fpos_t(54));
	EXPECT_EQ(f.state(), EoF);
}
TEST(NotEmptyStream, readWriteMode_WriteSmthToBegin) {
	const char* content = "Sample text\t\n Sample text\t\n";

	MemoryStream f(content, Read | Write);
	f.write(content, 10);
	f.seek(0, Begin);

	char ptr[10];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	size_t counter = 0;

	while (!f.eof()) {

		f.read(ptr, 10);
		ASSERT_EQ(std::string(ptr, ptr + f.count()), std::string(content + counter, content + counter + f.count()));
		counter += f.count();
	}
}
TEST(NotEmptyStream, readWriteMode_writeAfterReadBegin) {
	const char* content = "Sample text\t\n Sample text\t\n";

	MemoryStream f(content, Read | Write);

	char ptr[10];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	size_t counter = 0;

	while (!f.eof()) {

		EXPECT_EQ(f.state(), Good);
		f.read(ptr, 10);
		EXPECT_EQ(std::string(ptr, ptr + f.count()), std::string(content + counter, content + counter + f.count()));
		counter += f.count();
	}

	EXPECT_EQ(counter, strlen(content));
	EXPECT_EQ(f.state(), EoF);
	f.seek(0, Begin);
	EXPECT_EQ(f.pos(), 0);

	counter = 0;
	f.write(content, strlen(content));
	counter += f.count();

	EXPECT_EQ(std::string(f.str(), f.str() + f.count()), std::string(content, content + f.count()));

	EXPECT_EQ(f.pos(), strlen(content));
	EXPECT_EQ(f.state(), EoF);
}
TEST(NotEmptyStream, readWriteMode_getHalfOfStream) {
	const char* content = "Sample text\t\n Sample text\t\n";

	MemoryStream f(content, Read | Write);

	char ptr[13];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	size_t counter = 0;

	EXPECT_EQ(f.state(), Good);
	f.get(ptr, 13);
	counter += f.count();
	EXPECT_EQ(std::string(ptr, ptr + f.count()), std::string(content, content + f.count()));
	EXPECT_EQ(f.state(), Good);

	EXPECT_EQ(counter, 13);
	f.seek(0, Begin);
	EXPECT_EQ(f.pos(), 0);
}
TEST(NotEmptyStream, readWriteMode_ignoreToEoF) {
	const char* content = "Sample text";

	MemoryStream f(content, Read | Write);

	EXPECT_TRUE(f.good() == true);
	EXPECT_TRUE(f.eof() == false);
	EXPECT_TRUE(f.bad() == false);
	EXPECT_EQ(f.ignore(1, EOF).count(), 1);
	EXPECT_EQ(f.ignore(10, EOF).count(), 10);
	EXPECT_TRUE(f.state() == Good);
}
TEST(NotEmptyStream, readWriteMode_WriteBeginReadEoF) {
	const char* content = "Sample text\t\n";
	const char* str = "kekaseka";
	MemoryStream f(content, Read | Write);
	EXPECT_EQ(f.pos(), 0);
	EXPECT_EQ(f.write(str, strlen(str)).count(), strlen(str));
	EXPECT_EQ(f.pos(), strlen(str));
	const std::string s1(content + strlen(str), content + strlen(content));
	const std::string s2(str, str + strlen(str));

	const std::string s3 = s2 + s1;
	EXPECT_EQ(std::string(f.str(), f.str() + strlen(content)), s3);
	EXPECT_EQ(f.state(), Good);
	char ptr[13];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	size_t counter = 0;

	f.pos(0);
	while (!f.eof()) {
		EXPECT_EQ(f.state(), Good);
		f.read(ptr, 13);
		EXPECT_EQ(std::string(ptr, ptr + f.count()), std::string(f.str() + counter, f.str() + counter + f.count()));
		counter += f.count();
	}

	EXPECT_EQ(counter, strlen(content));
	EXPECT_EQ(f.pos(), 13);
	EXPECT_EQ(f.state(), EoF);
}
TEST(NotEmptyStream, readWriteMode_Swap) {
	const char* content = "Sample text\t\n";
	const char* str = "kekaseka";
	MemoryStream f(content, Read | Write);
	MemoryStream f1(str, Read | Write);

	const std::string s1(f.str(), f.str() + strlen(content));
	const std::string s2(f1.str(), f1.str() + strlen(str));

	f.swap(f1);
	EXPECT_EQ(std::string(f.str(), f.str() + strlen(str)), s2);
	EXPECT_EQ(std::string(f1.str(), f1.str() + strlen(content)), s1);
}
TEST(NotEmptyStream, SeekCurFar) {
	const char* content = "Sample text\t\n";
	const char* str = "kekaseka";
	MemoryStream f(content, Read | Write);
	EXPECT_EQ(f.write(str, strlen(str)).count(), strlen(str));
	EXPECT_EQ(f.seek(100000, Current).state(), Bad);
}
TEST(NotEmptyStream, PutOneInEoF) {
	const char* content = "Sample text\t\n Sample text\t\n";

	MemoryStream f(content, Read | Write);

	char ptr[10];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	size_t counter = 0;

	while (!f.eof()) {

		EXPECT_EQ(f.state(), Good);
		f.read(ptr, 10);
		EXPECT_EQ(std::string(ptr, ptr + f.count()), std::string(content + counter, content + counter + f.count()));
		counter += f.count();
	}

	EXPECT_EQ(f.state(), EoF);
	f.put('F');
	std::string str1(content, content + counter);
	str1 += 'F';
	EXPECT_EQ(std::string(f.str(), f.str() + counter + 1), str1);
	EXPECT_EQ(f.state(), EoF);
	EXPECT_EQ(f.pos(), 28);
	EXPECT_EQ(f.get(), EOF);
}
TEST(NotEmptyStream, PutOneSomewhere) {
	const char* content = "Sample text\t\n Sample text\t\n";

	MemoryStream f(content, Read | Write);

	char ptr[10];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	f.seek(0, End);
	const fpos_t size = f.pos();
	f.seek(0, Begin);

	while (f.pos() != size) {
		f.put('0');
	}
	ASSERT_EQ(std::string(f.str(), f.str() + strlen(content)), "000000000000000000000000000");
	ASSERT_EQ(f.pos(), strlen(content));
	f.pos(0);
	while (!f.eof()) {
		if (f.pos() == 27) {
			ASSERT_EQ(static_cast<char>(f.get()), EOF);
			break;
		}
		auto c = static_cast<char>(f.get());
		ASSERT_EQ(c, '0');
	}
}
TEST(NotEmptyStream, readWriteMode_WriteSomewhere) {
	const char* content = "Sample text\t\n";
	const char* str = "kekaseka";
	MemoryStream f(content, Read | Write);

	EXPECT_EQ(f.write(str, strlen(str)).count(), strlen(str));
	const std::string s1(content + strlen(str), content + strlen(content));
	const std::string s2(str, str + strlen(str));
	f.pos(0);
	const std::string s3 = s2 + s1;
	EXPECT_EQ(std::string(f.str(), f.str() + strlen(content)), s3);
	EXPECT_EQ(f.state(), Good);
	char ptr[13];
	std::iota(std::begin(ptr), std::end(ptr), 'A');

	size_t counter = 0;

	while (!f.eof()) {
		EXPECT_EQ(f.state(), Good);
		f.read(ptr, 13);
		EXPECT_EQ(std::string(ptr, ptr + f.count()), std::string(f.str() + counter, f.str() + counter + f.count()));
		counter += f.count();
	}

	EXPECT_EQ(counter, strlen(content));
	EXPECT_EQ(f.pos(), 13);
	EXPECT_EQ(f.state(), EoF);
}
TEST(NotEmptyStream, seek_pos_test) {
	const char* content = "SampleText";
	MemoryStream f(content, Read | Write);
	ASSERT_TRUE(f.pos() == 0);
	f.seek(-10, Begin);
	ASSERT_TRUE(f.state() == Bad);
	f.clearState();
	ASSERT_TRUE(f.state() == Good);
	ASSERT_TRUE(f.pos() == 0);
	f.seek(3, Begin);
	f.seek(1000, Begin);
	ASSERT_TRUE(f.state() == Bad);
	f.clearState();
	ASSERT_TRUE(f.state() == Good);
	ASSERT_TRUE(f.pos() == 3);
	f.seek(0, Begin);
	f.seek(10, Begin);
	ASSERT_TRUE(f.pos() == 10);
	f.seek(0, End);
	ASSERT_TRUE(f.pos() == strlen(content));
	f.seek(-10, End);
	ASSERT_TRUE(f.pos() + 10 == strlen(content));
	ASSERT_TRUE(f.pos() == 0);
	f.seek(-10, Current);
	ASSERT_EQ(f.pos(), 0);
	ASSERT_EQ(f.pos() + 10, strlen(content));
	f.pos(f.pos() + 10);
	ASSERT_TRUE(f.pos() == strlen(content));
	ASSERT_TRUE(f.get() == EOF);
}
TEST(NotEmptyStream, ignoreAll) {
	const char* content = "SampleText";
	MemoryStream f(content, Read | Write);
	f.ignore(-1);
	ASSERT_TRUE(f.count() == strlen(content));
	ASSERT_TRUE(f.pos() == strlen(content));
	ASSERT_TRUE(f.eof() == true);
}