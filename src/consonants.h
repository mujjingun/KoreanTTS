#ifndef __consonants_h__
#define __consonants_h__

#include "main.h"
#include "formants.h"

// 0. ㄱ
class Giyeok : public Consonant
{
public:
    Giyeok();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice);

private:
    fpoint closure_duration;
    fpoint s1, s2, s3;
    fpoint e1, e2, e3;

    fpoint aspiration_level;
    fpoint noise_level;
    fpoint burst_level;

    bool occluding, bursting1, bursting2, aspirating;

    Biquad burst_filt;
    Biquad filt1, filt2, filt3;
};

// 2. ㄴ
class Nieun : public Consonant
{
public:
    Nieun();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice);

private:
    Biquad filt1, filt2, filt3;
};

// 3. ㄷ
class Digeut : public Consonant
{
public:
    Digeut();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice);

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

// 6. ㅁ
class Mieum : public Consonant
{
public:
    Mieum();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice);

private:
    Biquad filt1, filt2, filt3;
};

// 7. ㅂ
class Bieup : public Consonant
{
public:
    Bieup();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice);

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

// 8. ㅃ
class Ssangbieup : public Consonant
{
public:
    Ssangbieup();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice);

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

// 9. ㅅ
class Siot : public Consonant
{
public:
    Siot();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice);

private:
    Biquad hpf;
};

// 17.
class Pieup : public Consonant
{
public:
    Pieup();
    virtual void init(const Vowel & next_vowel);
    virtual fpoint gen_sample(fpoint progress_sec, fpoint voice);

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
