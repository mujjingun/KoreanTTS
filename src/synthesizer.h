#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <vector>

#include "main.h"
#include "biquad.h"

class Synthesizer
{
    public:
        Synthesizer(){};
        virtual ~Synthesizer(){};

        void start(std::vector<int> const& phonemes);
        int16_t generate_sample();
        bool has_ended();

    protected:
        int update_params();
        fpoint gen_signal(fpoint freq, fpoint voice_level, fpoint noise_level);
        fpoint noise();

    private:
        int sample_idx;
        int vowel_idx;
        bool is_vowel;
        bool finished;
        std::vector<int> phonemes;

        fpoint f1, f2, f3;
        fpoint s1, s2, s3;
        fpoint e1, e2, e3;

        bool is_prev;
        fpoint prev1, prev2, prev3;

        fpoint voice_level;
        fpoint voice_level_start;
        fpoint voice_level_end;

        fpoint noise_level;

        fpoint burst_level;

        bool occluding, bursting, aspirating;

        fpoint burst_duration;
        fpoint vowel_duration;
        fpoint vowel_progress;
        fpoint last_vowel_sec;
        fpoint closure_duration;

        fpoint last_period;

        Biquad filt1 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
        Biquad filt2 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
        Biquad filt3 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
        Biquad filt4 = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
        Biquad burst_filt = Biquad(Biquad::BPF_CONSTANT_SKIRT, Fs);
        Biquad noise_filt = Biquad(Biquad::LPF, Fs);
};

#endif // SYNTHESIZER_H
