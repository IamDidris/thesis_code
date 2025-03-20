





#include "liquid.internal.h"


#define QUANTIZER(name)     LIQUID_CONCAT(quantizerf,name)

#define T                   float   

#define T_COMPLEX           0


#include "quantizer.proto.c"

