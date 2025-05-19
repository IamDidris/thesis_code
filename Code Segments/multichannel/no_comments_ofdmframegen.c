







#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "liquid.internal.h"

#define DEBUG_OFDMFRAMEGEN            1


int ofdmframegen_gensymbol(ofdmframegen    _q,
                           float complex * _buffer);

struct ofdmframegen_s {
    unsigned int M;         
    unsigned int cp_len;    
    unsigned char * p;      

    
    unsigned int taper_len; 
    float * taper;          
    float complex *postfix; 

    
    unsigned int M_null;    
    unsigned int M_pilot;   
    unsigned int M_data;    
    unsigned int M_S0;      
    unsigned int M_S1;      

    
    float g_data;           

    
    FFT_PLAN ifft;          
    float complex * X;      
    float complex * x;      

    
    float complex * S0;     
    float complex * s0;     

    
    float complex * S1;     
    float complex * s1;     

    
    msequence ms_pilot;
};






ofdmframegen ofdmframegen_create(unsigned int    _M,
                                 unsigned int    _cp_len,
                                 unsigned int    _taper_len,
                                 unsigned char * _p)
{
    
    if (_M < 8)
        return liquid_error_config("ofdmframegen_create(), number of subcarriers must be at least 8");
    if (_M % 2)
        return liquid_error_config("ofdmframegen_create(), number of subcarriers must be even");
    if (_cp_len > _M)
        return liquid_error_config("ofdmframegen_create(), cyclic prefix cannot exceed symbol length");
    if (_taper_len > _cp_len)
        return liquid_error_config("ofdmframegen_create(), taper length cannot exceed cyclic prefix");

    ofdmframegen q = (ofdmframegen) malloc(sizeof(struct ofdmframegen_s));
    q->M         = _M;
    q->cp_len    = _cp_len;
    q->taper_len = _taper_len;

    
    q->p = (unsigned char*) malloc((q->M)*sizeof(unsigned char));
    if (_p == NULL) {
        
        ofdmframe_init_default_sctype(q->M, q->p);
    } else {
        
        memmove(q->p, _p, q->M*sizeof(unsigned char));
    }

    
    if (ofdmframe_validate_sctype(q->p, q->M, &q->M_null, &q->M_pilot, &q->M_data))
        return liquid_error_config("ofdmframegen_create(), invalid subcarrier allocation");

    unsigned int i;

    
    q->X = (float complex*) FFT_MALLOC((q->M)*sizeof(float complex));
    q->x = (float complex*) FFT_MALLOC((q->M)*sizeof(float complex));
    q->ifft = FFT_CREATE_PLAN(q->M, q->X, q->x, FFT_DIR_BACKWARD, FFT_METHOD);

    
    q->S0 = (float complex*) malloc((q->M)*sizeof(float complex));
    q->s0 = (float complex*) malloc((q->M)*sizeof(float complex));
    q->S1 = (float complex*) malloc((q->M)*sizeof(float complex));
    q->s1 = (float complex*) malloc((q->M)*sizeof(float complex));
    ofdmframe_init_S0(q->p, q->M, q->S0, q->s0, &q->M_S0);
    ofdmframe_init_S1(q->p, q->M, q->S1, q->s1, &q->M_S1);

    
    q->taper   = (float*)         malloc(q->taper_len * sizeof(float));
    q->postfix = (float complex*) malloc(q->taper_len * sizeof(float complex));
    for (i=0; i<q->taper_len; i++) {
        float t = ((float)i + 0.5f) / (float)(q->taper_len);
        float g = sinf(M_PI_2*t);
        q->taper[i] = g*g;
    }
#if 0
    
    for (i=0; i<q->taper_len; i++) {
        printf("    taper[%2u] = %12.8f (%12.8f)\n", i, q->taper[i],
            q->taper[i] + q->taper[q->taper_len - i - 1]);
    }
#endif

    
    q->g_data = 1.0f / sqrtf(q->M_pilot + q->M_data);

    
    q->ms_pilot = msequence_create_default(8);

    return q;
}


int ofdmframegen_destroy(ofdmframegen _q)
{
    
    free(_q->p);

    
    FFT_FREE(_q->X);
    FFT_FREE(_q->x);
    FFT_DESTROY_PLAN(_q->ifft);

    
    free(_q->taper);
    free(_q->postfix);

    
    free(_q->S0);
    free(_q->s0);
    free(_q->S1);
    free(_q->s1);

    
    msequence_destroy(_q->ms_pilot);

    
    free(_q);
    return LIQUID_OK;
}

int ofdmframegen_print(ofdmframegen _q)
{
    printf("<liquid.ofdmframegen");
    printf(", subcarriers=%u", _q->M);
    printf(", null=%u", _q->M_null);
    printf(", pilot=%u", _q->M_pilot);
    printf(", data=%u", _q->M_data);
    printf(", cp=%u", _q->cp_len);
    printf(", taper=%u", _q->taper_len);
    printf(">\n");
    return LIQUID_OK;
}

int ofdmframegen_reset(ofdmframegen _q)
{
    msequence_reset(_q->ms_pilot);

    
    unsigned int i;
    for (i=0; i<_q->taper_len; i++)
        _q->postfix[i] = 0.0f;
    return LIQUID_OK;
}














int ofdmframegen_write_S0a(ofdmframegen    _q,
                           float complex * _y)
{
    unsigned int i;
    unsigned int k;
    for (i=0; i<_q->M + _q->cp_len; i++) {
        k = (i + _q->M - 2*_q->cp_len) % _q->M;
        _y[i] = _q->s0[k];
    }

    
    for (i=0; i<_q->taper_len; i++)
        _y[i] *= _q->taper[i];
    return LIQUID_OK;
}

int ofdmframegen_write_S0b(ofdmframegen _q,
                           float complex * _y)
{
    unsigned int i;
    unsigned int k;
    for (i=0; i<_q->M + _q->cp_len; i++) {
        k = (i + _q->M - _q->cp_len) % _q->M;
        _y[i] = _q->s0[k];
    }

    
    memmove(_q->postfix, _q->s0, _q->taper_len*sizeof(float complex));
    return LIQUID_OK;
}

int ofdmframegen_write_S1(ofdmframegen _q,
                           float complex * _y)
{
    
    memmove(_q->x, _q->s1, (_q->M)*sizeof(float complex));
    return ofdmframegen_gensymbol(_q, _y);
}






int ofdmframegen_writesymbol(ofdmframegen    _q,
                             float complex * _x,
                             float complex * _y)
{
    
    unsigned int i;
    unsigned int k;
    int sctype;
    for (i=0; i<_q->M; i++) {
        
        k = (i + _q->M/2) % _q->M;

        sctype = _q->p[k];
        if (sctype==OFDMFRAME_SCTYPE_NULL) {
            
            _q->X[k] = 0.0f;
        } else if (sctype==OFDMFRAME_SCTYPE_PILOT) {
            
            _q->X[k] = (msequence_advance(_q->ms_pilot) ? 1.0f : -1.0f) * _q->g_data;
        } else {
            
            _q->X[k] = _x[k] * _q->g_data;
        }

        
    }

    
    FFT_EXECUTE(_q->ifft);

    
    return ofdmframegen_gensymbol(_q, _y);
}


int ofdmframegen_writetail(ofdmframegen    _q,
                           float complex * _buffer)
{
    
    unsigned int i;
    for (i=0; i<_q->taper_len; i++)
        _buffer[i] = _q->postfix[i] * _q->taper[_q->taper_len-i-1];
    return LIQUID_OK;
}























int ofdmframegen_gensymbol(ofdmframegen    _q,
                           float complex * _buffer)
{
    
    memmove( &_buffer[0],          &_q->x[_q->M-_q->cp_len], _q->cp_len*sizeof(float complex));
    memmove( &_buffer[_q->cp_len], &_q->x[               0], _q->M    * sizeof(float complex));
    
    
    unsigned int i;
    for (i=0; i<_q->taper_len; i++) {
        _buffer[i] *= _q->taper[i];
        _buffer[i] += _q->postfix[i] * _q->taper[_q->taper_len-i-1];
    }

    
    memmove(_q->postfix, _q->x, _q->taper_len*sizeof(float complex));
    return LIQUID_OK;
}

