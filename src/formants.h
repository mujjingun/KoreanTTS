#ifndef FORMANTS_H_INCLUDED
#define FORMANTS_H_INCLUDED

#include "main.h"

struct Vowel
{
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;
};

struct Consonant
{
    fpoint f1, f2, f3;
    fpoint vot;
};

Vowel get_vowel_formants(int i);
fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex);
fpoint clamp(fpoint s, fpoint e, fpoint x);

#endif // FORMANTS_H_INCLUDED
