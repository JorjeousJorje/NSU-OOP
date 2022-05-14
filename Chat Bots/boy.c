#include <string.h>
#include "Chat_History.h"
#include "Stack.h"
#include "User.h"
#include "Boy.h"
#include "New.h"

#define pos(p) (((struct User *)(p)) -> position)
#define name(p) (((struct User *)(p)) -> name)
#define first(p) (((struct User *)(p)) -> frst_speak)

static void* Boy_ctor(void* _self, va_list* app)
{
	struct Boy* self = ((const struct Class*)User)->ctor(_self, app);
	return self;
}

static void Boy_dtor(void* _self)
{
	((const struct Class*)User)->dtor(_self);
}

static void Boy_type(void* _self, Stack* stack, const Phrases phrases)
{
	((const struct Class*)User)->Type(_self, stack, phrases);
	struct Boy* self = _self;
	if (stack_index_srch(stack, pos(self) + 1, phrases.dress_trigger)) {
		pos(self) = say_phrase(stack, self, phrases.rat_phrase);
	}
}

static const struct Class _Boy = {
	sizeof(struct Boy),
	Boy_ctor,
	Boy_dtor,
	Boy_type
};
const void* Boy = &_Boy;