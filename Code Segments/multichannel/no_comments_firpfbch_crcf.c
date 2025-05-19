





#include "liquid.internal.h"


#define EXTENSION_SHORT     "f"
#define EXTENSION_FULL      "crcf"


#define FIRPFBCH(name)      LIQUID_CONCAT(firpfbch_crcf,name)
#define FIRPFBCH2(name)     LIQUID_CONCAT(firpfbch2_crcf,name)
#define FIRPFBCHR(name)     LIQUID_CONCAT(firpfbchr_crcf,name)

#define T                   float complex   
#define TO                  float complex   
#define TC                  float           
#define TI                  float complex   
#define WINDOW(name)        LIQUID_CONCAT(windowcf,name)
#define DOTPROD(name)       LIQUID_CONCAT(dotprod_crcf,name)

#define TO_COMPLEX          1
#define TC_COMPLEX          0
#define TI_COMPLEX          1

#define PRINTVAL_TO(X,F)    PRINTVAL_CFLOAT(X,F)
#define PRINTVAL_TC(X,F)    PRINTVAL_FLOAT(X,F)
#define PRINTVAL_TI(X,F)    PRINTVAL_CFLOAT(X,F)


#include "firpfbch.proto.c"     
#include "firpfbch2.proto.c"    
#include "firpfbchr.proto.c"    

