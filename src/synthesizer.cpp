#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "synthesizer.h"
#include "soundio.h"
#include "utils.h"

Synthesizer::Synthesizer()
: phonemes(), samples(),
sample_idx(0), phoneme_idx(0), finished(false),
last_phoneme_sec(0), last_phoneme_sample_idx(0),
progress_sec(0), duration(0)
{
}

void Synthesizer::start(std::vector<int> const& aphonemes)
{
    this->phonemes = aphonemes;
    finished = false;
    sample_idx = 0;
    last_phoneme_sec = 0;
    last_phoneme_sample_idx = 0;
    phoneme_idx = 0;
}

// transition between phonemes
void Synthesizer::phoneme_transition()
{
    int ch = phonemes[phoneme_idx];
    int next = phonemes[phoneme_idx + 1];

    printf("generating %d : %d..\n", phoneme_idx, ch);

    std::stringstream filename;
    filename << std::setfill('0') << std::setw(2) << ch << std::setw(2) << next;

    std::cout << filename.str() << std::endl;

    samples = get_samples_from_file(filename.str());

    duration = samples.size() / Fs;
}

int Synthesizer::update_params()
{
    fpoint sec = fpoint(sample_idx) / Fs;
    progress_sec = sec - last_phoneme_sec;

    if(phoneme_idx == 0 || progress_sec > duration)
    {
        if((unsigned)++phoneme_idx >= phonemes.size() - 1)
        {
            return sample_idx;
        }

        last_phoneme_sec = sec;
        last_phoneme_sample_idx = sample_idx;
        progress_sec = 0;

        phoneme_transition();

        if(samples.empty()) samples.push_back(0);
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

    fpoint result = samples[sample_idx - last_phoneme_sample_idx];

    return clamp(-1, 1, result);
}

bool Synthesizer::has_ended()
{
    return finished;
}
