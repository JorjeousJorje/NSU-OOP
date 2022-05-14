#define _CRTDBG_MAP_ALLOC
#include <string.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>
#include "Chat_History.h"
#include "Stack.h"
#include "User.h"
#include "new.h"

#define name(p) (((struct User *)(p)) -> name)

static char* str_cpy(char* string)
{
	size_t count = strlen(string) + 1;
	char* newstr = malloc(count);
	if (!newstr) {
		return NULL;
	}
	strcpy_s(newstr, count, string);
	return newstr;
}

size_t say_phrase(Stack* stack, void* self, char* phrase)
{
	printf("%s: %s\n", name(self), phrase);
	return stack_push(stack, phrase);
}

static void* User_ctor(void* _self, va_list* app)
{
	struct User* self = _self;
	char* name = va_arg(*app, char*);
	self->name = str_cpy(name);
	self->frst_speak = true;
	self->position = 0;
	return self;
}

static void User_dtor(void* _self) 
{
	struct User* self = _self;
	if (self) {
		free(self->name);
		self->name = NULL;
	}
}

static void User_Type(void* _self, Stack* stack, const Phrases phrases)
{
	struct User* self = _self;
	if (self->frst_speak) {
		say_phrase(stack, self, phrases.hey_phrase);
		self->frst_speak = false;
	}
}

static const struct Class _User = {
	sizeof(struct User),     
	User_ctor,               
	User_dtor,                  
	User_Type               
};

const void* User = &_User;