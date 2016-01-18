#include <cmath>
#include <algorithm>
#include "biquad.h"

const fpoint M_PI = fpoint(3.1415926535897932384626);

Biquad::Biquad(int filterType, fpoint rate)
{
	sampleRate = rate;
	type = filterType;
	paramType = PARAM_Q;
	x_1_l = x_2_l = 0;
	y_1_l = y_2_l = 0;
	x_1_r = x_2_r = 0;
	y_1_r = y_2_r = 0;
	b0 = a0 = 1;
	b1 = a1 = 0;
	b2 = a2 = 0;
	b0a0 = b0 / a0;
	b1a0 = b1 / a0;
	b2a0 = b2 / a0;
	a1a0 = a1 / a0;
	a2a0 = a2 / a0;
	f0 = 3000;
	dBgain = 12;
	Q = 1;
	BW = -3;
	S = 1;
}

void Biquad::reset()
{
    x_1_l = x_2_l = 0;
	y_1_l = y_2_l = 0;
	x_1_r = x_2_r = 0;
	y_1_r = y_2_r = 0;
}

void Biquad::recalculateCoeffs()
{
	fpoint A;
	if(type == PEAKING_EQ || type == LOW_SHELF || type == HIGH_SHELF)
		A = pow(10.0f, dBgain / 40);
	else
		A = sqrt(pow(10.0f, dBgain / 20));

	fpoint w0 = M_PI * 2 * f0 / sampleRate;
	fpoint cosw0 = cos(w0);
	fpoint sinw0 = sin(w0);
	fpoint alpha = 0;
	if(paramType == PARAM_Q)
		alpha = sinw0 / (2 * Q);
	else if(paramType == PARAM_BW)
		alpha = sinw0 * sinh(log(2.0f) / 2.0f * BW * w0 / sinw0);
	else if (paramType == PARAM_S)
		alpha = sinw0 / 2 * sqrt((A + 1 / A) * (1 / S - 1) + 2);

	fpoint coeff = 0;
	switch (type) {
	case LPF:
		b0 = (1 - cosw0) / 2;
		b1 = 1 - cosw0;
		b2 = (1 - cosw0) / 2;
		a0 = 1 + alpha;
		a1 = -2 * cosw0;
		a2 = 1 - alpha;
		break;
	case HPF:
		b0 = (1 + cosw0) / 2;
		b1 = -(1 + cosw0);
		b2 = (1 + cosw0) / 2;
		a0 = 1 + alpha;
		a1 = -2 * cosw0;
		a2 = 1 - alpha;
		break;
	case BPF_CONSTANT_SKIRT:
		b0 = sinw0 / 2;
		b1 = 0;
		b2 = -sinw0 / 2;
		a0 = 1 + alpha;
		a1 = -2 * cosw0;
		a2 = 1 - alpha;
		break;
	case BPF_CONSTANT_PEAK:
		b0 = alpha;
		b1 = 0;
		b2 = -alpha;
		a0 = 1 + alpha;
		a1 = -2 * cosw0;
		a2 = 1 - alpha;
		break;
	case NOTCH:
		b0 = 1;
		b1 = -2 * cosw0;
		b2 = 1;
		a0 = 1 + alpha;
		a1 = -2 * cosw0;
		a2 = 1 - alpha;
		break;
	case APF:
		b0 = 1 - alpha;
		b1 = -2 * cosw0;
		b2 = 1 + alpha;
		a0 = 1 + alpha;
		a1 = -2 * cosw0;
		a2 = 1 - alpha;
		break;
	case PEAKING_EQ:
		b0 = 1 + alpha * A;
		b1 = -2 * cosw0;
		b2 = 1 - alpha * A;
		a0 = 1 + alpha / A;
		a1 = -2 * cosw0;
		a2 = 1 - alpha / A;
		break;
	case LOW_SHELF:
		coeff = sinw0 * sqrt((A * A + 1) * (1 / S - 1) + 2 * A);
		b0 = A * ((A + 1) - (A - 1) * cosw0 + coeff);
		b1 = 2 * A * ((A - 1) - (A + 1) * cosw0);
		b2 = A * ((A + 1) - (A - 1) * cosw0 - coeff);
		a0 = (A + 1) + (A - 1) * cosw0 + coeff;
		a1 = -2 * ((A - 1) + (A + 1) * cosw0);
		a2 = (A + 1) + (A - 1) * cosw0 - coeff;
		break;
	case HIGH_SHELF:
		coeff = sinw0 * sqrt((A * A + 1) * (1 / S - 1) + 2 * A);
		b0 = A * ((A + 1) + (A - 1) * cosw0 + coeff);
		b1 = -2 * A * ((A - 1) + (A + 1) * cosw0);
		b2 = A * ((A + 1) + (A - 1) * cosw0 - coeff);
		a0 = (A + 1) - (A - 1) * cosw0 + coeff;
		a1 = 2 * ((A - 1) - (A + 1) * cosw0);
		a2 = (A + 1) - (A - 1) * cosw0 - coeff;
	}
	b0a0 = b0 / a0;
	b1a0 = b1 / a0;
	b2a0 = b2 / a0;
	a1a0 = a1 / a0;
	a2a0 = a2 / a0;
}

void Biquad::setFilterType(int filterType)
{
	type = filterType;
}

void Biquad::setSampleRate(fpoint rate)
{
	sampleRate = rate;
}

void Biquad::setQ(fpoint q)
{
	paramType = PARAM_Q;
	Q = fmax(fmin(q, 115.0f), 0.001f);
}

void Biquad::setBW(fpoint bw)
{
	paramType = PARAM_BW;
	BW = bw;
}

void Biquad::setS(fpoint s)
{
	paramType = PARAM_S;
	S = fmax(fmin(s, 5.0f), 0.0001f);
}

void Biquad::setF0(fpoint freq)
{
	f0 = freq;
}

void Biquad::setDbGain(fpoint g)
{
	dBgain = g;
}

fpoint Biquad::process(fpoint v, int side)
{
	fpoint output;
	if (side == LEFT)
	{
		output = b0a0 * v + b1a0 * x_1_l + b2a0 * x_2_l - a1a0 * y_1_l - a2a0 * y_2_l;
		y_2_l = y_1_l;
		y_1_l = output;
		x_2_l = x_1_l;
		x_1_l = v;
	}
	else
	{
		output = b0a0 * v + b1a0 * x_1_r + b2a0 * x_2_r - a1a0 * y_1_r - a2a0 * y_2_r;
		y_2_r = y_1_r;
		y_1_r = output;
		x_2_r = x_1_r;
		x_1_r = v;
	}
	return output;
}
