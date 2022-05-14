#include "chat.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include "From_FILE.h"
#include "New.h"
#include "Boy.h"
#include "Girl.h"
#include "CowardGirl.h"
#include "Chat_History.h"
#include "RichBoy.h"
#include "Stack.h"
#include <stdio.h>

void destroy_chat(Stack* bots_array, Stack* history)
{
	stack_destroy(bots_array, delete);
	stack_destroy(history, 0);
}

size_t create_chat(size_t num_of_cycles)
{
	Stack* history = stack_create();
	if (!history) {
		return INVALID;
	}

	Stack* bots_array = read_file(stack_create(), "Users.txt");
	if (!bots_array) {
		return INVALID;
	}

	for (size_t i = 0; i < num_of_cycles; i++)
	{
		size_t j;
		for (j = 0; j < bots_array->len; j++)
		{
			Type(bots_array->data[j], history, phrases);
		}
		if (j + 1 == bots_array->len)
		{
			j = 0;
		}
	}
	destroy_chat(bots_array, history);
	return 0;
}
