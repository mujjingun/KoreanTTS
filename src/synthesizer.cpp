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
    voice = false;
    last_period = 0;
    last_vowel_sec = 0;
    vowel_duration = 0.5;
    vowel_idx = -1;
}

int Synthesizer::update_params()
{
    fpoint sec = fpoint(sample_idx) / Fs;
    fpoint vowel_progress = (sec - last_vowel_sec) / vowel_duration;
    if(vowel_idx < 0 || vowel_progress > 1)
    {
        if((size_t)++vowel_idx >= phonemes.size()) return sample_idx;
        last_vowel_sec = sec;

        int ch = phonemes[vowel_idx];
        if(ch < 100) voice = true;
        else
        {
            voice = false;
            return -1;
        }

        printf("generating %d : %d..\n", vowel_idx, ch);
        Vowel *vowel = get_formant(ch);
        vowel_type = vowel->type;

        // monophthong
        if(vowel_type == 'm')
        {
            Monophthong* m = dynamic_cast<Monophthong*>(vowel);
            f1 = m->f1;
            f2 = m->f2;
            f3 = m->f3;
            printf("%g %g %g\n", f1, f2, f3);
        }
        // diphthong
        else if(vowel_type == 'd')
        {
            Diphthong* d = dynamic_cast<Diphthong*>(vowel);
            Monophthong* m1 = dynamic_cast<Monophthong*>(get_formant(d->a));
            s1 = m1->f1;
            s2 = m1->f2;
            s3 = m1->f3;

            Monophthong* m2 = dynamic_cast<Monophthong*>(get_formant(d->b));
            e1 = m2->f1;
            e2 = m2->f2;
            e3 = m2->f3;

            printf("%g %g %g -> %g %g %g\n", s1, s2, s3, e1, e2, e3);
        }
    }

    if(voice && vowel_type == 'd')
    {
        f1 = linear(s1, e1, vowel_progress, 0, 0.2);
        f2 = linear(s2, e2, vowel_progress, 0, 0.2);
        f3 = linear(s3, e3, vowel_progress, 0, 0.2);
    }

    return -1;
}

int16_t Synthesizer::generate_sample()
{
    sample_idx++;

    if(update_params() >= 0)
    {
        finished = true;
        return 0;
    }

    fpoint result;

    if(voice)
    {
        fpoint F0 = 250 + sample_idx / 500; // HZ
        fpoint T = Fs / F0;

        fpoint phase = (sample_idx - last_period) / T;

        if(phase > 1) last_period += T;

        fpoint noise = fpoint(rand()) / RAND_MAX;

        fpoint in = osc_get_sample(phase) + noise / 50;

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

        // resynthesis
        fpoint add = (v1 + v2 * 2 + v3 * 2 + v4 * 0.1) / 5.1;

        result = add;

    }

    return SoundIO::normalize(result);
}

bool Synthesizer::has_ended()
{
    return finished;
}
