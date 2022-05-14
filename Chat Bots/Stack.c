#define _CRTDBG_MAP_ALLOC
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <crtdbg.h>
#include <assert.h>
#include <memory.h>
static bool increase(Stack* pstack, void* data)
{
	Stack* stack = pstack;
	size_t new_capacity = stack->capacity * 2;
	void* new_array = realloc(stack->data, sizeof(data) * new_capacity);
	if (new_array == NULL) {
		return false;
	}
	stack->data = new_array;
	stack->capacity = new_capacity;
	return true;
}

void* stack_create()
{
	Stack* pstack = (Stack*)malloc(sizeof(Stack));
	if (pstack == NULL) {
		return NULL;
	}
	pstack->len = 0;
	pstack->capacity = 0;
	pstack->data = NULL;

	return pstack;

}

void stack_destroy(Stack* stack, void(*dtor)(void*))
{
	if (!stack) {
		return;
	}
	stack_clear(stack, dtor);
	free(stack->data);
	stack->data = NULL;
	free(stack);
	stack = NULL;
}

void stack_remove(Stack* stack, void* data, void(*dtor)(void*))
{
	if (!data) {
		return;
	}
	if (!stack) {
		return;
	}
	if (!stack->data) {
		return;
	}
	if (dtor) {
		dtor(data);
		stack->len--;
		return;
	}
	stack->len--;
}

void stack_clear(Stack* stack, void(*dtor)(void*))
{
	if (!stack) {
		return;
	}
	if (!stack->data) {
		return;
	}
	for (size_t i = 0; stack->len != 0; i++)
	{
		stack_remove(stack, stack->data[i], dtor);
	}
}

size_t stack_push(Stack* pstack, void* data)
{ 
	if (pstack->capacity == 0) {
		size_t new_capacity = INIT_SIZE;
		pstack->data = calloc(new_capacity, sizeof(data));

		if (pstack->data == NULL) {
			return INVALID;
		}
		else {
			pstack->capacity = new_capacity;
		}
	}
	if(pstack->len == pstack->capacity) {
		if (!increase(pstack, data)) {
			return INVALID;
		}
	}
	assert(pstack->len < pstack->capacity);
	pstack->data[pstack->len] = data;
	pstack->len++;

	return pstack->len - 1;
}

bool stack_index_srch(Stack* stack, size_t index, void* data)
{
	if (!data) {
		return false;
	}

	for (size_t i = index; i < stack->len; i++)
	{
		if (strstr(stack->data[i], data)) {
			return true;
		}
	}
	return false;
}
