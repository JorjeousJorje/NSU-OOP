#include "MemoryStream.hpp"
#include <iostream>
#include <crtdbg.h>
#include <cassert>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf
#include <numeric> 
#define _CRTDBG_MAP_ALLOC


void loop()
{
	const char* content = "goo678o6";
	const char* str = "Sampletext\t\n Sample text\t\nkekmekluldSample text\t\n Sample text\t\nkekmekluld";
	MemoryStream f(str, Read | Write);
	char ptr[100000];
	f.read(ptr, 10000);
	//f.write(str, strlen(str));
	std::iota(std::begin(ptr), std::end(ptr), 'A');
	size_t counter = 0;

	//while (!f.eof()) {

	//	f.read(ptr, 10);
	//	assert(std::string(ptr, ptr + f.count()) == std::string(str + counter, str + counter + f.count()));
	//	counter += f.count();
	//}

	//assert(counter == strlen(str));
}


int main(){
	loop();
	assert(!_CrtDumpMemoryLeaks());
	return 0;
}