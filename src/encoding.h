#ifndef ENCODING_H_INCLUDED
#define ENCODING_H_INCLUDED

#include <vector>

int console_input ( wchar_t * str, int size );

std::vector<int> decompose( wchar_t * , int );

#endif // ENCODING_H_INCLUDED
