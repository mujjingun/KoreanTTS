#include "sdft.h"
#include <stdio.h>

#define M_PI 3.1415926535897932384626

SDFT::SDFT():
    idx(0),
    oldest_data(0),
    newest_data(0),
    bins(N, 0)
{
    // clear data
    for (int i = 0; i < N; ++i)
    {
        in[i] = 0;
    }

    //initilaize coefficients
    for (int n = 0; n < N; ++n)
    {
        fpoint a = 2.0 * M_PI * n / fpoint(N);
        coeffs[n] = complex(cos(a), sin(a));
    }
    for(int i = 0; i <= N / 2; i++)
    {
        freqs[i] = fpoint(i) / N * Fs;
    }
}

// add data to circular buffer
// and perform the sliding DFT
void SDFT::sdft(fpoint d)
{
    oldest_data = in[idx];
    newest_data = in[idx] = d;
    complex delta = newest_data - oldest_data;
    for (int i = 0; i < N; ++i)
    {
        bins[i] = bins[i] * coeffs[i] + delta;
    }
    if (++idx == N) idx = 0; // bump global index
}

// apply hann window
void SDFT::apply_window()
{
    for (int i = 1; i < N - 1; ++i)
    {
        //trans_bins[i] = - bins[i - 1] + bins[i] * fpoint(2) - bins[i + 1];
    }
}

// mirror the freqs
void SDFT::mirror()
{
    for(int i = 0; i < N / 2; ++i)
    {
        bins[N - i] = std::conj(bins[i]);
    }
}

// sum to recreate middle sample
complex SDFT::sum_frame()
{
    complex sum = 0;
    for (int i = 0; i < N; i += 2)
    {
        sum += bins[i];
    }
    for (int i = 1; i < N; i += 2)
    {
        sum -= bins[i];
    }
    return sum / fpoint(N);
}

// print spectrum
void SDFT::powr_spectrum(fpoint *powr)
{
    for (int i = 0; i < N / 2; ++i)
    {
        powr[i] = std::abs(bins[i]);

        printf("%.2f,", powr[i]);
    }

}
