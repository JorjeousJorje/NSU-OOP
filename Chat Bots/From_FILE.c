#define _CRT_SECURE_NO_WARNINGS
#include "From_FILE.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "ctype.h"
#include "New.h"
#include "Boy.h"
#include "Girl.h"
#include "CowardGirl.h"
#include "RichBoy.h"
#include "Stack.h"
#define MAX_NAME_LEN 100

static void make_symbol_small(char* name)
{
	if (name) {
		for (size_t i = 0; name[i] != '\0'; i++) {
			int int_symbol = tolower(name[i]);
			char ch_symbol = int_symbol;
			name[i] = ch_symbol;
		}
	}
}
static void* open_file(const char* filename)
{
	FILE* file;
	fopen_s(&file, filename, "r");
	if (!file || fgetc(file) == EOF) {
		return NULL;
	}
	rewind(file);
	return file;
}
static void* create_bot(char* name, char* role)
{
	if (name && role)
	{
		make_symbol_small(role);
		if (!strcmp(role, "user")) {
			return new(User, name);
		}
		else if (!strcmp(role, "boy")) {
			return new(Boy, name);
		}
		else if (!strcmp(role, "girl")) {
			return new(Girl, name);
		}
		else if (!strcmp(role, "richboy")) {
			return new(RichBoy, name);
		}
		else if (!strcmp(role, "cowardgirl")) {
			return new(CowardGirl, name);
		}
	}

	return NULL;
}

void* read_file(Stack* stack, const char* filename)
{
	FILE* file = open_file(filename);
	if (file) {
		char buffer[MAX_NAME_LEN] = { 0 };
		while (fgets(buffer, MAX_NAME_LEN, file) != NULL) {

			char name[MAX_NAME_LEN] = { 0 };
			char role[MAX_NAME_LEN] = { 0 };
			if (sscanf_s(buffer, "%s%s", name, MAX_NAME_LEN, role, MAX_NAME_LEN) == 2) {
				void* user = create_bot(name, role);
				if (user) {
					stack_push(stack, user);
				}
			}
		}
		fclose(file);
		return stack;
	}
		return NULL;
}
