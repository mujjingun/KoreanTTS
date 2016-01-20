#include "consonants.h"
#include "vocal.h"

Consonant::Consonant(fpoint f1, fpoint f2, fpoint duration)
: f1(f1), f2(f2), duration(duration)
{
    // TODO
}

////////////////////////////////////////
//
//  ㅂ : Bieup
//
////////////////////////////////////////

Bieup::Bieup()
: Consonant(20, 800, 0.02 + 0.035)
{
    closure_duration = 0.02;

    burst_filt = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
}

void Bieup::init(FormantFilters & filt, const Vowel & next_vowel)
{
    this->filt = &filt;

    s1 = f1;
    s2 = f2;
    s3 = next_vowel.s3;

    e1 = next_vowel.s1;
    e2 = next_vowel.s2;
    e3 = next_vowel.s3;

    burst_level = 0;
    aspiration_level = 0.03;

    occluding = true;
    bursting = false;
    aspirating = false;

}

fpoint Bieup::gen_sample(fpoint progress_sec)
{
    fpoint result = 0;

    // burst
    if(occluding && progress_sec > closure_duration)
    {
        occluding = false;
        bursting = true;

        burst_level = 0.3;
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

        filt->f1.setF0(f1);
        filt->f1.setQ(f1 / 80);
        filt->f1.recalculateCoeffs();

        filt->f2.setF0(f2);
        filt->f2.setQ(f2 / 80);
        filt->f2.recalculateCoeffs();

        filt->f3.setF0(f3);
        filt->f3.setQ(f3 / 80);
        filt->f3.recalculateCoeffs();

        result += filt->f1.process(in, Biquad::LEFT);
        result += filt->f2.process(in, Biquad::LEFT);
        result += filt->f3.process(in, Biquad::LEFT);
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
