//
// The sliding DFT implementation.
//

#ifndef SDFT_H_INCLUDED
#define SDFT_H_INCLUDED

#include "main.h"

class SDFT
{
public:

    // window size
    static const int N = 1024;

private:

    // input signal (circular queue)
    complex in[N];

    // frequencies of each bins
    fpoint freqs[N / 2 + 1];

    // forward coeffs -2 PI e^iw -- normalized (divided by N)
    complex coeffs[N];

    // global index for input and output signals
    int idx;

    // these are just there to optimize (get rid of index lookups in sdft)
    complex oldest_data, newest_data;

public:

    // frequency bins of input signal after ft
    // Size increased by one because the optimized sdft code writes data to freqs[N]
    std::vector<complex> bins;

    SDFT();
    void sdft(fpoint d);
    void apply_window();
    void mirror();
    complex sum_frame();
    void powr_spectrum(fpoint *powr);
};

#endif // SDFT_H_INCLUDED
