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

    private:
        int sample_idx;
        int vowel_idx;
        bool is_vowel;
        bool finished;
        std::vector<int> phonemes;

        fpoint f1, f2, f3;
        fpoint s1, s2, s3;
        fpoint e1, e2, e3;
        fpoint vowel_duration;
        fpoint vowel_progress;
        fpoint last_period;
        fpoint last_vowel_sec;

        Biquad filt1 = Biquad(Biquad::BPF_CONSTANT_PEAK, Fs);
        Biquad filt2 = Biquad(Biquad::BPF_CONSTANT_PEAK, Fs);
        Biquad filt3 = Biquad(Biquad::BPF_CONSTANT_PEAK, Fs);
        Biquad filt4 = Biquad(Biquad::BPF_CONSTANT_PEAK, Fs);
};

#endif // SYNTHESIZER_H
