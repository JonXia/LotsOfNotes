// test.cpp
#include "template.h"
//////////////////////////////////////////////////////////////////////////
// define template function
template<class T>
T func(T A, T B) {
    return A + B;
}
// explicitly declare 
#define _EXPLICIT_DEFINE(CT) template CT func(CT,CT)
_EXPLICIT_DEFINE(char);
_EXPLICIT_DEFINE(int);
_EXPLICIT_DEFINE(float);
_EXPLICIT_DEFINE(double);
#undef _EXPLICIT_DEFINE

//////////////////////////////////////////////////////////////////////////
// template member function
template<class T>
Test<T>::Test(T a) {
    this->_a = a;
}
// explicitly declare 
#define _EXPLICIT_DEFINE(CT) template class Test<CT>
_EXPLICIT_DEFINE(char);
_EXPLICIT_DEFINE(int);
_EXPLICIT_DEFINE(float);
_EXPLICIT_DEFINE(double);
#undef _EXPLICIT_DEFINE