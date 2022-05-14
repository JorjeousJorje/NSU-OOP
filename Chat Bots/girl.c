#include <string.h>
#include "Chat_History.h"
#include "Stack.h"
#include "User.h"
#include "Girl.h"
#include "new.h"


#define pos(p) (((struct User *)(p)) -> position)
#define name(p) (((struct User *)(p)) -> name)
#define first(p) (((struct User *)(p)) -> frst_speak)

static void* Girl_ctor(void* _self, va_list* app)
{
	struct Girl* self = ((const struct Class*)User)->ctor(_self, app);
	return self;
}

static void Girl_dtor(void* _self)
{
	((const struct Class*)User)->dtor(_self);
}

static void Girl_type(void* _self, Stack* stack, const Phrases phrases)
{
	((const struct Class*)User)->Type(_self, stack, phrases);
	struct Girl* self = _self;
	bool current_frst_speech = (pos(self) == 0) ? true : false;
	if (current_frst_speech) {
		pos(self) = say_phrase(stack, self, phrases.dress_phrase);
	}
}

static const struct Class _Girl = {
	sizeof(struct Girl),     
	Girl_ctor,                
	Girl_dtor,                 
	Girl_type                
};
const void* Girl = &_Girl;