/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
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
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
	} tab;
	
/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
        {"id", 	        id},
	{"number", 	number},
	{":=", 	        assign},
	{"undef", 	undef},
	{"predef", 	predef},
	{"tempty", 	tempty},
	{"error",        error},
	{"type",         typ},
	{"$",            '$'},
	{"(",            '('},
	{")",            ')'},
	{"*",            '*'},
	{"+",            '+'},
	{",",            ','},
	{"-",            '-'},
	{".",            '.'},
	{"/",            '/'},
	{":",            ':'},
	{";",            ';'},
	{"=",            '='},
	{"TERROR", 	nfound}
        };


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	input},
	{"output", 	output},
	{"var", 	var},
	{"begin", 	begin},
	{"end", 	end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	real},
	{"KERROR", 	nfound}
	} ;
   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{  
    printf("\nTHE PROGRAM KEYWORDS");
    for(int i = 0; i< (kend - kstart-1); i++)
    {
        printf("\n%s        %i", keywordtab[i].text, keywordtab[i].token);
    }

    printf("\n\nTHE PROGRAM TOKENS");
    for(int i = 0; i < (tend- tstart + 11); i++)
    {
        printf("\n%s        %i", tokentab[i].text, tokentab[i].token);
    }
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
    //Checks for numbers -> Starts with number = numerical
    if(fplex[0] < 58 && fplex[0] > 47)
    {
        return number;
    }

    //Checks for tokens - Skips ID and number which are handled seperately
    for(int i = 1; i<20; i++)
    {
        if(strcmp(fplex, tokentab[i].text) == 0) return tokentab[i].token;
    }

    //Checks for keywords
    for(int i = 0; i<9; i++)
    {
        if(strcmp(fplex, keywordtab[i].text) == 0) return keywordtab[i].token;
    }
    return 0;
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{   
   for(int i = 0; i<9; i++)
   {
       if(strcmp(fplex, keywordtab[i].text) == 0) return keywordtab[i].token;
   }
   
   return id;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
    //Checking for tokens
   for(int i = 0; i<20; i++)
   {
       if(ftok == tokentab[i].token)
       {
           return tokentab[i].text;
       } 
   }

    //Checking for keywords
   for(int i = 0; i<(kend - kstart -1); i++)
   {
       if(ftok == keywordtab[i].token)
       {
           return keywordtab[i].text;
       } 
   }

   return "-1";
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
