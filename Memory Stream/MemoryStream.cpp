#include "MemoryStream.hpp"
#include <cstdarg>
#include <cstring>
#include <utility>
#include <algorithm>

namespace
{
	constexpr size_t INIT_CAPACITY = 20;
	constexpr size_t INCREASE_NUM = 10;
	char* increase(char* stream, const size_t& capacity, size_t& old_capacity)
	{
		const auto new_capacity = capacity;
		auto* new_stream = stream;
		
		stream = new char[new_capacity];
		if (memcpy_s(stream, new_capacity, new_stream, old_capacity) != 0) {
			return nullptr;
		};
		old_capacity = new_capacity;

		delete[] new_stream;

		return stream;
	}
	bool is_readable(const MemoryStream& stream, const uint8_t mode, const size_t size, const size_t object_size = 1)
	{
		return (mode & Read) && !stream.bad() && !stream.eof() && size != 0 && object_size != 0;
	}
	bool is_writable(const MemoryStream& stream, const uint8_t mode)
	{
		return (mode & Write) && !stream.bad();
	}
}

MemoryStream::MemoryStream()
{
	stream = new char[INIT_CAPACITY];
	size = 0;
	capacity = INIT_CAPACITY;
	_state = Good;
	mode = Read | Write;
	prev_count = 0;
	_pos = 0;
}
MemoryStream::MemoryStream(const char* name, uint8_t _mode) : MemoryStream()
{
	mode = _mode;
	
	if (name == nullptr) {
		return;
	}
	
	size = strlen(name);
	capacity = std::max(size + 1, INIT_CAPACITY);
	if (capacity > INIT_CAPACITY) {
		delete[] stream;
		stream = new char[capacity];
	}
	_state = Good;
	prev_count = 0;
	_pos = 0;
	memmove_s(stream, capacity, name, strlen(name));
}
MemoryStream::~MemoryStream()
{
	delete[] stream;
}

const char* MemoryStream::str() const
{
	return stream;
}
void MemoryStream::str(const char* s)
{
	_pos = 0;
	size = strlen(s);
	const auto new_capacity = std::max(size + 1, capacity);
	if(new_capacity > capacity) {
		stream = increase(stream, new_capacity, capacity);
	}
	capacity = new_capacity;
	
	memmove_s(stream, capacity, s, strlen(s));
}

void MemoryStream::swap(MemoryStream& memory_stream)
{
	std::swap(size, memory_stream.size);
	std::swap(capacity, memory_stream.capacity);
	std::swap(_pos, memory_stream._pos);
	std::swap(_state, memory_stream._state);
	std::swap(prev_count, memory_stream.prev_count);
	std::swap(stream, memory_stream.stream);
}

uint8_t MemoryStream::state() const
{
	return _state;
}
void MemoryStream::clearState()
{
	_state = Good;
}
size_t MemoryStream::count() const
{
	return prev_count; 
}
bool MemoryStream::good() const
{
	return _state == Good;
}
bool MemoryStream::eof() const
{
	return _state == EoF || _state == (EoF | Bad);
}
bool MemoryStream::bad() const
{
	return _state == Bad || _state == (Bad | EoF);
}

MemoryStream& MemoryStream::ignore(size_t n, int delim)
{
	prev_count = 0;
	if (is_readable(*this, mode, size, n)) {
		const auto prev_pos = _pos;
		if (_pos == size) {
			_state |= EoF;
			return *this;
		}
		const auto is_end = std::min(size - _pos, n);
		for (auto i = _pos; i < is_end; i++) {
			if (get() == delim) {
				++_pos;
				prev_count = _pos - prev_pos - 1;
				return *this;
			}
		}
		prev_count = is_end;
		if (_pos == size) {
			_state |= EoF;
			return *this;
		}
	}
	return *this;
}
MemoryStream& MemoryStream::read(void* buffer, size_t _size)
{
	prev_count = 0;
	if (buffer && is_readable(*this, mode, size, _size)) {
		if (_pos == size) {
			_state |= EoF;
			return *this;
		}
		const auto is_end = std::min(size - _pos, _size);
		char* buf = static_cast<char*>(buffer);
		get(buf, is_end);
	}
	return *this;
}

MemoryStream& MemoryStream::write(const void* buffer, size_t _size)
{
	prev_count = 0;
	if (_size != 0 && buffer && is_writable(*this,mode)) {
		const char* buf = static_cast<const char*>(buffer);
		put(buf, _size);
	}
	return *this;
}

int MemoryStream::get()
{
	prev_count = 0;
	if (is_readable(*this, mode, size, 1)) {
		if (_pos == size) {
			_state |= EoF;
			return EOF;
		}
		prev_count = 1;
		++_pos;
		return static_cast<int>(stream[_pos - 1]);
	}
	return EOF;
}
MemoryStream& MemoryStream::get(char& c) 
{
	c = static_cast<char>(get());
	return *this;
}
MemoryStream& MemoryStream::get(char* s, size_t len)
{
	prev_count = 0;
	if (s && is_readable(*this, mode, size, len)) {
		if (_pos == size) {
			_state |= EoF;
			return *this;
		}
		//const auto is_end = (len > size - _pos) ? size - _pos : len;
		const auto is_end = std::min(size - _pos, len);
		if(memmove_s(s, is_end,  (stream + _pos), is_end) != 0) {
			_state = Bad;
			return *this;
		}
		_pos += is_end;
		prev_count = is_end;
	}
	return *this;
}

MemoryStream& MemoryStream::put(char c)
{
	prev_count = 0;
	if (is_writable(*this, mode))
	{
		++_pos;
		if (_pos == capacity) {
			const auto new_capacity = capacity * 2;
			stream = increase(stream, new_capacity, capacity);
			if (!stream) {
				_state = Bad;
				return *this;
			}
			++size;
		}
		stream[_pos - 1] = c;
		prev_count = 1;
	}
	return *this;
}
MemoryStream& MemoryStream::put(const char* s, size_t len)	
{
	prev_count = 0;
	if (len != 0 && s && is_writable(*this, mode)) {
		if(len + _pos > capacity) {
			const auto new_capacity = len + _pos + INCREASE_NUM;
			stream = increase(stream, new_capacity, capacity);
			size = len + _pos;
		}
		if (memmove_s((stream + _pos), capacity, s, len) != 0) {
			_state = Bad;
			return *this;
		}
		_pos += len;
		prev_count = len;
	}	
	return *this;
}

fpos_t MemoryStream::pos() const
{
	return _pos;
}
MemoryStream& MemoryStream::pos(fpos_t p)
{
	const auto prev_pos = _pos;
	_pos = p;
	if (_pos > size || _pos < 0) {
		_state = Bad;
		_pos = prev_pos;
	}
	return *this;
}
long int MemoryStream::tell() const
{
	return static_cast<long int>(pos());
}

MemoryStream& MemoryStream::print(const char* format, ...)
{
	prev_count = 0;
	if (format && is_writable(*this, mode)) {
		va_list arg;
		va_start(arg, format);
		const int num_elems = _vscprintf(format, arg) + 1;
		if (num_elems > 1) {
			char* buf = new char[num_elems];
			vsnprintf(buf, num_elems, format, arg);
			const int real_size = num_elems - 1;
			put(buf, real_size);
			delete[] buf;
		}
		va_end(arg);
	}
	return *this;
}

MemoryStream& MemoryStream::seek(long int offset, SeekDirection dir)
{
	const auto prev_pos = _pos;
	switch (dir) {
	case Begin:
		_pos = offset;
		break;
	case End:
		_pos = size + offset;
		break;
	case Current:
		_pos += offset;
	default:
		break;
	}
	if (_pos > size || _pos < 0) {
		_state = Bad;
		_pos = prev_pos;
	}
	return *this;
}