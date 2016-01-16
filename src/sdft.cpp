#include "sdft.h"
#include <cstdio>
#define M_PI 3.1415926535897932384626

// input signal
complex in[N];

// frequencies of each bins
fpoint freqs[N / 2 + 1];

// frequency bins of input signal after ft
// Size increased by one because the optimized sdft code writes data to freqs[N]
complex bins[N];

// transformed bins
complex trans_bins[N];

// forward coeffs -2 PI e^iw -- normalized (divided by N)
complex coeffs[N];

// global index for input and output signals
int idx;

// these are just there to optimize (get rid of index lookups in sdft)
complex oldest_data, newest_data;

//initilaize coefficients
void init_coeffs()
{
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

// initialize all data buffers
void init()
{
    // clear data
    for (int i = 0; i < N; ++i)
        in[i] = 0;
    init_coeffs();
    oldest_data = newest_data = 0.0;
    idx = 0;
}

// add data to circular buffer
void add_data(fpoint d)
{
    oldest_data = in[idx];
    newest_data = in[idx] = d;
}

// sliding dft
void sdft()
{
    complex delta = newest_data - oldest_data;
    for (int i = 0; i < N; ++i)
    {
        trans_bins[i] = bins[i] = bins[i] * coeffs[i] + delta;
    }
}

// apply hann window
void apply_window()
{
    for (int i = 1; i < N - 1; ++i)
    {
        trans_bins[i] = - bins[i - 1] + bins[i] * fpoint(2) - bins[i + 1];
    }
}

// mirror the freqs
void mirror()
{
    for(int i = 0; i < N / 2; ++i)
    {
        trans_bins[N - i] = std::conj(trans_bins[i]);
    }
}

// sum to recreate first sample
complex sum_frame()
{
    complex sum = 0;
    for (int i = 0; i < N; i += 2)
    {
        sum += trans_bins[i];
    }
    for (int i = 1; i < N; i += 2)
    {
        sum -= trans_bins[i];
    }
    return sum / fpoint(N);
}

// magnitude of complex number
fpoint mag(complex& c)
{
    return sqrt(c.real() * c.real() + c.imag() * c.imag());
}

// print spectrum
void powr_spectrum(fpoint *powr)
{
    for (int i = 0; i < N / 2; ++i)
    {
        powr[i] = mag(trans_bins[i]);

        printf("%.2f,", powr[i]);
    }

}

// increase idx
void end_frame()
{
    if (++idx == N) idx = 0; // bump global index
}

fpoint get_freq(int n)
{
    return freqs[n];
}

void copy_buffer(complex *buf)
{
    for (int i = 0; i <= N / 2; ++i)
    {
        buf[i] = trans_bins[i];
    }
}

void paste_buffer(complex *buf)
{
    for (int i = 0; i <= N / 2; ++i)
    {
        trans_bins[i] = buf[i];
    }
}

void calc_bp_filter(fpoint freq, fpoint* filter)
{
    for(int j = 0; j <= N / 2; j++)
    {
        fpoint f = get_freq(j);
        fpoint r = 200, w0 = freq;
        filter[j] = r / 2 / ((f - w0) * (f - w0) + (r / 2) * (r / 2)) * 1000;
    }
}
