#include <cstdlib>

#include "synthesizer.h"
#include "soundio.h"
#include "formants.h"
#include "oscilator.h"

void Synthesizer::start(std::vector<int> const& phonemes)
{
    this->phonemes = phonemes;
    finished = false;
    sample_idx = 0;
    last_period = 0;
    last_vowel_sec = 0;
    vowel_duration = 0.5;
    vowel_idx = -1;
}

int Synthesizer::update_params()
{
    fpoint sec = fpoint(sample_idx) / Fs;
    fpoint vowel_progress_sec = sec - last_vowel_sec;
    fpoint vowel_progress = vowel_progress_sec / vowel_duration;

    if(vowel_idx < 0 || vowel_progress > 1)
    {
        if((size_t)++vowel_idx >= phonemes.size()) return sample_idx;
        last_vowel_sec = sec;

        int ch = phonemes[vowel_idx];

        printf("generating %d : %d..\n", vowel_idx, ch);

        // vowel
        if(ch < 100)
        {
            is_vowel = true;

            Vowel vowel = get_vowel_formants(ch);

            s1 = vowel.s1;
            s2 = vowel.s2;
            s3 = vowel.s3;

            e1 = vowel.e1;
            e2 = vowel.e2;
            e3 = vowel.e3;

            printf("%g %g %g -> %g %g %g\n", s1, s2, s3, e1, e2, e3);
        }
        // inital
        else if(ch < 200)
        {
            is_vowel = false;
        }
        // final
        else if(ch < 300)
        {

        }

    }

    f1 = linear(s1, e1, vowel_progress, 0, 0.2);
    f2 = linear(s2, e2, vowel_progress, 0, 0.2);
    f3 = linear(s3, e3, vowel_progress, 0, 0.2);

    return -1;
}

int16_t Synthesizer::generate_sample()
{
    sample_idx++;

    if(update_params() >= 0 || finished)
    {
        finished = true;
        return 0;
    }

    fpoint result = 0;

    fpoint in = gen_signal(250 + sample_idx / 500, 1.0, 1.0 / 50);

    filt1.setF0(f1);
    filt1.setQ(10);
    filt1.recalculateCoeffs();

    filt2.setF0(f2);
    filt2.setQ(20);
    filt2.recalculateCoeffs();

    filt3.setF0(f3);
    filt3.setQ(20);
    filt3.recalculateCoeffs();

    filt4.setF0(5000);
    filt4.setQ(10);
    filt4.recalculateCoeffs();

    fpoint v1 = filt1.process(in, Biquad::LEFT);
    fpoint v2 = filt2.process(in, Biquad::LEFT);
    fpoint v3 = filt3.process(in, Biquad::LEFT);
    fpoint v4 = filt4.process(in, Biquad::LEFT);

    fpoint add = (v1 + v2 * 2 + v3 * 2 + v4 * 0.1) / 5.1;

    result = add;

    return SoundIO::normalize(result);
}

fpoint Synthesizer::gen_signal(fpoint freq, fpoint voice_level, fpoint noise_level)
{
    fpoint F0 = freq; // HZ
    fpoint T = Fs / F0;

    if(sample_idx - last_period > T) last_period += T;

    fpoint phase = (sample_idx - last_period) / T;

    fpoint noise = fpoint(rand()) / RAND_MAX;

    fpoint in = osc_get_sample(phase) * voice_level + noise * noise_level;

    return in;
}

bool Synthesizer::has_ended()
{
    return finished;
}
