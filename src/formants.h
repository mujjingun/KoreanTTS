#ifndef FORMANTS_H_INCLUDED
#define FORMANTS_H_INCLUDED

#include "main.h"
#include "biquad.h"

class FormantFilters
{
public:
    Biquad f1;
    Biquad f2;
    Biquad f3;
    Biquad f4;
    Biquad f5;

    void reset()
    {
        f1.reset();
        f2.reset();
        f3.reset();
        f4.reset();
        f5.reset();
    }
};

struct Vowel
{
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;
};

class Consonant
{
public:
    fpoint f1, f2;
    fpoint duration;

    Consonant(fpoint f1, fpoint f2, fpoint duration);
    Consonant(): Consonant(0, 0, 0.5) {};

    virtual ~Consonant(){}

    Consonant(const Consonant&) = delete;
    Consonant& operator=(const Consonant&) = delete;

    virtual void init(FormantFilters & filt, const Vowel & next_vowel){};
    virtual fpoint gen_sample(fpoint progress_sec){return 0;};

protected:
    FormantFilters *filt;
};

const Vowel & get_vowel_formants(int i);
Consonant * get_consonant_formants(int i);
fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex);
fpoint clamp(fpoint s, fpoint e, fpoint x);

extern Consonant* null_consonant;

#endif // FORMANTS_H_INCLUDED
