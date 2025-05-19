





#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "liquid.internal.h"

#define LIQUID_VALIDATE_INPUT
#define QUANTIZER_MAX_BITS      32



unsigned int quantize_adc(float _x, unsigned int _num_bits)
{
#ifdef LIQUID_VALIDATE_INPUT
    if (_num_bits > QUANTIZER_MAX_BITS) {
        liquid_error(LIQUID_EIRANGE,"quantize_adc(), maximum bits exceeded");
        return 0;
    }
#endif

    if (_num_bits == 0)
        return 0;

    unsigned int n = _num_bits-1;   
    unsigned int N = 1<<n;          

    
    int neg = (_x < 0);
    unsigned int r = floorf(fabsf(_x)*N);

    
    if (r >= N)
        r = N-1;

    
    if (neg)
        r |= N;

    return r;
}

float quantize_dac(unsigned int _s, unsigned int _num_bits)
{
#ifdef LIQUID_VALIDATE_INPUT
    if (_num_bits > QUANTIZER_MAX_BITS) {
        liquid_error(LIQUID_EIRANGE,"quantize_dac(), maximum bits exceeded");
        return 0.0f;
    }
#endif
    if (_num_bits == 0)
        return 0.0f;

    unsigned int n = _num_bits-1;   
    unsigned int N = 1<<n;          
    float r = ((float)(_s & (N-1))+0.5f) / (float) (N);

    
    return (_s & N) ? -r : r;
}

