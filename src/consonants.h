#ifndef __consonants_h__
#define __consonants_h__

#include "main.h"
#include "formants.h"

class Bieup : public Consonant
{
public:
    Bieup();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec);

private:
    fpoint closure_duration;
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;

    fpoint aspiration_level;
    fpoint noise_level;
    fpoint burst_level;

    bool occluding, bursting, aspirating;

    Biquad burst_filt;
    Biquad filt1, filt2, filt3;
};

class Ssangbieup : public Consonant
{
public:
    Ssangbieup();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec);

private:
    fpoint closure_duration;
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;

    fpoint aspiration_level;
    fpoint noise_level;
    fpoint burst_level;

    bool occluding, bursting, aspirating;

    Biquad burst_filt;
    Biquad filt1, filt2, filt3;
};

class Siot : public Consonant
{
public:
    Siot();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec);

private:
    Biquad lpf;
};

class Pieup : public Consonant
{
public:
    Pieup();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec);

private:
    fpoint closure_duration;
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;

    fpoint aspiration_level;
    fpoint noise_level;
    fpoint burst_level;

    bool occluding, bursting, aspirating;

    Biquad burst_filt;
    Biquad filt1, filt2, filt3;
};

#endif
