#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "synthesizer.h"
#include "soundio.h"
#include "utils.h"
#include "cepstrum.h"
#define M_PI 3.14159265358979323846264338328L

const int L = 256;
const int R = L / 4;

Synthesizer::Synthesizer():
    phonemes(),
    samples(),
    ova(L),
    sig(L),
    window(L),
    sample_idx(0),
    phoneme_idx(0),
    finished(false),
    last_phoneme_sec(0),
    last_phoneme_sample_idx(0),
    progress_sec(0),
    duration(0)
{
    for(int i = 0; i < L; i++)
    {
        window[i] = .5*(1 - cos(2 * M_PI * i / (L+1)));
    }

    //for(auto a : window) std::cout<<std::fixed<<a.real()<<", ";
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

    //printf("generating %d : %d..\n", phoneme_idx, ch);

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

std::vector<float> Synthesizer::generate_frame()
{
    // shift the signal left to get space for the new signal
    sig = sig.shift(R);

    for(int i = 0; i < R; i++)
    {
        sample_idx++;

        if(update_params() >= 0 || finished)
        {
            finished = true;
            return std::vector<float>();
        }

        fpoint in = samples[sample_idx - last_phoneme_sample_idx];

        // append the signal at the end
        sig[L - R + i] = in;
    }

    std::valarray<complex> seg = sig * window, tmp(L);

    fft(seg);

    auto env = spectral_envelope(seg);

    // pitch shifting
    tmp = seg;
    for(unsigned i = 0; i < L / 2; i++)
    {
        unsigned shift = unsigned(i * 1);
        if(shift < L / 2)
        {
            seg[i] = tmp[shift];
        }
        else
        {
            seg[i] = 0;
        }
        seg[L - i - 1] = conj(seg[i]);
    }

    //std::cout<<"{";
    //for(auto a : env) std::cout<<a<<", ";
    //std::cout<<"},"<<std::endl;

    ifft(seg);

    ova = ova.shift(R) + seg * window;

    std::vector<float> out;

    for(unsigned i = 0; i < R; i++)
    {
        out.push_back((float)clamp(-1, 1, ova[i].real()));
    }

    return out;
}

bool Synthesizer::has_ended()
{
    return finished;
}
