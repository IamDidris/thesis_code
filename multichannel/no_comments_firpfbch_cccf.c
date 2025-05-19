





#include "liquid.internal.h"


#define EXTENSION_SHORT     "f"
#define EXTENSION_FULL      "cccf"


#define FIRPFBCH(name)      LIQUID_CONCAT(firpfbch_cccf,name)

#define T                   float complex   
#define TO                  float complex   
#define TC                  float complex   
#define TI                  float complex   
#define WINDOW(name)        LIQUID_CONCAT(windowcf,name)
#define DOTPROD(name)       LIQUID_CONCAT(dotprod_cccf,name)

#define TO_COMPLEX          1
#define TC_COMPLEX          1
#define TI_COMPLEX          1

#define PRINTVAL_TO(X,F)    PRINTVAL_CFLOAT(X,F)
#define PRINTVAL_TC(X,F)    PRINTVAL_CFLOAT(X,F)
#define PRINTVAL_TI(X,F)    PRINTVAL_CFLOAT(X,F)


#include "firpfbch.proto.c"

