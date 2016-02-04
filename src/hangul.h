#ifndef HANGUL_H_INCLUDED
#define HANGUL_H_INCLUDED

#include "main.h"

std::vector<int> to_phonetic(std::vector<int> str);
std::vector<int> to_narrow(std::vector<int> str);

#endif // HANGUL_H_INCLUDED
