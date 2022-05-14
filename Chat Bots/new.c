#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include "New.h"

 void* new(void* _class, ...)
 {
	struct Class* class = _class;
	void* p = calloc(1, class->size);
	if (!p) {
		return NULL;
	}

	*(struct Class**)p = class;

	if (class->ctor) {
		va_list ap;
		va_start(ap, _class);
		p = class->ctor(p, &ap);
		va_end(ap);
	}

	return p;
}

void delete(void* self)
{
	struct Class** cp = self;
	if (self && *cp && (*cp)->dtor) {
		(*cp)->dtor(self);
	}
	free(self);
	self = NULL;
}

void Type(void* self, Stack* history, const Phrases default_phrases)
{
	struct Class* const* cp = self;
	if (self && *cp && (*cp)->Type) {
		(*cp)->Type(self, history, default_phrases);
	}
}