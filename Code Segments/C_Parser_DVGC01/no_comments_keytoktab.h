



#ifndef KEYTOK_H
#define KEYTOK_H




#define nfound -1

typedef enum tvalues { tstart=257,
                  id, number, assign, predef, tempty, undef, error, typ,
                  tend,
               kstart,
                  program, input, output, var,
                  begin, end, boolean, integer, real,
                  kend 
                  } toktyp;





void   p_toktab();                 
toktyp lex2tok(char * fplex);      
toktyp key2tok(char * fplex);      
char * tok2lex(toktyp ftok);       

#endif




