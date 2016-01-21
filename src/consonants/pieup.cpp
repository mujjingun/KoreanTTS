#include "../consonants.h"
#include "../vocal.h"

////////////////////////////////////////
//
//  ㅍ : Pieup
//
////////////////////////////////////////

Pieup::Pieup()
: Consonant(20, 800, 0.02 + 0.075)
{
    closure_duration = 0.02;

    burst_filt = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);

    filt1 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt2 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt3 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
}

void Pieup::init(const Vowel & next_vowel)
{
    s1 = f1;
    s2 = f2;
    s3 = next_vowel.s3;

    e1 = next_vowel.s1;
    e2 = next_vowel.s2;
    e3 = next_vowel.s3;

    burst_level = 0;
    aspiration_level = 0.02;

    occluding = true;
    bursting = false;
    aspirating = false;

}

fpoint Pieup::gen_sample(fpoint progress_sec)
{
    fpoint result = 0;

    // burst
    if(occluding && progress_sec > closure_duration)
    {
        occluding = false;
        bursting = true;

        burst_level = 0.4;
    }
    // aspiration
    else if(progress_sec > closure_duration)
    {
        fpoint f1, f2, f3;

        bursting = false;
        aspirating = true;

        burst_level = 0;

        noise_level = linear(0, aspiration_level, progress_sec, closure_duration, duration);

        f1 = linear(s1, e1, progress_sec, -0.3, duration);
        f2 = linear(s2, e2, progress_sec, -0.3, duration);
        f3 = linear(s3, e3, progress_sec, -0.3, duration);

        fpoint in = osc_noise() * noise_level;

        filt1.setF0(f1);
        filt1.setQ(f1 / 80);
        filt1.recalculateCoeffs();

        filt2.setF0(f2);
        filt2.setQ(f2 / 80);
        filt2.recalculateCoeffs();

        filt3.setF0(f3);
        filt3.setQ(f3 / 80);
        filt3.recalculateCoeffs();

        result += filt1.process(in, Biquad::LEFT);
        result += filt2.process(in, Biquad::LEFT);
        result += filt3.process(in, Biquad::LEFT) * 0.2;
    }
    // occluding
    else
    {
        return 0;
    }

    burst_filt.setF0(400);
    burst_filt.setQ(8);
    burst_filt.recalculateCoeffs();

    result += burst_filt.process(burst_level, Biquad::LEFT);

    return result;
}
