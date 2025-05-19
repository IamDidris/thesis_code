





#include "liquid.internal.h"


#define QUANTIZER(name)     LIQUID_CONCAT(quantizercf,name)

#define T                   float complex   

#define T_COMPLEX           1


#include "quantizer.proto.c"

