#ifndef SDFT_H_INCLUDED
#define SDFT_H_INCLUDED

#include "main.h"

void init();
void add_data(fpoint d);
void sdft();
void apply_window();
fpoint get_freq(int n);
void mirror();
complex sum_frame();
void end_frame();
void powr_spectrum(fpoint *powr);
void copy_buffer(complex *buf);
void paste_buffer(complex *buf);
void calc_bp_filter(fpoint freq, fpoint* filter);

#endif // SDFT_H_INCLUDED
