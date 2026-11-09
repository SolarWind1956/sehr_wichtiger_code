#include <string>
using std::string;

#include <iostream>
using std::cout; using std::endl;

#ifndef INITIALIZER_LIST
#include <iterator>
using std::begin; using std::end;
#endif

#include "variadic_template.h"

int main()
{
	//test_variadic_temple_func();
	test_remove_reference();
	return 0;
}
