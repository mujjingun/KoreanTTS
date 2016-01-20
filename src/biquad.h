#ifndef __biquad_h__
#define __biquad_h__

#include "main.h"

class Biquad
{
public:

    enum
    {
        LPF,
        HPF,
        BPF_CONSTANT_SKIRT,
        BPF_CONSTANT_PEAK,
        NOTCH,
        APF,
        PEAKING_EQ,
        LOW_SHELF,
        HIGH_SHELF
    };

    enum
    {
        PARAM_Q,
        PARAM_BW,
        PARAM_S
    };

    enum
    {
        LEFT,
        RIGHT
    };

    Biquad() {};
    Biquad(int type, fpoint sampleRate);
    virtual ~Biquad() {};

    void recalculateCoeffs();
    void setFilterType(int filterType);
    void setSampleRate(fpoint rate);
    void setQ(fpoint q);
    void setBW(fpoint bw);
    void setS(fpoint s);
    void setF0(fpoint freq);
    void setDbGain(fpoint g);
    void reset();

    /// process one sample
    fpoint process(fpoint v, int side);

private:

    fpoint sampleRate;
    int type;
    int paramType;
    fpoint x_1_l, x_2_l;
    fpoint y_1_l, y_2_l;
    fpoint x_1_r, x_2_r;
    fpoint y_1_r, y_2_r;
    fpoint b0, a0;
    fpoint b1, a1;
    fpoint b2, a2;
    fpoint b0a0, b1a0, b2a0;
    fpoint a1a0, a2a0;
    fpoint f0;
    fpoint dBgain;
    fpoint Q;
    fpoint BW;
    fpoint S;
};

#endif
