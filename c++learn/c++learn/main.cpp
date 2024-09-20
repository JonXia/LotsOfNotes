// main.cpp
#include <iostream>
#include "template/template.h"
#include "point/point.h"

using namespace std;
int main(int argc, char** argv) {
	// test template function
	//cout << func(10) << endl;
	//cout << func(10, 1) << endl;
	//cout << func(10.) << endl;
	cout << func(10., 1.) << endl;

	// test template class
	Test<int> a(13);
	Test<double> b(14);
	//cout << a._a << endl;
	//cout << b._a << endl;

	// test point
	int pb{ 0 };
	func_point(a._a, &pb);
	//cout << pb;

	return EXIT_SUCCESS;
}
