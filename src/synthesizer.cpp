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
    vowel_idx = -1;
    burst_duration = 0.001;
    filt1.reset();
    filt2.reset();
    filt3.reset();
    filt4.reset();
    burst_filt.reset();
}

int Synthesizer::update_params()
{
    fpoint sec = fpoint(sample_idx) / Fs;
    fpoint vowel_progress_sec = sec - last_vowel_sec;
    fpoint vowel_progress = vowel_progress_sec / vowel_duration;

    if(vowel_idx < 0 || vowel_progress_sec > vowel_duration)
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

            // is there a pause before the vowel
            if(vowel_idx == 0 || phonemes[vowel_idx - 1] >= 300)
            {
                is_prev = false;
            }
            else
            {
                is_prev = true;
                int prev_ch = phonemes[vowel_idx - 1];

                // previously a vowel
                if(prev_ch < 100)
                {
                    Vowel prev_vowel = get_vowel_formants(prev_ch);
                    prev1 = prev_vowel.e1;
                    prev2 = prev_vowel.e2;
                    prev3 = prev_vowel.e3;
                }
                // previously *should be* an inital consonant
                else
                {
                    Consonant prev_consonant = get_consonant_formants(prev_ch - 100);
                    prev1 = prev_consonant.f1;
                    prev2 = prev_consonant.f2;
                    prev3 = vowel.s3;
                }
            }

            voice_level_start = 0;
            voice_level_end = 1.0;

            noise_level = 0;

            burst_level = 0;

            vowel_duration = 0.3;
        }
        // inital
        else if(ch < 200)
        {
            is_vowel = false;

            Consonant consonant = get_consonant_formants(ch - 100);
            Vowel next_vowel = get_vowel_formants(phonemes[vowel_idx + 1]);

            s1 = consonant.f1;
            s2 = consonant.f2;
            s3 = next_vowel.s3;

            e1 = next_vowel.s1;
            e2 = next_vowel.s2;
            e3 = next_vowel.s3;

            voice_level = 0;

            noise_level = 0;

            burst_level = 0;

            occluding = true;
            bursting = false;
            aspirating = false;

            closure_duration = 0.01;
            vowel_duration = closure_duration + consonant.vot;

        }
        // final
        else if(ch < 300)
        {
            //TODO
        }
        else
        {
            vowel_duration = 0;
        }

        printf("%g %g %g -> %g %g %g\n", s1, s2, s3, e1, e2, e3);
    }

    if(is_vowel)
    {
        f1 = linear(s1, e1, vowel_progress_sec, 0, 0.15);
        f2 = linear(s2, e2, vowel_progress_sec, 0, 0.15);
        f3 = linear(s3, e3, vowel_progress_sec, 0, 0.15);

        if(is_prev)
        {
            f1 = linear(prev1, f1, vowel_progress_sec, -0.6, 0.1);
            f2 = linear(prev2, f2, vowel_progress_sec, -0.6, 0.1);
            f3 = linear(prev3, f3, vowel_progress_sec, -0.6, 0.1);
        }

        voice_level = linear(voice_level_start, voice_level_end, vowel_progress_sec, 0, 0.035);
        voice_level = linear(voice_level, 0, vowel_progress_sec, vowel_duration - 0.05, vowel_duration);
    }
    // stop consonant
    else if(!is_vowel)
    {
        fpoint since_vot = vowel_progress_sec - closure_duration;

        // burst
        if(occluding && vowel_progress_sec > closure_duration)
        {
            occluding = false;
            bursting = true;

            burst_level = 0.5;
        }
        // aspiration
        else if(vowel_progress_sec > closure_duration)
        {
            bursting = false;
            aspirating = true;

            burst_level = 0;

            noise_level = 0.4;

            f1 = linear(s1, e1, since_vot, -0.3, vowel_duration + 0.1);
            f2 = linear(s2, e2, since_vot, -0.3, vowel_duration + 0.1);
            f3 = linear(s3, e3, since_vot, -0.3, vowel_duration + 0.1);
        }
    }

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

    fpoint in = gen_signal(250 - sample_idx / 400, voice_level, noise_level);

    filt1.setF0(f1);
    filt1.setQ(f1 / 20);
    filt1.recalculateCoeffs();

    filt2.setF0(f2);
    filt2.setQ(f2 / 20);
    filt2.recalculateCoeffs();

    filt3.setF0(f3);
    filt3.setQ(f3 / 20);
    filt3.recalculateCoeffs();

    filt4.setF0(5000);
    filt4.setQ(30);
    filt4.recalculateCoeffs();

    fpoint v1 = filt1.process(in, Biquad::LEFT);
    fpoint v2 = filt2.process(in, Biquad::LEFT);
    fpoint v3 = filt3.process(in, Biquad::LEFT);
    fpoint v4 = filt4.process(in, Biquad::LEFT);

    if(is_vowel || aspirating)
    {
        fpoint add = (v1 * 3 + v2 * 2 + v3 + v4 * 0.05) / 50;

        result += add;
    }

    burst_filt.setF0(4000);
    burst_filt.setQ(2);
    burst_filt.recalculateCoeffs();

    result += burst_filt.process(burst_level, Biquad::LEFT);

    return SoundIO::normalize(result);
}

fpoint Synthesizer::gen_signal(fpoint freq, fpoint voice_level, fpoint noise_level)
{
    fpoint F0 = freq; // HZ
    fpoint T = Fs / F0;

    if(sample_idx - last_period > T) last_period += T;

    fpoint phase = (sample_idx - last_period) / T;

    fpoint in = osc_get_sample(phase) * voice_level + noise() * noise_level;

    return in;
}

fpoint Synthesizer::noise()
{
    noise_filt.setF0(5000);
    noise_filt.setQ(1);
    noise_filt.recalculateCoeffs();
    fpoint noise = fpoint(rand()) / RAND_MAX;
    return noise;//noise_filt.process(noise, Biquad::LEFT);
}

bool Synthesizer::has_ended()
{
    return finished;
}
