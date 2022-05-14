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
#include "chat.h"

int main(int argc, char** argv)
{
	size_t res = create_chat(10);
		assert(!_CrtDumpMemoryLeaks());
	return res;

}