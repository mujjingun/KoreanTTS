#include "../consonants.h"
#include "../vocal.h"

////////////////////////////////////////
//
//  ㅁ : Mieum
//
////////////////////////////////////////

Mieum::Mieum()
: Consonant(240, 1125, 0.05, 0.6)
{
    filt1 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt2 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt3 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
}

void Mieum::init(const Vowel & next_vowel)
{
    filt1.reset();
    filt2.reset();
    filt3.reset();
}

fpoint Mieum::gen_sample(fpoint progress_sec, fpoint voice)
{
    fpoint result = 0;

    voice *= linear(0, 0.05, progress_sec, 0, duration);

    fpoint f1, f2, f3;

    f1 = 246;
    f2 = linear(1661, 1400, progress_sec, 0, duration);
    f3 = 2626;

    filt1.setF0(f1);
    filt1.setQ(f1 / 80);
    filt1.recalculateCoeffs();

    filt2.setF0(f2);
    filt2.setQ(f2 / 80);
    filt2.recalculateCoeffs();

    filt3.setF0(f3);
    filt3.setQ(f3 / 80);
    filt3.recalculateCoeffs();

    result += filt1.process(voice, Biquad::LEFT);
    result += filt2.process(voice, Biquad::LEFT) * 0.01;
    result += filt3.process(voice, Biquad::LEFT) * 0.01;

    return result;
}
