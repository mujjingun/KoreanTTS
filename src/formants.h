#ifndef FORMANTS_H_INCLUDED
#define FORMANTS_H_INCLUDED

#include <memory>
#include "main.h"
#include "biquad.h"

struct Vowel
{
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;
};

class Consonant
{
public:
    fpoint f1, f2;
    fpoint duration, transition;

    Consonant(fpoint f1, fpoint f2, fpoint duration, fpoint transition)
    : f1(f1), f2(f2), duration(duration), transition(transition){};

    Consonant(): Consonant(0, 0, 0.1, 0.4) {};

    virtual ~Consonant(){}

    Consonant(const Consonant&) = delete;
    Consonant& operator=(const Consonant&) = delete;

    virtual void init(const Vowel & next_vowel){};
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice){return 0;};
};

class Consonants
{
public:
    Consonants();
    Consonant* get(int i);
private:
    std::unique_ptr<Consonant> consonant_formants[20];
};

const Vowel & get_vowel_formants(int i);
fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex);
fpoint clamp(fpoint s, fpoint e, fpoint x);

#endif // FORMANTS_H_INCLUDED
