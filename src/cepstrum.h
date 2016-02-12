#ifndef CEPSTRUM_H_INCLUDED
#define CEPSTRUM_H_INCLUDED

#include "main.h"
#include <valarray>

std::vector<fpoint> spectral_envelope(std::valarray<complex> const& dft);
void fft(std::valarray<complex> &x);
void ifft(std::valarray<complex>& x);

#endif // CEPSTRUM_H_INCLUDED
