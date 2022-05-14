#include <stdlib.h>
#include <stdio.h>
#include "Chat_History.h"
#include "Stack.h"
#include "CowardGirl.h"
#include "new.h"

#define pos(p) (((struct User *)(p)) -> position)
#define name(p) (((struct User *)(p)) -> name)

static void* CowardGirl_ctor(void* _self, va_list* app)
{
	struct CowardGirl* self = ((const struct Class*)Girl)->ctor(_self, app);
	return self;
}

static void CowardGirl_dtor(void* _self)
{
	((const struct Class*)User)->dtor(_self);
}

static void CowardGirl_type(void* _self, Stack* stack, const Phrases phrases)
{
	((const struct Class*)Girl)->Type(_self, stack, phrases);
	struct CowardGirl* self = _self;
	if (stack_index_srch(stack, pos(self) + 1, phrases.rat_trigger)) {
		pos(self) = say_phrase(stack, self, phrases.rats_phrase);
		pos(self) = stack_push(stack, phrases.rat_trigger);
	}
}

static const struct Class _CowardGirl = {
	sizeof(struct CowardGirl),     
	CowardGirl_ctor,                
	CowardGirl_dtor,               
	CowardGirl_type                
};
const void* CowardGirl = &_CowardGirl;