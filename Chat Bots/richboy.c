#include "Chat_History.h"
#include "Stack.h"
#include "RichBoy.h"
#include "new.h"

#define pos(p) (((struct User *)(p)) -> position)
#define name(p) (((struct User *)(p)) -> name)

static void* RichBoy_ctor(void* _self, va_list* app)
{
	struct RichBoy* self = ((const struct Class*)Boy)->ctor(_self, app);
	return self;
}

static void RichBoy_dtor(void* _self)
{
	((const struct Class*)User)->dtor(_self);
}

static void RichBoy_type(void* _self, Stack* stack, const Phrases phrases)
{
	((const struct Class*)User)->Type(_self, stack, phrases);
	struct RichBoy* self = _self;
	bool current_frst_speech = (pos(self) == 0) ? true : false;
	if (current_frst_speech) {
		say_phrase(stack, self, phrases.supercar);
	}
	if (stack_index_srch(stack, pos(self) + 1, phrases.dress_trigger)) {
		pos(self) = say_phrase(stack, self, phrases.richest_boy);
	}
}

static const struct Class _RichBoy = {
	sizeof(struct RichBoy),      
	RichBoy_ctor,                
	RichBoy_dtor,                 
	RichBoy_type                
};
const void* RichBoy = &_RichBoy;