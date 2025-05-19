/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"         /* when the keytoktab is added   */
#include "lexer.h"              /* when the lexer     is added   */
#include "symtab.h"             /* when the symtab    is added   */
#include "optab.h"              /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
//enum tvalues { program = 257, id, input, output, var, integer, begin, equals, numeric, end };
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void printIn(char * functionName)
{
    if(DEBUG) printf("\n *** In %s", functionName);
}

static void printOut(char * functionName)
{
    if(DEBUG) printf("\n *** Out %s", functionName);
}

/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
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

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
//************************ program  ************************************
static void program_header()
{
   printIn("program_header");
   match(program); 
  if(lookahead == id)
  {
      addp_name(get_lexeme());
      match(id);
  }else
  {
      printf("\nSYNTAX: ID expected found %s", get_lexeme());
      addp_name("???");
      is_parse_ok = 0;
  }
  
   match('('); match(input);
   match(','); match(output);
   match(')'); match(';');

   printOut("program_header");
}


//********************** VAR    ***************************************
static void type()
{
    printIn("type");

    switch(lookahead){
        case integer:
            setv_type(integer);
            match(integer);
            break;

        case real:
            setv_type(real);
            match(real);
            break;

        case boolean:
            setv_type(boolean);
            match(boolean);
            break;
        
        default: 
            printf("\nSYNTAX: Type name expected found %s", get_lexeme());
            is_parse_ok = 0;
            setv_type(error);
            break;
    }

    printOut("type");

}

static void id_list()
{
    printIn("id_list");
    if(lookahead == id)
    {
        char * var_name = get_lexeme();
        if(!find_name(var_name)) addv_name(var_name);
        else
        {
            printf("\nSEMANTIC: ID already declared: %s", get_lexeme());
            is_parse_ok = 0;
        } 
    }
    match(id);

    if(lookahead == ',')
    {
        match(',');
        id_list();
    }
    

    printOut("id_list");
}

static void var_dec()
{
    printIn("var_dec");

    id_list();
    match(':');
    type();
    match(';');

    printOut("var_dec");

}

static void var_dec_list()
{
    printIn("var_dec_list");

    var_dec();
    if(lookahead == id)
    {
        var_dec_list();
    }

    printOut("var_dec_list");
}

static void var_post()
{
    printIn("var_post");

    match(var);
    var_dec_list();

    printOut("var_post");
}


//**************************' STAT_PART ***********************************
static toktyp stat_expr();

static toktyp stat_operand()
{
    printIn("stat_operand");

    toktyp varType = error;

    if(lookahead == id)
    {
        char * var_name;
        var_name = get_lexeme();
        
        if(find_name(var_name))
        {
            varType = get_ntype(var_name);
            match(id);
        } 
        else {
            printf("\nSEMANTIC: ID NOT declared: %s", get_lexeme());
            match(id);
            varType = undef;
        }
    } else if(lookahead == number) {
        match(number);
        varType = integer; 
    } else
    {   
        printf("\nSYNTAX: Operand expected");
        is_parse_ok = 0;
        varType = error;
    }

    printOut("stat_operand");
    return varType;
    
}

static toktyp stat_factor()
{
    printIn("stat_factor");

    toktyp type;
    if(lookahead == '(')
    {
        match('(');
        type = stat_expr();
        match(')');

    }else type = stat_operand();

    printOut("stat_factor");
    return type;
}


static toktyp stat_term()
{
    printIn("stat_term");

    toktyp leftSide = stat_factor();
    if(lookahead == '*')
    {
        match('*');
        leftSide = get_otype('*', leftSide, stat_term());
    }

    printOut("stat_term");
    return leftSide;
}


static toktyp stat_expr()
{
    printIn("stat_expr");

    toktyp leftSide = stat_term();
    if(lookahead == '+')
    {
        match('+');
        leftSide = get_otype('+', leftSide, stat_expr());
    }


    printOut("stat_expr");
    return leftSide;
}

static void assign_stat()
{
    printIn("assign_stat");

    char * leftLex = get_lexeme();
    toktyp leftSide = error;
    toktyp rightSide = error;

    if(lookahead == id)
    {
        if(find_name(leftLex)) leftSide = get_ntype(leftLex);
        else printf("\nSYNTAX: ID NOT declared: %s", leftLex);
        match(id);
    }else
    {
        printf("\nSYNTAX: ID expected found %s", get_lexeme());
        is_parse_ok = 0;
    }
    
    match(assign);

    rightSide = stat_expr();

    if(leftSide != rightSide)
    {
        printf("\nSEMANTIC: Assign types: %s := %s", tok2lex(leftSide), tok2lex(rightSide));
        is_parse_ok = 0;
    }

    printOut("assign_stat");
    
    
}

static void stat()
{
    printIn("stat");
    assign_stat();

    printOut("stat");
}

static void stat_list()
{
    printIn("stat_list");

    stat();
    if(lookahead == ';')
    {
        match(';');
        stat_list();
    }

    printOut("stat_list");
}

static void stat_part()
{
    printIn("stat_part");

    match(begin); 
    stat_list();
    match(end); 
    match('.'); 

    printOut("stat_part");
}
   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    printIn("parser");

   lookahead = get_token();       // get the first token

    if(lookahead != '$')
    {
        program_header();               // call the first grammar rule
        var_post();                      // call second grammar rule
        stat_part();                     //Call to third grammar rule

        //Check for symbols after end of program
        if(lookahead != '$')
        {
            printf("\nSYNTAX: Extra symbols after end of parse!\n");    
            while(lookahead != '$')
            {
                printf("%s ", get_lexeme());
                lookahead = get_token();
            }
        } 

    //When input file is empty
    } else
    {
        printf("\nSYNTAX: Input file is empty");
        is_parse_ok = 0;
    }
    if(is_parse_ok) printf("\nPARSE SUCCESSFUL!");
    printf("\n____________________________________________________________");
    p_symtab();
    
    printOut("parser");
    return is_parse_ok;             // status indicator
   
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/