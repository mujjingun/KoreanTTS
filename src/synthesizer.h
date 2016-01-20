#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <vector>
#include <random>

#include "main.h"
#include "biquad.h"

class FormantFilters
{
public:
    Biquad f1;
    Biquad f2;
    Biquad f3;
    Biquad f4;
    Biquad f5;

    void reset()
    {
        f1.reset();
        f2.reset();
        f3.reset();
        f4.reset();
        f5.reset();
    }
};

class Synthesizer
{
public:
    Synthesizer();
    virtual ~Synthesizer() {};

    void start(std::vector<int> const& phonemes);
    fpoint generate_sample();
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

    bool is_prev_consonant, is_prev_vowel, is_next;
    fpoint prev1, prev2, prev3;
    fpoint next1, next2, next3;

    fpoint voice_level;
    fpoint voice_level_start;
    fpoint voice_level_end;

    fpoint aspiration_level;
    fpoint noise_level;

    fpoint burst_level;

    bool occluding, bursting, aspirating;

    fpoint burst_duration;
    fpoint vowel_duration;
    fpoint vowel_progress_sec;
    fpoint last_vowel_sec;
    fpoint closure_duration;

    fpoint last_period;

    FormantFilters filt;

    Biquad burst_filt;
    Biquad noise_filt;

    std::default_random_engine noise_generator;
    std::normal_distribution<fpoint> gauss_distribution;
};

#endif // SYNTHESIZER_H
