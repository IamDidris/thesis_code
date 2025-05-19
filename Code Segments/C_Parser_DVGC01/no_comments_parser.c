






#include <stdio.h>
#include <ctype.h>
#include <string.h>




#include "keytoktab.h"         
#include "lexer.h"              
#include "symtab.h"             
#include "optab.h"              




#define DEBUG 0
static int  lookahead=0;
static int  is_parse_ok=1;














static void printIn(char * functionName)
{
    if(DEBUG) printf("\n *** In %s", functionName);
}

static void printOut(char * functionName)
{
    if(DEBUG) printf("\n *** Out %s", functionName);
}




static void match(int t)
{
    if(DEBUG) printf("\n --------In match expected: %4d, found: %4d",
                    t, lookahead);
    if (lookahead == t) lookahead = get_token();
    else {
      is_parse_ok=0;
      printf("\nSYNTAX: Symbol expected %4s found %4s",
              tok2lex(t), get_lexeme());
      }
    printOut("Match");
}




/



int parser()
{
    printIn("parser");

   lookahead = get_token();       

    if(lookahead != '$')
    {
        program_header();               
        var_post();                      
        stat_part();                     

        
        if(lookahead != '$')
        {
            printf("\nSYNTAX: Extra symbols after end of parse!\n");    
            while(lookahead != '$')
            {
                printf("%s ", get_lexeme());
                lookahead = get_token();
            }
        } 

    
    } else
    {
        printf("\nSYNTAX: Input file is empty");
        is_parse_ok = 0;
    }
    if(is_parse_ok) printf("\nPARSE SUCCESSFUL!");
    printf("\n____________________________________________________________");
    p_symtab();
    
    printOut("parser");
    return is_parse_ok;             
   
}



