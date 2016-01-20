#ifndef __consonants_h__
#define __consonants_h__

#include "main.h"
#include "formants.h"

class Bieup : public Consonant
{
public:
    Bieup();
    virtual void init(FormantFilters & filt, const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec);

private:
    fpoint closure_duration;
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;

    fpoint aspiration_level;
    fpoint noise_level;
    fpoint burst_level;

    bool occluding, bursting, aspirating;

    FormantFilters *filt;

    Biquad burst_filt;
};

class Pieup : public Consonant
{
public:
    Pieup();
    virtual void init(FormantFilters & filt, const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec);

private:
    fpoint closure_duration;
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;

    fpoint aspiration_level;
    fpoint noise_level;
    fpoint burst_level;

    bool occluding, bursting, aspirating;

    FormantFilters *filt;

    Biquad burst_filt;
};

#endif
