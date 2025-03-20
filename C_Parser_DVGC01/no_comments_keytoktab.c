






#include <stdio.h>
#include <ctype.h>
#include <string.h>




#include "keytoktab.h"







typedef struct tab {
	char 	* text;
	int 	token;
	} tab;
	



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




toktyp lex2tok(char * fplex)
{
    
    if(fplex[0] < 58 && fplex[0] > 47)
    {
        return number;
    }

    
    for(int i = 1; i<20; i++)
    {
        if(strcmp(fplex, tokentab[i].text) == 0) return tokentab[i].token;
    }

    
    for(int i = 0; i<9; i++)
    {
        if(strcmp(fplex, keywordtab[i].text) == 0) return keywordtab[i].token;
    }
    return 0;
}




toktyp key2tok(char * fplex)
{   
   for(int i = 0; i<9; i++)
   {
       if(strcmp(fplex, keywordtab[i].text) == 0) return keywordtab[i].token;
   }
   
   return id;
}




char * tok2lex(toktyp ftok)
{
    
   for(int i = 0; i<20; i++)
   {
       if(ftok == tokentab[i].token)
       {
           return tokentab[i].text;
       } 
   }

    
   for(int i = 0; i<(kend - kstart -1); i++)
   {
       if(ftok == keywordtab[i].token)
       {
           return keywordtab[i].text;
       } 
   }

   return "-1";
}




