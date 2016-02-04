#include <cstdlib>
#include <cmath>

#include "synthesizer.h"
#include "soundio.h"
#include "utils.h"

Synthesizer::Synthesizer()
{
}

void Synthesizer::start(std::vector<int> const& aphonemes)
{
    this->phonemes = aphonemes;
    finished = false;
    sample_idx = 0;
    last_vowel_sec = 0;
    phoneme_idx = -1;
}

void Synthesizer::phoneme_transition()
{
    int ch = phonemes[phoneme_idx];

    printf("generating %d : %d..\n", phoneme_idx, ch);

    duration = 0.2;
}

int Synthesizer::update_params()
{
    sec = fpoint(sample_idx) / Fs;
    progress_sec = sec - last_vowel_sec;

    if(phoneme_idx < 0 || progress_sec > duration)
    {
        if((unsigned)++phoneme_idx >= phonemes.size()) return sample_idx;
        last_vowel_sec = sec;
        progress_sec = 0;

        phoneme_transition();
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

    return clamp(-1, 1, result);
}

bool Synthesizer::has_ended()
{
    return finished;
}
