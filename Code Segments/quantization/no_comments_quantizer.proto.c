





#include <stdio.h>
#include <stdlib.h>

struct QUANTIZER(_s) {
    int ctype;          
    unsigned int n;     
                        
};





QUANTIZER() QUANTIZER(_create)(liquid_compander_type _ctype,
                               float                 _range,
                               unsigned int          _num_bits)
{
    
    if (_num_bits == 0)
        return liquid_error_config("quantizer_create(), must have at least one bit/sample");

    
    QUANTIZER() q = (QUANTIZER()) malloc(sizeof(struct QUANTIZER(_s)));

    
    q->ctype = _ctype;
    q->n     = _num_bits;

    
    return q;
}

int QUANTIZER(_destroy)(QUANTIZER() _q)
{
    
    free(_q);
    return LIQUID_OK;
}

int QUANTIZER(_print)(QUANTIZER() _q)
{
    printf("quantizer:\n");
    printf("  compander :   ");
    switch(_q->ctype) {
    case LIQUID_COMPANDER_NONE:     printf("none\n");   break;
    case LIQUID_COMPANDER_LINEAR:   printf("linear\n"); break;
    case LIQUID_COMPANDER_MULAW:    printf("mu-law\n"); break;
    case LIQUID_COMPANDER_ALAW:     printf("A-law\n");  break;
    default:
        printf("unknown\n");
    }
    printf("  num bits  :   %u\n", _q->n);
    return LIQUID_OK;
}

int QUANTIZER(_execute_adc)(QUANTIZER() _q,
                             T _x,
                             unsigned int * _sample)
{
#if T_COMPLEX
#else
#endif
    *_sample = 0;
    return LIQUID_OK;
}

int QUANTIZER(_execute_dac)(QUANTIZER() _q,
                             unsigned int _sample,
                             T * _x)
{
#if T_COMPLEX
#else
#endif
    *_x = 0.0;
    return LIQUID_OK;
}


