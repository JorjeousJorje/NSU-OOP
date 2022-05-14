#pragma once
#include <stddef.h>
#include <stdarg.h>
#include "Stack.h"
#include "Chat_History.h"

//��� ��������� �������(boy, girl � �.�)
struct Class {
	size_t size;
	void* (*ctor)(void* self, va_list* app);
	void (*dtor)(void* self);
	void (*Type)(void* self, void* history, const Phrases default_phrases);
};

void* new(const void* class, ...);
void delete(void* item);
void Type(void* self, Stack* history, const Phrases default_phrases);