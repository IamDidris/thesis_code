





#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "liquid.internal.h"

#define LIQUID_VALIDATE_INPUT

float compress_mulaw(float _x, float _mu)
{
#ifdef LIQUID_VALIDATE_INPUT
    if ( _mu <= 0.0f ) {
        liquid_error(LIQUID_EIRANGE,"compress_mulaw(), mu out of range");
        return 0.0f;
    }
#endif
    float y = logf(1 + _mu*fabsf(_x)) / logf(1 + _mu);
    return copysignf(y, _x);
}

float expand_mulaw(float _y, float _mu)
{
#ifdef LIQUID_VALIDATE_INPUT
    if ( _mu <= 0.0f ) {
        liquid_error(LIQUID_EIRANGE,"expand_mulaw(), mu out of range");
        return 0.0f;
    }
#endif
    float x = (1/_mu)*( powf(1+_mu,fabsf(_y)) - 1);
    return copysign(x, _y);
}

int compress_cf_mulaw(float complex _x, float _mu, float complex * _y)
{
#ifdef LIQUID_VALIDATE_INPUT
    if ( _mu <= 0.0f )
        return liquid_error(LIQUID_EIRANGE,"compress_mulaw(), mu out of range");
#endif
    *_y = cexpf(_Complex_I*cargf(_x)) * logf(1 + _mu*cabsf(_x)) / logf(1 + _mu);
    return LIQUID_OK;
}

int expand_cf_mulaw(float complex _y, float _mu, float complex * _x)
{
#ifdef LIQUID_VALIDATE_INPUT
    if ( _mu <= 0.0f )
        return liquid_error(LIQUID_EIRANGE,"expand_mulaw(), mu out of range");
#endif
    *_x = cexpf(_Complex_I*cargf(_y)) * (1/_mu)*( powf(1+_mu,cabsf(_y)) - 1);
    return LIQUID_OK;
}



