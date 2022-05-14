#pragma once
#include <stddef.h>
#include <stdarg.h>

struct User {
	const void* class;
	char* name;
	size_t position;
	bool frst_speak;
};

size_t say_phrase(Stack* stack, void* self, char* phrase);
extern const void* User;