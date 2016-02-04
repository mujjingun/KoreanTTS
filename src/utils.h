#ifndef FORMANTS_H_INCLUDED
#define FORMANTS_H_INCLUDED

#include "main.h"

fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex);
fpoint clamp(fpoint s, fpoint e, fpoint x);
fpoint osc_noise();

#endif // FORMANTS_H_INCLUDED
