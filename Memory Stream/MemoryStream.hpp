#pragma once
#include <cstdint>
#include <cstdio>

enum OpenMode : uint8_t
{
	Read = 0x01, // разрешает операции чтения из потока
	Write = 0x02, // разрешает операции записи в поток
};

enum IOState : uint8_t
{
	Good = 0x00, // нет ошибок
	EoF = 0x01, // при чтении достигнут конец потока
	Bad = 0x02, // ошибка ввода-вывода
};

enum SeekDirection : int
{
	Begin = SEEK_SET,
	Current = SEEK_CUR,
	End = SEEK_END,
};


class MemoryStream final
{
public:
	MemoryStream();
	explicit MemoryStream(const char* name, uint8_t mode);
	~MemoryStream();

public:
	//объекты класса stream нельзя копировать:
	MemoryStream(const MemoryStream&) = delete;
	MemoryStream& operator=(const MemoryStream&) = delete;

public:
	const char* str() const;
	void str(const char* s);
	void swap(MemoryStream& memory_stream);

public:
	uint8_t state() const;
	void clearState();

public:
	bool good() const;
	bool eof() const;
	bool bad() const;

public:
	size_t count() const;

public:
	MemoryStream& ignore(size_t n = 1, int delim = EOF);

public:
	MemoryStream& read(void* buffer, size_t size);
	MemoryStream& write(const void* buffer, size_t size);

public:
	int get();
	MemoryStream& get(char& c);
	MemoryStream& get(char* s, size_t len);
	MemoryStream& put(char c);
	MemoryStream& put(const char* s, size_t len);

public:
	MemoryStream& print(const char* format, ...);

public:
	fpos_t pos() const;
	MemoryStream& pos(fpos_t p);
	long int tell() const;
	MemoryStream& seek(long int offset, SeekDirection dir);

private:
	char* stream;
	size_t size;
	size_t capacity;
	uint8_t _state;
	uint8_t mode;
	size_t prev_count;
	fpos_t _pos;
};
