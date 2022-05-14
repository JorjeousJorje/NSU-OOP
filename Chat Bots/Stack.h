#pragma once
#include <stdio.h>
#include <stdbool.h>

#define INIT_SIZE 20
static const size_t INVALID = ~((size_t)0);

typedef struct {
	size_t len;
	size_t capacity;
	void** data;
} Stack;

void* stack_create();
void stack_destroy(Stack* stack, void(*dtor)(void*));
void stack_clear(Stack* stack, void(*dtor)(void*));
void stack_remove(Stack* stack, void* data, void(*dtor)(void*));

size_t stack_push(Stack* pstack, void* data);
bool stack_index_srch(Stack* stack, size_t chat_position, void* data);
