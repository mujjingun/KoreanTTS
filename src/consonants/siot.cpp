#include "../consonants.h"
#include "../vocal.h"

////////////////////////////////////////
//
//  ㅅ : Siot
//
////////////////////////////////////////

Siot::Siot()
: Consonant(20, 1800, 0.07)
{
    lpf = Biquad(Biquad::LPF, Fs);
}

void Siot::init(const Vowel & next_vowel)
{
    lpf.setF0(3500);
    lpf.setQ(1);
    lpf.recalculateCoeffs();
}

fpoint Siot::gen_sample(fpoint progress_sec)
{
    fpoint result = 0;

    fpoint in = osc_noise() * 0.1;

    result += lpf.process(in, Biquad::LEFT);

    return result;
}

