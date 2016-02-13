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

// FFT frame size
const int L = 1024;

// overlap factor
const int Ov = 8;

// step size
const int R = L / Ov;

Synthesizer::Synthesizer():
    phonemes(),
    samples(),
    ova(L),
    sig(L),
    window(L),
    last_phase(L / 2 + 1),
    sum_phase(L / 2 + 1),

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

    samples = get_samples_from_file("untitled");//filename.str());

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
            break;
        }

        fpoint in = samples[sample_idx - last_phoneme_sample_idx];

        // append the signal at the end
        sig[L - R + i] = in;
    }

    // apply analysis window
    std::valarray<complex> seg = sig * window, tmp(L);

    fft(seg);

    auto env = spectral_envelope(seg);
    //for(auto a : env) std::cout << std::fixed << a << ",";
    //std::cout << "\n\n";

    const fpoint expect = 2 * M_PI / Ov;

    // frequency between two bins
    const fpoint bin_freq = fpoint(Fs) / L;

    std::valarray<fpoint> tmp_mag(L / 2 + 1);
    std::valarray<fpoint> tmp_frq(L / 2 + 1);

    for(unsigned i = 0; i <= L / 2; i++)
    {
        // compute magnitude and phase
        fpoint magn = abs(seg[i]) * 2;
        fpoint phas = arg(seg[i]);

        // compute phase difference
        fpoint pdiff = phas - last_phase[i];
        last_phase[i] = phas;

        // subtract expected phase difference
        pdiff -= i * expect;

        // map delta phase into +/- Pi interval
        pdiff -= 2 * M_PI * floor(pdiff / (2 * M_PI) + 0.5);

        // get deviation from bin frequency from the +/- Pi interval
        fpoint fdev = Ov * pdiff / (2 * M_PI);

        // compute the k-th partials' true frequency
        fpoint true_freq = i * bin_freq + fdev * bin_freq;

        // store magnitude and true frequency in analysis arrays
        tmp_mag[i] = magn;
        tmp_frq[i] = true_freq;
    }

    // copy buffers
    std::valarray<fpoint> seg_mag(L / 2 + 1);
    std::valarray<fpoint> seg_frq(L / 2 + 1);

    // pitch shifting
    fpoint pitch_shift = 1.5;
    for (unsigned i = 0; i <= L / 2; i++)
    {
        unsigned index = i * pitch_shift;
        if (index <= L / 2)
        {
            seg_mag[index] += tmp_mag[i];// / env[i];
            seg_frq[index] = tmp_frq[i] * pitch_shift;
        }
    }

    // synthesis
    for (unsigned i = 0; i <= L / 2; i++)
    {
        // get magnitude and true frequency from synthesis arrays
        fpoint mag = seg_mag[i];
        fpoint frq = seg_frq[i];

        // subtract bin mid frequency
        frq -= i * bin_freq;

        // get bin deviation from freq deviation
        frq /= bin_freq;

        // take osamp into account
        frq = 2 * M_PI * frq / Ov;

        // add the overlap phase advance back in
        frq += i * expect;

        // accumulate delta phase to get bin phase
        sum_phase[i] += frq;
        fpoint phase = sum_phase[i];

        // get real and imag part
        seg[i] = std::polar(mag, phase);
        seg[L - i - 1] = 0;
    }

    ifft(seg);

    ova = ova.shift(R) + seg * window;

    std::vector<float> out(R, 0);

    for(unsigned i = 0; i < R; i++)
    {
        out[i] = (float)clamp(-1, 1, ova[i].real());
    }

    return out;
}

bool Synthesizer::has_ended()
{
    return finished;
}
