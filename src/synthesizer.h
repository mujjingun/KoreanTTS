#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <vector>

#include "main.h"

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
    void phoneme_transition();
    fpoint gen_signal(fpoint freq, fpoint voice_level, fpoint noise_level);
    fpoint noise();

private:
    int sample_idx;
    int phoneme_idx;
    bool is_vowel;
    bool finished;
    std::vector<int> phonemes;

    fpoint last_vowel_sec;
    fpoint sec;
    fpoint progress_sec;
    fpoint duration;
};

#endif // SYNTHESIZER_H
