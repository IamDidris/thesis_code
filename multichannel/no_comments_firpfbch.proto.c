



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "liquid.internal.h"


struct FIRPFBCH(_s) {
    int type;                   
    unsigned int num_channels;  
    unsigned int p;             

    
    unsigned int h_len;         
    TC * h;                     
    
    
    DOTPROD() * dp;             
    WINDOW() * w;               
    unsigned int filter_index;  

    
    FFT_PLAN fft;               
    TO * x;                     
    TO * X;                     
};





int FIRPFBCH(_analyzer_push)(FIRPFBCH() _q, TI _x);

int FIRPFBCH(_analyzer_run)(FIRPFBCH()   _q,
                            unsigned int _k,
                            TO *         _X);







FIRPFBCH() FIRPFBCH(_create)(int          _type,
                             unsigned int _M,
                             unsigned int _p,
                             TC *         _h)
{
    
    if (_type != LIQUID_ANALYZER && _type != LIQUID_SYNTHESIZER)
        return liquid_error_config("firpfbch_%s_create(), invalid type: %d", EXTENSION_FULL, _type);
    if (_M == 0)
        return liquid_error_config("firpfbch_%s_create(), number of channels must be greater than 0", EXTENSION_FULL);
    if (_p == 0)
        return liquid_error_config("firpfbch_%s_create(), invalid filter size (must be greater than 0)", EXTENSION_FULL);

    
    FIRPFBCH() q = (FIRPFBCH()) malloc(sizeof(struct FIRPFBCH(_s)));

    
    q->type         = _type;
    q->num_channels = _M;
    q->p            = _p;

    
    q->h_len = q->num_channels * q->p;

    
    q->dp = (DOTPROD()*) malloc((q->num_channels)*sizeof(DOTPROD()));
    q->w  = (WINDOW()*)  malloc((q->num_channels)*sizeof(WINDOW()));

    
    q->h = (TC*) malloc((q->h_len)*sizeof(TC));
    unsigned int i;
    for (i=0; i<q->h_len; i++)
        q->h[i] = _h[i];

    
    unsigned int n;
    unsigned int h_sub_len = q->p;
    TC h_sub[h_sub_len];
    for (i=0; i<q->num_channels; i++) {
        
        for (n=0; n<h_sub_len; n++) {
            h_sub[h_sub_len-n-1] = q->h[i + n*(q->num_channels)];
        }
        
        
        q->dp[i] = DOTPROD(_create)(h_sub,h_sub_len);
        q->w[i]  = WINDOW(_create)(h_sub_len);
    }

    
    q->x = (T*) FFT_MALLOC((q->num_channels)*sizeof(T));
    q->X = (T*) FFT_MALLOC((q->num_channels)*sizeof(T));

    
    if (q->type == LIQUID_ANALYZER)
        q->fft = FFT_CREATE_PLAN(q->num_channels, q->X, q->x, FFT_DIR_FORWARD, FFT_METHOD);
    else
        q->fft = FFT_CREATE_PLAN(q->num_channels, q->X, q->x, FFT_DIR_BACKWARD, FFT_METHOD);

    
    FIRPFBCH(_reset)(q);

    
    return q;
}







FIRPFBCH() FIRPFBCH(_create_kaiser)(int          _type,
                                    unsigned int _M,
                                    unsigned int _m,
                                    float        _as)
{
    
    if (_type != LIQUID_ANALYZER && _type != LIQUID_SYNTHESIZER)
        return liquid_error_config("firpfbch_%s_create_kaiser(), invalid type: %d", EXTENSION_FULL, _type);
    if (_M == 0)
        return liquid_error_config("firpfbch_%s_create_kaiser(), number of channels must be greater than 0", EXTENSION_FULL);
    if (_m == 0)
        return liquid_error_config("firpfbch_%s_create_kaiser(), invalid filter size (must be greater than 0)", EXTENSION_FULL);
    
    _as = fabsf(_as);

    
    unsigned int h_len = 2*_M*_m + 1;
    float h[h_len];
    float fc = 0.5f / (float)_M; 
    liquid_firdes_kaiser(h_len, fc, _as, 0.0f, h);

    
    TC hc[h_len];
    unsigned int i;
    for (i=0; i<h_len; i++)
        hc[i] = h[i];

    
    unsigned int p = 2*_m;
    FIRPFBCH() q = FIRPFBCH(_create)(_type, _M, p, hc);

    
    return q;
}








FIRPFBCH() FIRPFBCH(_create_rnyquist)(int          _type,
                                      unsigned int _M,
                                      unsigned int _m,
                                      float        _beta,
                                      int          _ftype)
{
    
    if (_type != LIQUID_ANALYZER && _type != LIQUID_SYNTHESIZER)
        return liquid_error_config("firpfbch_%s_create_rnyquist(), invalid type: %d", EXTENSION_FULL, _type);
    if (_M == 0)
        return liquid_error_config("firpfbch_%s_create_rnyquist(), number of channels must be greater than 0", EXTENSION_FULL);
    if (_m == 0)
        return liquid_error_config("firpfbch_%s_create_rnyquist(), invalid filter size (must be greater than 0)", EXTENSION_FULL);
    
    
    unsigned int h_len = 2*_M*_m + 1;
    float h[h_len];
    if (liquid_firdes_prototype(_ftype, _M, _m, _beta, 0.0f, h) != LIQUID_OK)
        return liquid_error_config("firpfbch_%s_create_rnyquist(), invalid filter type/configuration", EXTENSION_FULL);

    
    
    unsigned int g_len = 2*_M*_m;
    TC gc[g_len];
    unsigned int i;
    if (_type == LIQUID_SYNTHESIZER) {
        for (i=0; i<g_len; i++)
            gc[i] = h[i];
    } else {
        for (i=0; i<g_len; i++)
            gc[i] = h[g_len-i-1];
    }

    
    unsigned int p = 2*_m;
    FIRPFBCH() q = FIRPFBCH(_create)(_type, _M, p, gc);

    
    return q;
}


int FIRPFBCH(_destroy)(FIRPFBCH() _q)
{
    unsigned int i;

    
    for (i=0; i<_q->num_channels; i++) {
        DOTPROD(_destroy)(_q->dp[i]);
        WINDOW(_destroy)(_q->w[i]);
    }
    free(_q->dp);
    free(_q->w);

    
    FFT_DESTROY_PLAN(_q->fft);

    
    free(_q->h);
    FFT_FREE(_q->x);
    FFT_FREE(_q->X);

    
    free(_q);
    return LIQUID_OK;
}


int FIRPFBCH(_reset)(FIRPFBCH() _q)
{
    unsigned int i;
    for (i=0; i<_q->num_channels; i++) {
        WINDOW(_reset)(_q->w[i]);
        _q->x[i] = 0;
        _q->X[i] = 0;
    }
    _q->filter_index = _q->num_channels-1;
    return LIQUID_OK;
}


int FIRPFBCH(_print)(FIRPFBCH() _q)
{
    printf("<liquid.firpfbch, type=\"%s\", channels=%u, semilen=%u>\n",
        _q->type == LIQUID_ANALYZER ? "analyzer" : "synthesizer",
        _q->num_channels, _q->p);
    return LIQUID_OK;
}









int FIRPFBCH(_synthesizer_execute)(FIRPFBCH() _q,
                                   TI *       _x,
                                   TO *       _y)
{
    unsigned int i;

    
    memmove(_q->X, _x, _q->num_channels*sizeof(TI));

    
    FFT_EXECUTE(_q->fft);

    
    T * r;      
    for (i=0; i<_q->num_channels; i++) {
        WINDOW(_push)(_q->w[i], _q->x[i]);
        WINDOW(_read)(_q->w[i], &r);
        DOTPROD(_execute)(_q->dp[i], r, &_y[i]);

        
        
    }
    return LIQUID_OK;
}









int FIRPFBCH(_analyzer_execute)(FIRPFBCH() _q,
                                TI *       _x,
                                TO *       _y)
{
    unsigned int i;

    
    for (i=0; i<_q->num_channels; i++)
        FIRPFBCH(_analyzer_push)(_q, _x[i]);

    
    
    return FIRPFBCH(_analyzer_run)(_q, 0, _y);
}









int FIRPFBCH(_analyzer_push)(FIRPFBCH() _q,
                             TI         _x)
{
    
    WINDOW(_push)(_q->w[_q->filter_index], _x);

    
    _q->filter_index = (_q->filter_index + _q->num_channels - 1) % _q->num_channels;
    return LIQUID_OK;
}





int FIRPFBCH(_analyzer_run)(FIRPFBCH()   _q,
                            unsigned int _k,
                            TO *         _y)
{
    unsigned int i;

    
    
    T * r;  
    unsigned int index;
    for (i=0; i<_q->num_channels; i++) {
        
        index = (i+_k) % _q->num_channels;

        
        WINDOW(_read)(_q->w[index], &r);

        
        DOTPROD(_execute)(_q->dp[i], r, &_q->X[_q->num_channels-i-1]);
    }

    
    FFT_EXECUTE(_q->fft);

    
    memmove(_y, _q->x, _q->num_channels*sizeof(TO));
    return LIQUID_OK;
}

