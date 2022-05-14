#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dynArray.h"
#define INITIAL_CAPACITY 20

typedef void* Pointer;

typedef struct {
	size_t len;
	size_t capacity;
	size_t itemSize;
	Pointer* mas;

} DynArray;

static bool increase(void* darray)
{
	DynArray* dynarr = darray;
	size_t new_capacity = dynarr->capacity * 2;
	Pointer* new_array = realloc(dynarr->mas, dynarr->itemSize * new_capacity);
	if (new_array == NULL) {
		return false;
	}
	else {
		dynarr->mas = new_array;
		dynarr->capacity = new_capacity;

		return true;
	}
}

static bool decrease(void* darray)
{
	DynArray* dynarr = darray;

	size_t new_capacity = max(dynarr->capacity / 2, INITIAL_CAPACITY);
	if (new_capacity != dynarr->capacity)
	{
		Pointer* new_array = (Pointer*)realloc(dynarr->mas, dynarr->itemSize * new_capacity);
		if (new_array == NULL) {
			return false;
		}
		else {
			dynarr->mas = new_array;
			dynarr->capacity = new_capacity;

			return true;
		}
	}
	return false;
}

size_t darray_stop(const void* darray)
{
	return INVALID;
}

void* darray_create(size_t itemSize)
{
	if (itemSize == 0) {
		return NULL;
	}

	DynArray* darray = (DynArray*)malloc(sizeof(DynArray));
	if (darray == NULL) {
		return NULL;
	}

	darray->itemSize = itemSize;
	darray->capacity = INITIAL_CAPACITY;
	darray->len = 0;

	darray->mas = (Pointer*)malloc(INITIAL_CAPACITY * itemSize);
	if (darray->mas == NULL) {
		free(darray);
		darray = NULL;
		return NULL;
	}

	return darray;
}

void darray_clear(void* darray, void(*destroy)(void*))
{
	if (darray == NULL) {
		return;
	}

	DynArray* dynarr = darray;
	if (dynarr->len == 0) {
		return;
	}
	char* ptr = (char*)dynarr->mas;
	
	if (destroy != NULL)
	{
		for (; dynarr->len > 0; dynarr->len--)
		{
			destroy(ptr + (dynarr->len - 1) * dynarr->itemSize);
		}
	}
	dynarr->len = 0;
}

void darray_destroy(void* darray, void(*destroy)(void*))
{
	if (darray == NULL) {
		return;
	}

	DynArray* dynarr = darray;

	if (dynarr->mas != NULL) {
		darray_clear(dynarr, destroy);
		free(dynarr->mas);
		dynarr->mas = NULL;
	}
	free(dynarr);
	dynarr = NULL;
}

void* darray_init(void* darray, size_t itemSize, void(*destroy)(void*))
{
	if (darray == NULL || itemSize == 0) {
		return NULL;
	}

	DynArray* dynarr = darray;

	darray_clear(dynarr, destroy);
	free(dynarr->mas);
	dynarr->mas = NULL;
	dynarr->itemSize = itemSize;
	dynarr->mas = (Pointer*)malloc(dynarr->itemSize * dynarr->capacity);

	return dynarr;
}

size_t darray_count(const void* darray)
{
	if (darray == NULL) {
		return darray_stop(darray);
	}
	const DynArray* dynarr = darray;

	return dynarr->len;
}

void* darray_item(void* darray, size_t i)
{
	if (darray == NULL) {
		return NULL;
	}
	DynArray* dynarr = darray;

	if (dynarr->len <= i) {
		return NULL;
	}
	if (dynarr->len == 0) {
		return NULL;
	}
	if (dynarr->mas == NULL) {
		return NULL;
	}

	char* ptr = (char*)dynarr->mas;

	return ptr + i * dynarr->itemSize;
}

void* darray_add(void* darray)
{
	DynArray* dynarr = darray;
	if (darray == NULL) {
		return NULL;
	}
	if (dynarr->mas == NULL) {
		return NULL;
	}

	dynarr->len++;

	if (dynarr->len == dynarr->capacity) {
		if (!increase(dynarr)) {
			return NULL;
		}
	}
	
	assert(dynarr->len < dynarr->capacity);

	char* ptr = (char*)dynarr->mas;

	return ptr + (dynarr->len - 1) * dynarr->itemSize;
}

void* darray_insert(void* darray, size_t i)
{
	DynArray* dynarr = darray;
	if (darray == NULL) {
		return NULL;
	}
	if (dynarr->len == i) {
		return darray_add(dynarr);
	}
	if (dynarr->len < i) {
		return NULL;
	}
	if (dynarr->mas == NULL) {
		return NULL;
	}

	dynarr->len++;
	if (dynarr->len == dynarr->capacity) {
		if (!increase(dynarr)) {
			return NULL;
		}
	}
	assert(dynarr->len < dynarr->capacity);

	char* ptr = (char*)dynarr->mas;

	for (size_t k = i; k <= dynarr->len; k++) 
	{
		memmove(ptr + (k + 1) * dynarr->itemSize, ptr + k * dynarr->itemSize, dynarr->itemSize);
	}

	return  ptr + i * dynarr->itemSize;
}

void darray_remove(void* darray, size_t i, void(*destroy)(void*))
{
	DynArray* dynarr = darray;
	if (darray == NULL) {
		return;
	}
	if (dynarr->mas == NULL) {
		return;
	}
	if (dynarr->len == 0) {
		return;
	}
	if (dynarr->len <= i) {
		return;
	}

	char* ptr = (char*)dynarr->mas;

	if (destroy != NULL) {
		destroy(ptr + i * dynarr->itemSize);	
	}

	for (size_t k = i; k < dynarr->len; k++)
	{
		//*(ptr + k * dynarr->itemSize + j) = *(ptr + (k + 1) * dynarr->itemSize + j);
		memmove(ptr + k * dynarr->itemSize, ptr + (k + 1) * dynarr->itemSize, dynarr->itemSize);
	}
	dynarr->len--;

	if ((dynarr->len * 4 <= dynarr->capacity) && dynarr->capacity > INITIAL_CAPACITY) {//dynarr->mas != NULL
		if (!decrease(dynarr)) {
			return;
		}
	}

}

size_t darray_first(const void* darray)
{
	const DynArray* dynarr = darray;
	if (darray == NULL || dynarr->len == 0) {
		return darray_stop(darray);
	}

	char* ptr = (char*)dynarr->mas;

	return (size_t)(ptr);
}

size_t darray_last(const void* darray)
{
	const DynArray* dynarr = darray;
	if (darray == NULL || dynarr->len == 0) {
		return darray_stop(darray);
	}

	char* ptr = (char*)dynarr->mas;

	return (size_t)(ptr + (dynarr->len - 1) * dynarr->itemSize);
}

size_t darray_next(const void* darray, size_t item_id)
{
	const DynArray* dynarr = darray;
	if (darray == NULL || darray_stop(darray) == item_id || dynarr->len == 0 || dynarr->len == 1) {
		return darray_stop(darray);
	}

	char* ptr_item = (char*)item_id;
	char* ptr_last = (char*)dynarr->mas + (dynarr->len - 1) * dynarr->itemSize;

	if (ptr_last == ptr_item) {
		return darray_stop(darray);
	}

	char* ptr = (char*)item_id;

	return (size_t)(ptr + dynarr->itemSize);
}

size_t darray_prev(const void* darray, size_t item_id)
{
	const DynArray* dynarr = darray;
	if (darray == NULL || darray_stop(darray) == item_id || dynarr->len == 0 || dynarr->len == 1) {
		return darray_stop(darray);
	}

	char* item_ptr = (char*)item_id;
	char* ptr_start = (char*)dynarr->mas;

	if (ptr_start == item_ptr) {
		return darray_stop(darray);
	}

	return (size_t)(item_ptr - dynarr->itemSize);
}

void* darray_current(const void* darray, size_t item_id)
{
	const DynArray* dynarr = darray;
	if (darray == NULL || darray_stop(darray) == item_id || dynarr->len == 0) {
		return NULL;
	}

	char* ptr = (char*)item_id;

	return ptr;
	
}

void darray_erase(void* darray, size_t item_id, void(*destroy)(void*))
{
	DynArray* dynarr = darray;
	if (darray == NULL || darray_stop(darray) == item_id || dynarr->len == 0) {
		return;
	}

	char* ptr_required = (char*)item_id;
	char* ptr_start = (char*)dynarr->mas;

	for (size_t i = 0; i < dynarr->len; i++)
	{
		if (ptr_start + i * dynarr->itemSize == ptr_required)
		{
			darray_remove(darray, i, destroy);
			break;
		}
	}
}