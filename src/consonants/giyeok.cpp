#include "../consonants.h"
#include "../vocal.h"

////////////////////////////////////////
//
//  ㄱ : Giyeok
//
////////////////////////////////////////

Giyeok::Giyeok()
: Consonant(20, 3800, 0.02 + 0.053, 0.4)
{
    closure_duration = 0.02;

    burst_filt = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);

    filt1 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt2 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt3 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
}

void Giyeok::init(const Vowel & next_vowel)
{
    s1 = f1;
    s2 = f2;
    s3 = next_vowel.s3;

    e1 = next_vowel.s1;
    e2 = next_vowel.s2;
    e3 = next_vowel.s3;

    aspiration_level = 0.001;

    occluding = true;
    bursting1 = false;
    bursting2 = false;
    aspirating = false;

}

fpoint Giyeok::gen_sample(fpoint progress_sec, fpoint voice)
{
    fpoint result = 0;

    burst_level = 0;

    // burst
    if(occluding && progress_sec > closure_duration)
    {
        occluding = false;
        bursting1 = true;

        burst_level = 0.03;
    }
    // second larger burst
    else if(bursting1 && progress_sec > closure_duration + 0.012)
    {
        bursting1 = false;
        bursting2 = true;

        burst_level = 0.3;
    }
    // aspiration
    else if(progress_sec > closure_duration)
    {
        fpoint f1, f2, f3;

        bursting2 = false;
        aspirating = true;

        burst_level = 0;

        noise_level = linear(0, aspiration_level, progress_sec, closure_duration, duration);

        f1 = 600;
        f2 = 1600;
        f3 = 2000;

        fpoint in = osc_noise() * noise_level;

        filt1.setF0(f1);
        filt1.setQ(f1 / 200);
        filt1.recalculateCoeffs();

        filt2.setF0(f2);
        filt2.setQ(f2 / 200);
        filt2.recalculateCoeffs();

        filt3.setF0(f3);
        filt3.setQ(f3 / 80);
        filt3.recalculateCoeffs();

        result += filt1.process(in, Biquad::LEFT) * 2;
        result += filt2.process(in, Biquad::LEFT) * 2;
        result += filt3.process(in, Biquad::LEFT);
    }
    // occluding
    else
    {
        return 0;
    }

    burst_filt.setF0(1500);
    burst_filt.setQ(0.2);
    burst_filt.recalculateCoeffs();

    result += burst_filt.process(burst_level, Biquad::LEFT);

    return result;
}
