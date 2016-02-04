#ifndef ENCODING_H_INCLUDED
#define ENCODING_H_INCLUDED

#include "main.h"
#include <string>

std::wstring console_input();

std::vector<int> decompose( std::wstring const & str );

#endif // ENCODING_H_INCLUDED
