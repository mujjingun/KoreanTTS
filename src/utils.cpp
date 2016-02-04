#include <cmath> // fmin, fmax
#include <random>
#include "utils.h"

fpoint clamp(fpoint s, fpoint e, fpoint x)
{
    return fmax(fmin(e, x), s);
}

fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex)
{
    return (e - s) * clamp(0, 1, (x - sx) / (ex - sx)) + s;
}

std::default_random_engine noise_generator;
std::normal_distribution<fpoint> gauss_distribution(0, 1.0);

fpoint osc_noise()
{
    return clamp(-1, 1, gauss_distribution(noise_generator));
}
