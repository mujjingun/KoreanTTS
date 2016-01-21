#include "../consonants.h"
#include "../vocal.h"

////////////////////////////////////////
//
//  ㅅ : Siot
//
////////////////////////////////////////

Siot::Siot()
: Consonant(20, 1800, 0.1)
{
    hpf = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
}

void Siot::init(const Vowel & next_vowel)
{
}

fpoint Siot::gen_sample(fpoint progress_sec)
{
    fpoint result = 0;

    fpoint noise_level;

    noise_level = linear(0, 0.2, progress_sec, 0, 0.05);

    fpoint freq = linear(6000, 2000, progress_sec, 0, duration);

    hpf.setF0(freq);
    hpf.setQ(0.1);
    hpf.recalculateCoeffs();

    fpoint in = osc_noise() * noise_level;

    result += hpf.process(in, Biquad::LEFT);

    return result;
}

