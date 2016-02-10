#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <vector>

#include "main.h"
#include "sdft.h"

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
    std::vector<int> phonemes;
    std::vector<fpoint> samples;

    SDFT sdft;

    int sample_idx;
    int phoneme_idx;
    bool finished;

    fpoint last_phoneme_sec;
    fpoint last_phoneme_sample_idx;
    fpoint progress_sec;
    fpoint duration;
};

#endif // SYNTHESIZER_H
