








#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "liquid.internal.h"


struct FIRPFBCH2(_s) {
    int          type;  
    unsigned int M;     
    unsigned int M2;    
    unsigned int m;     

    
    unsigned int h_len; 
    
    
    DOTPROD() * dp;     

    
    FFT_PLAN ifft;      
    TO * X;             
    TO * x;             

    
    
    WINDOW() * w0;      
    WINDOW() * w1;      
    int flag;           
};







FIRPFBCH2() FIRPFBCH2(_create)(int          _type,
                               unsigned int _M,
                               unsigned int _m,
                               TC *         _h)
{
    
    if (_type != LIQUID_ANALYZER && _type != LIQUID_SYNTHESIZER)
        return liquid_error_config("firpfbch2_%s_create(), invalid type %d", EXTENSION_FULL, _type);
    if (_M < 2 || _M % 2)
        return liquid_error_config("firpfbch2_%s_create(), number of channels must be greater than 2 and even", EXTENSION_FULL);
    if (_m < 1)
        return liquid_error_config("firpfbch2_%s_create(), filter semi-length must be at least 1", EXTENSION_FULL);

    
    FIRPFBCH2() q = (FIRPFBCH2()) malloc(sizeof(struct FIRPFBCH2(_s)));

    
    q->type     = _type;        
    q->M        = _M;           
    q->m        = _m;           

    
    q->h_len    = 2*q->M*q->m;  
    q->M2       = q->M / 2;     

    
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

    
    q->w0 = (WINDOW()*) malloc((q->M)*sizeof(WINDOW()));
    q->w1 = (WINDOW()*) malloc((q->M)*sizeof(WINDOW()));
    for (i=0; i<q->M; i++) {
        q->w0[i] = WINDOW(_create)(h_sub_len);
        q->w1[i] = WINDOW(_create)(h_sub_len);
    }

    
    FIRPFBCH2(_reset)(q);
    return q;
}






FIRPFBCH2() FIRPFBCH2(_create_kaiser)(int          _type,
                                      unsigned int _M,
                                      unsigned int _m,
                                      float        _as)
{
    
    if (_type != LIQUID_ANALYZER && _type != LIQUID_SYNTHESIZER)
        return liquid_error_config("firpfbch2_%s_create_kaiser(), invalid type %d", EXTENSION_FULL, _type);
    if (_M < 2 || _M % 2)
        return liquid_error_config("firpfbch2_%s_create_kaiser(), number of channels must be greater than 2 and even", EXTENSION_FULL);
    if (_m < 1)
        return liquid_error_config("firpfbch2_%s_create_kaiser(), filter semi-length must be at least 1", EXTENSION_FULL);

    
    unsigned int h_len = 2*_M*_m+1;
    float * hf = (float*)malloc(h_len*sizeof(float));

    
    
    float fc = (_type == LIQUID_ANALYZER) ? 1.0f/(float)_M : 0.5f/(float)_M;

    
    liquid_firdes_kaiser(h_len, fc, _as, 0.0f, hf);

    
    float hf_sum = 0.0f;
    unsigned int i;
    for (i=0; i<h_len; i++) hf_sum += hf[i];
    for (i=0; i<h_len; i++) hf[i] = hf[i] * (float)_M / hf_sum;

    
    TC * h = (TC*) malloc(h_len * sizeof(TC));
    for (i=0; i<h_len; i++)
        h[i] = (TC) hf[i];

    
    FIRPFBCH2() q = FIRPFBCH2(_create)(_type, _M, _m, h);

    
    free(hf);
    free(h);

    
    return q;
}


FIRPFBCH2() FIRPFBCH2(_copy)(FIRPFBCH2() q_orig)
{
    
    if (q_orig == NULL)
        return liquid_error_config("firfilt_%s_copy(), object cannot be NULL", EXTENSION_FULL);

    
    FIRPFBCH2() q_copy = (FIRPFBCH2()) malloc(sizeof(struct FIRPFBCH2(_s)));
    memmove(q_copy, q_orig, sizeof(struct FIRPFBCH2(_s)));

    
    unsigned int i;
    q_copy->dp = (DOTPROD()*) malloc((q_copy->M)*sizeof(DOTPROD()));
    for (i=0; i<q_copy->M; i++)
        q_copy->dp[i] = DOTPROD(_copy)(q_orig->dp[i]);

    
    q_copy->X = (T*) FFT_MALLOC((q_copy->M)*sizeof(T));   
    q_copy->x = (T*) FFT_MALLOC((q_copy->M)*sizeof(T));   
    q_copy->ifft = FFT_CREATE_PLAN(q_copy->M, q_copy->X, q_copy->x, FFT_DIR_BACKWARD, FFT_METHOD);

    
    q_copy->w0 = (WINDOW()*) malloc((q_copy->M)*sizeof(WINDOW()));
    q_copy->w1 = (WINDOW()*) malloc((q_copy->M)*sizeof(WINDOW()));
    for (i=0; i<q_copy->M; i++) {
        q_copy->w0[i] = WINDOW(_copy)(q_orig->w0[i]);
        q_copy->w1[i] = WINDOW(_copy)(q_orig->w1[i]);
    }

    return q_copy;
}


int FIRPFBCH2(_destroy)(FIRPFBCH2() _q)
{
    unsigned int i;

    
    for (i=0; i<_q->M; i++)
        DOTPROD(_destroy)(_q->dp[i]);
    free(_q->dp);

    
    FFT_DESTROY_PLAN(_q->ifft);
    FFT_FREE(_q->X);
    FFT_FREE(_q->x);
    
    
    for (i=0; i<_q->M; i++) {
        WINDOW(_destroy)(_q->w0[i]);
        WINDOW(_destroy)(_q->w1[i]);
    }
    free(_q->w0);
    free(_q->w1);

    
    free(_q);
    return LIQUID_OK;
}


int FIRPFBCH2(_reset)(FIRPFBCH2() _q)
{
    unsigned int i;

    
    for (i=0; i<_q->M; i++) {
        WINDOW(_reset)(_q->w0[i]);
        WINDOW(_reset)(_q->w1[i]);
    }

    
    _q->flag = 0;
    return LIQUID_OK;
}


int FIRPFBCH2(_print)(FIRPFBCH2() _q)
{
    printf("<liquid.firpfbch2, type=\"%s\", channels=%u, semilen=%u>\n",
        _q->type == LIQUID_ANALYZER ? "analyzer" : "synthesizer",
        _q->M, _q->m);
    return LIQUID_OK;
}


int FIRPFBCH2(_get_type)(FIRPFBCH2() _q)
{
    return _q->type;
}


unsigned int FIRPFBCH2(_get_M)(FIRPFBCH2() _q)
{
    return _q->M;
}


unsigned int FIRPFBCH2(_get_m)(FIRPFBCH2() _q)
{
    return _q->m;
}




int FIRPFBCH2(_execute_analyzer)(FIRPFBCH2() _q,
                                 TI *        _x,
                                 TO *        _y)
{
    unsigned int i;

    
    
    
    unsigned int base_index = _q->flag ? _q->M : _q->M2;
    for (i=0; i<_q->M2; i++) {
        
        WINDOW(_push)(_q->w0[base_index-i-1], _x[i]);
    }

    
    unsigned int offset = _q->flag ? _q->M2 : 0;
    TI * r;      
    for (i=0; i<_q->M; i++) {
        
        WINDOW(_read)(_q->w0[i], &r);

        
        DOTPROD(_execute)(_q->dp[(offset+i)%_q->M], r, &_q->X[i]);
    }

    
    FFT_EXECUTE(_q->ifft);

    
    for (i=0; i<_q->M; i++)
        _y[i] = _q->x[i] / (float)(_q->M);

    
    _q->flag = 1 - _q->flag;
    return LIQUID_OK;
}




int FIRPFBCH2(_execute_synthesizer)(FIRPFBCH2() _q,
                                    TI *        _x,
                                    TO *        _y)
{
    unsigned int i;

    
    memmove(_q->X, _x, _q->M * sizeof(TI));

    
    FFT_EXECUTE(_q->ifft);

    
    
    for (i=0; i<_q->M; i++)
        _q->x[i] *= 1.0f / (float)(_q->M);
    
    for (i=0; i<_q->M; i++)
        _q->x[i] *= (float)(_q->M2);

    
    WINDOW() * buffer = (_q->flag == 0 ? _q->w1 : _q->w0);
    for (i=0; i<_q->M; i++)
        WINDOW(_push)(buffer[i], _q->x[i]);

    
    TO * r0, * r1;  
    TO   y0,   y1;  
    for (i=0; i<_q->M2; i++) {
        
        unsigned int b = (_q->flag == 0) ? i : i+_q->M2;

        
        WINDOW(_read)(_q->w0[b], &r0);
        WINDOW(_read)(_q->w1[b], &r1);

        
        TO * p0 = _q->flag ? r0 : r1;
        TO * p1 = _q->flag ? r1 : r0;

        
        DOTPROD(_execute)(_q->dp[i],        p0, &y0);
        DOTPROD(_execute)(_q->dp[i+_q->M2], p1, &y1);

        
        _y[i] = y0 + y1;
    }
    _q->flag = 1 - _q->flag;
    return LIQUID_OK;
}






int FIRPFBCH2(_execute)(FIRPFBCH2() _q,
                        TI *        _x,
                        TO *        _y)
{
    switch (_q->type) {
    case LIQUID_ANALYZER:
        return FIRPFBCH2(_execute_analyzer)(_q, _x, _y);
    case LIQUID_SYNTHESIZER:
        return FIRPFBCH2(_execute_synthesizer)(_q, _x, _y);
    default:;
    }
    return liquid_error(LIQUID_EINT,"firpfbch2_%s_execute(), invalid internal type", EXTENSION_FULL);
}

