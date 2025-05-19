




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "liquid.internal.h"


struct FIRPFBCHR(_s) {
    unsigned int M;     
    unsigned int P;     
    unsigned int m;     

    
    unsigned int h_len; 
    
    
    DOTPROD() * dp;     

    
    FFT_PLAN ifft;      
    TO * X;             
    TO * x;             

    
    
    WINDOW() * w;       
    unsigned int base_index;
};







FIRPFBCHR() FIRPFBCHR(_create)(unsigned int _chans,
                               unsigned int _decim,
                               unsigned int _m,
                               TC *         _h)
{
    
    if (_chans < 2)
        return liquid_error_config("firpfbchr_%s_create(), number of channels must be at least 2", EXTENSION_FULL);
    if (_decim < 1)
        return liquid_error_config("firpfbchr_%s_create(), decimation rate must be at least 1", EXTENSION_FULL);
    if (_m < 1)
        return liquid_error_config("firpfbchr_%s_create(), filter semi-length must be at least 1", EXTENSION_FULL);
    if (_h == NULL)
        return liquid_error_config("firpfbchr_%s_create(), filter coefficients cannot be null", EXTENSION_FULL);

    
    FIRPFBCHR() q = (FIRPFBCHR()) malloc(sizeof(struct FIRPFBCHR(_s)));

    
    q->M  = _chans; 
    q->P  = _decim; 
    q->m  = _m;     

    
    q->h_len    = 2*q->M*q->m;  

    
    q->dp = (DOTPROD()*) malloc((q->M)*sizeof(DOTPROD()));
    unsigned int i;
    unsigned int n;
    unsigned int h_sub_len = 2 * q->m;
    TC h_sub[h_sub_len];
    for (i=0; i<q->M; i++) {
        
        
        for (n=0; n<h_sub_len; n++)
            h_sub[h_sub_len-n-1] = _h[i + n*(q->M)];

        
        q->dp[i] = DOTPROD(_create)(h_sub,h_sub_len);
    }

    
    q->X = (T*) FFT_MALLOC((q->M)*sizeof(T));   
    q->x = (T*) FFT_MALLOC((q->M)*sizeof(T));   
    q->ifft = FFT_CREATE_PLAN(q->M, q->X, q->x, FFT_DIR_BACKWARD, FFT_METHOD);

    
    q->w = (WINDOW()*) malloc((q->M)*sizeof(WINDOW()));
    for (i=0; i<q->M; i++)
        q->w[i] = WINDOW(_create)(h_sub_len);

    
    FIRPFBCHR(_reset)(q);
    return q;
}







FIRPFBCHR() FIRPFBCHR(_create_kaiser)(unsigned int _chans,
                                      unsigned int _decim,
                                      unsigned int _m,
                                      float        _as)
{
    
    if (_chans < 2)
        return liquid_error_config("firpfbchr_%s_create_kaiser(), number of channels must be at least 2", EXTENSION_FULL);
    if (_decim < 1)
        return liquid_error_config("firpfbchr_%s_create_kaiser(), decimation rate must be at least 1", EXTENSION_FULL);
    if (_m < 1)
        return liquid_error_config("firpfbchr_%s_create_kaiser(), filter semi-length must be at least 1", EXTENSION_FULL);
    if (_as <= 0.0f)
        return liquid_error_config("firpfbchr_%s_create_kaiser(), stop-band suppression out of range", EXTENSION_FULL);

    
    unsigned int h_len = 2*_chans*_m+1;
    float * hf = (float*)malloc(h_len*sizeof(float));

    
    float fc = 0.5f/(float)_decim;

    
    liquid_firdes_kaiser(h_len, fc, _as, 0.0f, hf);

    
    float hf_sum = 0.0f;
    unsigned int i;
    for (i=0; i<h_len; i++) hf_sum += hf[i];
    for (i=0; i<h_len; i++) hf[i] = hf[i] * sqrtf(_decim) * (float)_chans / hf_sum;

    
    TC * h = (TC*) malloc(h_len * sizeof(TC));
    for (i=0; i<h_len; i++)
        h[i] = (TC) hf[i];

    
    FIRPFBCHR() q = FIRPFBCHR(_create)(_chans, _decim, _m, h);

    
    free(hf);
    free(h);

    
    return q;
}


int FIRPFBCHR(_destroy)(FIRPFBCHR() _q)
{
    unsigned int i;

    
    for (i=0; i<_q->M; i++)
        DOTPROD(_destroy)(_q->dp[i]);
    free(_q->dp);

    
    FFT_DESTROY_PLAN(_q->ifft);
    FFT_FREE(_q->X);
    FFT_FREE(_q->x);
    
    
    for (i=0; i<_q->M; i++)
        WINDOW(_destroy)(_q->w[i]);
    free(_q->w);

    
    free(_q);
    return LIQUID_OK;
}


int FIRPFBCHR(_reset)(FIRPFBCHR() _q)
{
    unsigned int i;

    
    for (i=0; i<_q->M; i++)
        WINDOW(_reset)(_q->w[i]);

    
    _q->base_index = _q->M - 1;
    return LIQUID_OK;
}


int FIRPFBCHR(_print)(FIRPFBCHR() _q)
{
    printf("<liquid.firpfbchr, channels=%u, decim=%u, semilen=%u>\n",
        _q->M, _q->P, _q->m);
    return LIQUID_OK;
}


unsigned int FIRPFBCHR(_get_M)(FIRPFBCHR() _q)
{
    return FIRPFBCHR(_get_num_channels)(_q);
}


unsigned int FIRPFBCHR(_get_num_channels)(FIRPFBCHR() _q)
{
    return _q->M;
}


unsigned int FIRPFBCHR(_get_P)(FIRPFBCHR() _q)
{
    return FIRPFBCHR(_get_decim_rate)(_q);
}


unsigned int FIRPFBCHR(_get_decim_rate)(FIRPFBCHR() _q)
{
    return _q->P;
}


unsigned int FIRPFBCHR(_get_m)(FIRPFBCHR() _q)
{
    return _q->m;
}




int FIRPFBCHR(_push)(FIRPFBCHR() _q,
                     TI *        _x)
{
    
    unsigned int i;
    for (i=0; i<_q->P; i++) {
        
        WINDOW(_push)(_q->w[_q->base_index], _x[i]);

        
        _q->base_index = _q->base_index == 0 ? _q->M-1 : _q->base_index-1;
    }
    return LIQUID_OK;
}




int FIRPFBCHR(_execute)(FIRPFBCHR() _q,
                        TO *        _y)
{
    unsigned int i;

    
    TO * r;  
    for (i=0; i<_q->M; i++) {
        
        unsigned int buffer_index = (_q->base_index+i+1) % _q->M;

        
        WINDOW(_read)(_q->w[buffer_index], &r);

        
        DOTPROD(_execute)(_q->dp[i], r, &_q->X[buffer_index]);
    }

    
    FFT_EXECUTE(_q->ifft);

    
    float g = 1.0f / (float)(_q->M);
    for (i=0; i<_q->M; i++)
        _y[i] = _q->x[i] * g;
    return LIQUID_OK;
}

