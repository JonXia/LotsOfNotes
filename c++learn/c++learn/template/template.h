// test.h
#ifndef __TEST_H__
#define __TEST_H__
//////////////////////////////////////////////////////////////////////////
// template function
template<class T>
T func(T, T B = T(0));

//////////////////////////////////////////////////////////////////////////
// template class
template<class T>
class Test
{
public:
    Test() {}
    // template member function which is defined in test.cpp
    Test(T a);
    ~Test() {}
public:
    // template member variables
    T _a;
};
#endif
