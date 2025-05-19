







#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "liquid.internal.h"







int ofdmframe_init_S0(unsigned char * _p,
                      unsigned int    _M,
                      float complex * _S0,
                      float complex * _s0,
                      unsigned int *  _M_S0)
{
    unsigned int i;

    
    unsigned int m = liquid_nextpow2(_M);
    if (m < 4)      m = 4;
    else if (m > 8) m = 8;

    
    msequence ms = msequence_create_default(m);

    unsigned int s;
    unsigned int M_S0 = 0;

    
    for (i=0; i<_M; i++) {
        
        
        s = msequence_generate_symbol(ms,3) & 0x01;

        if (_p[i] == OFDMFRAME_SCTYPE_NULL) {
            
            _S0[i] = 0.0f;
        } else {
            if ( (i%2) == 0 ) {
                
                _S0[i] = s ? 1.0f : -1.0f;
                M_S0++;
            } else {
                
                _S0[i] = 0.0f;
            }
        }
    }

    
    msequence_destroy(ms);

    
    if (M_S0 == 0)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_init_S0(), no subcarriers enabled; check allocation");

    
    *_M_S0 = M_S0;

    
    fft_run(_M, _S0, _s0, LIQUID_FFT_BACKWARD, 0);

    
    float g = 1.0f / sqrtf(M_S0);
    for (i=0; i<_M; i++)
        _s0[i] *= g;
    return LIQUID_OK;
}








int ofdmframe_init_S1(unsigned char * _p,
                      unsigned int    _M,
                      float complex * _S1,
                      float complex * _s1,
                      unsigned int *  _M_S1)
{
    unsigned int i;

    
    unsigned int m = liquid_nextpow2(_M);
    if (m < 4)      m = 4;
    else if (m > 8) m = 8;

    
    
    
    m++;

    
    msequence ms = msequence_create_default(m);

    unsigned int s;
    unsigned int M_S1 = 0;

    
    for (i=0; i<_M; i++) {
        
        
        s = msequence_generate_symbol(ms,3) & 0x01;

        if (_p[i] == OFDMFRAME_SCTYPE_NULL) {
            
            _S1[i] = 0.0f;
        } else {
            _S1[i] = s ? 1.0f : -1.0f;
            M_S1++;
        }
    }

    
    msequence_destroy(ms);

    
    if (M_S1 == 0)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_init_S1(), no subcarriers enabled; check allocation");

    
    *_M_S1 = M_S1;

    
    fft_run(_M, _S1, _s1, LIQUID_FFT_BACKWARD, 0);

    
    float g = 1.0f / sqrtf(M_S1);
    for (i=0; i<_M; i++)
        _s1[i] *= g;
    return LIQUID_OK;
}








int ofdmframe_init_default_sctype(unsigned int    _M,
                                  unsigned char * _p)
{
    
    if (_M < 6)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_init_default_sctype(), less than 6 subcarriers");

    unsigned int i;
    unsigned int M2 = _M/2;

    
    unsigned int G = _M / 10;
    if (G < 2) G = 2;

    
    unsigned int P = (_M > 34) ? 8 : 4;
    unsigned int P2 = P/2;

    
    for (i=0; i<_M; i++)
        _p[i] = OFDMFRAME_SCTYPE_NULL;

    
    for (i=1; i<M2-G; i++) {
        if ( ((i+P2)%P) == 0 )
            _p[i] = OFDMFRAME_SCTYPE_PILOT;
        else
            _p[i] = OFDMFRAME_SCTYPE_DATA;
    }

    
    for (i=1; i<M2-G; i++) {
        unsigned int k = _M - i;
        if ( ((i+P2)%P) == 0 )
            _p[k] = OFDMFRAME_SCTYPE_PILOT;
        else
            _p[k] = OFDMFRAME_SCTYPE_DATA;
    }
    return LIQUID_OK;
}






int ofdmframe_init_sctype_range(unsigned int    _M,
                                float           _f0,
                                float           _f1,
                                unsigned char * _p)
{
    
    if (_M < 6)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_init_sctype_range(), less than 6 subcarriers");
    if (_f0 < -0.5f || _f0 > 0.5f)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_init_sctype_range(), lower frequency edge must be in [-0.5,0.5]");
    if (_f1 < -0.5f || _f1 > 0.5f)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_init_sctype_range(), upper frequency edge must be in [-0.5,0.5]");
    if (_f0 >= _f1)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_init_sctype_range(), lower frequency edge must be below upper edge");

    
    int M0 = (int)((_f0 + 0.5f) * _M); 
    int M1 = (int)((_f1 + 0.5f) * _M); 
    int Mp = M1 - M0;
    if (Mp > (int)_M) {
        Mp = (int)_M;
    } else if (Mp < 6) {
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_init_sctype_range(), less than 6 subcarriers (effectively)");
    }

    
    unsigned int P = (Mp > 34) ? 8 : 4;

    
    int i;
    for (i=0; i<(int)_M; i++) {
        
        unsigned int k = ((unsigned int)i + _M/2) % _M;
        if (i < M0 || i > M1) {
            
            _p[k] = OFDMFRAME_SCTYPE_NULL;
        } else if ( (k%P)==0 ) {
            _p[k] = OFDMFRAME_SCTYPE_PILOT;
        } else {
            _p[k] = OFDMFRAME_SCTYPE_DATA;
        }
    }
    return LIQUID_OK;
}








int ofdmframe_validate_sctype(unsigned char * _p,
                              unsigned int    _M,
                              unsigned int *  _M_null,
                              unsigned int *  _M_pilot,
                              unsigned int *  _M_data)
{
    
    unsigned int M_null  = 0;
    unsigned int M_pilot = 0;
    unsigned int M_data  = 0;

    unsigned int i;
    for (i=0; i<_M; i++) {
        
        if (_p[i] == OFDMFRAME_SCTYPE_NULL)
            M_null++;
        else if (_p[i] == OFDMFRAME_SCTYPE_PILOT)
            M_pilot++;
        else if (_p[i] == OFDMFRAME_SCTYPE_DATA)
            M_data++;
        else {
            return liquid_error(LIQUID_EICONFIG,"ofdmframe_validate_sctype(), invalid subcarrier type (%u)", _p[i]);
        }
    }

    
    if ( (M_pilot + M_data) == 0)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_validate_sctype(), must have at least one enabled subcarrier");
    if (M_data == 0)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_validate_sctype(), must have at least one data subcarrier");
    if (M_pilot < 2)
        return liquid_error(LIQUID_EICONFIG,"ofdmframe_validate_sctype(), must have at least two pilot subcarriers");

    
    if (_M_null  != NULL) *_M_null  = M_null;
    if (_M_pilot != NULL) *_M_pilot = M_pilot;
    if (_M_data  != NULL) *_M_data  = M_data;
    return LIQUID_OK;
}






int ofdmframe_print_sctype(unsigned char * _p,
                           unsigned int    _M)
{
    unsigned int i;

    printf("[");
    for (i=0; i<_M; i++) {
        unsigned int k = (i + _M/2) % _M;

        switch (_p[k]) {
        case OFDMFRAME_SCTYPE_NULL:  printf("."); break;
        case OFDMFRAME_SCTYPE_PILOT: printf("|"); break;
        case OFDMFRAME_SCTYPE_DATA:  printf("+"); break;
        default:
            return liquid_error(LIQUID_EICONFIG,"ofdmframe_print_default_sctype(), invalid subcarrier type");
        }
    }
    printf("]\n");
    return LIQUID_OK;
}

