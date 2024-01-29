#include "point.h"
#include <iostream>
void func_point(int a, int* b)
{

	std::cout << "func_point:" << " a:" << a << " b:" << b;
	*b = 8;
	std::cout << "a:" << a << " *a:" << *b << "\n";
}
