






#include <stdio.h>
#include <ctype.h>
#include <string.h>




#include "keytoktab.h"




#define NENTS 4

static int optab[][NENTS] = {
   {'+', integer, integer, integer},
   {'+', real,    real,    real},
   {'+', integer, real,    real},
   {'+', real,    integer, real},
   {'*', integer, integer, integer},
   {'*', real,    real,    real},
   {'*', integer, real,    real},
   {'*', real,    integer, real},
   {'$', undef,   undef,   undef}
   };




static void p_optabLine(int index)
{
    printf("\n%10s %10s %10s %10s", tok2lex(optab[index][0]), tok2lex(optab[index][1]), tok2lex(optab[index][2]), tok2lex(optab[index][3]));
}







void p_optab()
{
    int i = 0;
    printf("\n____________________________________________________________");
    printf("\n  THE OPERATOR TABLE");
    printf("\n____________________________________________________________");
    printf("\n%10s %10s %10s %10s", "operator", "arg1", "arg2", "result");
    printf("\n____________________________________________________________");
    while(optab[i][0] != '$')
    {
        p_optabLine(i++);
    }
    printf("\n____________________________________________________________\n\n");
}




int get_otype(int op, int arg1, int arg2)
{  int i = 0;
   while(optab[i][0] != '$')
   {
       if(op == optab[i][0] && arg1 == optab[i][1] && arg2 == optab[i][2])
       {
           return optab[i][3];
       } 
       i++;
   }
    return optab[i][3];
}



