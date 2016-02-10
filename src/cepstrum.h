#ifndef CEPSTRUM_H_INCLUDED
#define CEPSTRUM_H_INCLUDED

#include "main.h"

std::vector<fpoint> spectral_envelope(std::vector<complex> const& dft);

#endif // CEPSTRUM_H_INCLUDED
