#include <cstdlib>
#include <cmath>

#include "synthesizer.h"
#include "soundio.h"
#include "formants.h"
#include "oscilator.h"

Synthesizer::Synthesizer()
{
    gauss_distribution = std::normal_distribution<fpoint>(-1.0, 1.0);

    filt.f1 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt.f2 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt.f3 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt.f4 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    filt.f5 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);

    burst_filt = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
    noise_filt = Biquad(Biquad::LPF, Fs);
}

void Synthesizer::start(std::vector<int> const& phonemes)
{
    this->phonemes = phonemes;
    finished = false;
    sample_idx = 0;
    last_period = 0;
    last_vowel_sec = 0;
    vowel_idx = -1;
    burst_duration = 0.001;
    filt.reset();
    burst_filt.reset();
}

int Synthesizer::update_params()
{
    fpoint sec = fpoint(sample_idx) / Fs;
    vowel_progress_sec = sec - last_vowel_sec;

    if(vowel_idx < 0 || vowel_progress_sec > vowel_duration)
    {
        if((size_t)++vowel_idx >= phonemes.size()) return sample_idx;
        last_vowel_sec = sec;
        vowel_progress_sec = 0;

        int ch = phonemes[vowel_idx];

        printf("generating %d : %d..\n", vowel_idx, ch);

        // vowel
        if(ch < 100)
        {
            is_vowel = true;

            occluding = false;
            bursting = false;
            aspirating = false;

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
                is_prev_consonant = false;
                is_prev_vowel = false;
            }
            else
            {
                int prev_ch = phonemes[vowel_idx - 1];

                // previously a vowel
                if(prev_ch < 100)
                {
                    is_prev_consonant = false;
                    is_prev_vowel = true;

                    Vowel prev_vowel = get_vowel_formants(prev_ch);
                    prev1 = prev_vowel.e1;
                    prev2 = prev_vowel.e2;
                    prev3 = prev_vowel.e3;
                }
                // previously *should be* an inital consonant
                else
                {
                    is_prev_consonant = true;
                    is_prev_vowel = false;

                    Consonant prev_consonant = get_consonant_formants(prev_ch - 100);
                    prev1 = prev_consonant.f1;
                    prev2 = prev_consonant.f2;
                    prev3 = vowel.s3;
                }
            }

            // next is a vowel
            if((size_t)vowel_idx < phonemes.size() - 1 && phonemes[vowel_idx + 1] < 100)
            {
                is_next = true;

                int next_ch = phonemes[vowel_idx + 1];

                Vowel next_vowel = get_vowel_formants(next_ch);
                next1 = next_vowel.s1;
                next2 = next_vowel.s2;
                next3 = next_vowel.s3;
            }
            else
            {
                is_next = false;
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

            aspiration_level = 0.03;
            noise_level = 0;

            burst_level = 0;

            occluding = true;
            bursting = false;
            aspirating = false;

            closure_duration = 0.02;
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
        f1 = linear(s1, e1, vowel_progress_sec, 0, 0.1);
        f2 = linear(s2, e2, vowel_progress_sec, 0, 0.1);
        f3 = linear(s3, e3, vowel_progress_sec, 0, 0.1);

        if(is_prev_consonant)
        {
            f1 = linear(prev1, f1, vowel_progress_sec, -0.6, 0.1);
            f2 = linear(prev2, f2, vowel_progress_sec, -0.6, 0.1);
            f3 = linear(prev3, f3, vowel_progress_sec, -0.6, 0.1);
        }

        if(is_next)
        {
            f1 = linear(f1, next1, vowel_progress_sec, vowel_duration - 0.05, vowel_duration);
            f2 = linear(f2, next2, vowel_progress_sec, vowel_duration - 0.05, vowel_duration);
            f3 = linear(f3, next3, vowel_progress_sec, vowel_duration - 0.05, vowel_duration);
        }

        if(is_prev_vowel)
        {
            voice_level = voice_level_end;
        }
        else
        {
            voice_level = linear(voice_level_start, voice_level_end, vowel_progress_sec, 0, 0.01);
        }

        if(!is_next)
        {
            voice_level = linear(voice_level, 0, vowel_progress_sec, vowel_duration - 0.15, vowel_duration);
        }
    }
    // stop consonant
    else if(!is_vowel)
    {
        // burst
        if(occluding && vowel_progress_sec > closure_duration)
        {
            occluding = false;
            bursting = true;

            burst_level = 0.3;
        }
        // aspiration
        else if(vowel_progress_sec > closure_duration)
        {
            bursting = false;
            aspirating = true;

            burst_level = 0;

            noise_level = linear(0, aspiration_level, vowel_progress_sec, closure_duration, vowel_duration);

            f1 = linear(s1, e1, vowel_progress_sec, -0.3, vowel_duration);
            f2 = linear(s2, e2, vowel_progress_sec, -0.3, vowel_duration);
            f3 = linear(s3, e3, vowel_progress_sec, -0.3, vowel_duration);
        }
    }

    return -1;
}

fpoint Synthesizer::generate_sample()
{
    sample_idx++;

    if(update_params() >= 0 || finished)
    {
        finished = true;
        return 0;
    }

    fpoint result = 0;

    fpoint in = gen_signal(290 - sin(vowel_progress_sec * 10) * 50, voice_level, noise_level);

    filt.f1.setF0(f1);
    filt.f1.setQ(f1 / 80);
    filt.f1.recalculateCoeffs();

    filt.f2.setF0(f2);
    filt.f2.setQ(f2 / 80);
    filt.f2.recalculateCoeffs();

    filt.f3.setF0(f3);
    filt.f3.setQ(f3 / 80);
    filt.f3.recalculateCoeffs();

    filt.f4.setF0(4500);
    filt.f4.setQ(4500.0 / 80);
    filt.f4.recalculateCoeffs();

    filt.f5.setF0(5500);
    filt.f5.setQ(5500.0 / 80);
    filt.f5.recalculateCoeffs();

    fpoint v1 = filt.f1.process(in, Biquad::LEFT);
    fpoint v2 = filt.f2.process(in, Biquad::LEFT);
    fpoint v3 = filt.f3.process(in, Biquad::LEFT);
    fpoint v4 = filt.f4.process(in, Biquad::LEFT);
    fpoint v5 = filt.f5.process(in, Biquad::LEFT);

    if(is_vowel || aspirating)
    {
        fpoint add = (v1 * 1.3 + v2 + v3 + v4 * 0.1 + v5 * 0.1) / 10;

        result += add;
    }

    burst_filt.setF0(400);
    burst_filt.setQ(8);
    burst_filt.recalculateCoeffs();

    result += burst_filt.process(burst_level, Biquad::LEFT);

    return result;
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

    fpoint noise = gauss_distribution(noise_generator);
    return noise_filt.process(noise, Biquad::LEFT);
}

bool Synthesizer::has_ended()
{
    return finished;
}
