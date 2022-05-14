#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "dynArray.h"

typedef struct {
	float d_variable;
} Value;

int main(int argc, char* argv[])
{
	//Создаем динамический массив с элементами типа Value;
	void* darray = darray_create(sizeof(Value));

	assert(0 == darray_count(darray));
	assert(darray_stop(darray) == darray_first(darray));

	//Создаем объект для динамического массива

	/*Value* insertedValue1 = (Value*)darray_add(darray);
	darray_clear(darray, NULL);
	assert(darray_count(darray) == 0);*/
	Value* insertedValue1 = (Value*)darray_add(darray);
	darray_remove(darray,0,0);
	darray_clear(darray,0);
	for (int i = 0; i < 21; ++i) 
	{
		insertedValue1 = (Value*)darray_insert(darray, 0);
	}
	for (int i = 0; i < 21; ++i)
	{
		darray_remove(darray, 0, 0);
	}
	/*darray_clear(darray, NULL);
	assert(darray_count(darray) == 0);
	
	assert(insertedValue1 != NULL);
	assert(darray_count(darray) == 1);
	assert(insertedValue1 == darray_item(darray, 0));*/



	//Value* insertedValue2 = (Value*)darray_insert(darray, 1);
	//assert(insertedValue2 != NULL);
	//assert(darray_count(darray) == 2);
	//assert(insertedValue2 = darray_item(darray, 1));
	//darray_erase(darray, darray_first(darray), NULL);
	//Добавляем новый элемент в динамический массив
	//Value* insertedValue2 = (Value*)darray_add(darray);
	//Value* insertedValue3 = (Value*)darray_add(darray);
	//Value* insertedValue4 = (Value*)darray_insert(darray, 3);

	//Инициализируем добавленный элемент
	//*insertedValue = value;
	//*insertedValue1 = value1;
	//*insertedValue2 = value2;
	//*insertedValue3 = value3;
	////*insertedValue4 = value4;

	//Value* item = (Value*)darray_item(darray, 0);

	//for (size_t i = 0; 8 > i; ++i) 
	//{
	//	assert(item->array[i] == value.array[i]);
	//}

	//assert(fabsf(item->d_variable - value.d_variable) < 1e-10f);
	//assert(NULL == darray_item(darray, 5));

	//assert(darray_next(darray, darray_last(darray)) == darray_stop(darray));

	//assert(darray_prev(darray, darray_first(darray)) == darray_stop(darray));
	//darray_remove(darray, 0, 0);
	//assert(darray_count(darray) == 0);
	darray_destroy(darray, 0);
	//free(value);
	

	assert(_CrtDumpMemoryLeaks() == 0);
	return 0;
}