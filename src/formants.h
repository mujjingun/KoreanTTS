#ifndef FORMANTS_H_INCLUDED
#define FORMANTS_H_INCLUDED

#include "main.h"

class Vowel
{
public:
    char type;

    Vowel(char type): type(type) {}
    virtual ~Vowel() {}

};

class Monophthong : public Vowel
{
public:
    fpoint f1, f2, f3;

    Monophthong(fpoint f1, fpoint f2, fpoint f3): Vowel('m'), f1(f1), f2(f2), f3(f3) {}
    virtual ~Monophthong() {}

};

class Diphthong : public Vowel
{
public:
    int a, b;

    Diphthong(int a, int b): Vowel('d'), a(a), b(b) {}
    virtual ~Diphthong() {}

};

Vowel* get_formant(int i);
fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex);
fpoint clamp(fpoint s, fpoint e, fpoint x);

#endif // FORMANTS_H_INCLUDED
