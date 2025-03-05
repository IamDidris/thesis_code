/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
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
#define BUFSIZE 1024
#define LEXSIZE   30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/


/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{
    int i = 0; 
    while((buffer[i]=fgetc(stdin))!=EOF)
    {
        i++;
    }
    buffer[i++] = '$';
    buffer[i] = '\0';
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/  

static void pbuffer()
{   
    printf("\n____________________________________________________________");
    printf("\n  THE PROGRAM TEXT");
    printf("\n____________________________________________________________");
    printf("\n%s", buffer);
    printf("\n____________________________________________________________");
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{   
   lexbuf[plex++] = buffer[pbuf++];
   }

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{  
    /* 
    1. Whitespace: Blank, tab, newline - Ignored
    2. Alphanumeric: Begins with a letter - Key2Tok
    3. Numeric string: Begins with a number - Number
    4. Other strings: Begins with non-letter/number - Lex2Tok
    */

    if(pbuf == 0)
    {
        get_prog();
        pbuffer();
    } 
    memset(lexbuf, '\0', LEXSIZE);       //reset buffer
    plex = 0; //Resets buffer pointer

    //Skips all spaces in buffer
    while(isspace(buffer[pbuf])) pbuf++; 

    get_char(); //Gets the first letter

    //Checks for numericals
    if(isdigit(lexbuf[0]))
    {
        while(!isspace(buffer[pbuf]) && isdigit(buffer[pbuf])) //-<-Kan leda till konstigt bryt. Eftersom att vi slutar om det kommer en bokstav - Ex 1123asdasf
        {
            get_char();
        }
        return number;
    }
    //Checks for alphanumericals
    else if(isalpha(lexbuf[0]))
    {
        //Look for keywords
        while(!isspace(buffer[pbuf]) && (isalpha(buffer[pbuf]) || isdigit(buffer[pbuf]))) //-<- Bör tillåta allt förutom konstiga tecken
        {
            get_char();
        }
        return key2tok(lexbuf);
        
    } //All other scenarios
    else{
        if((lexbuf[0] == ':') && (buffer[pbuf] == '='))
        {
            get_char();
        } 
        return lex2tok(lexbuf);
    }
    return 0;
   }

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{  
   return lexbuf;
   }

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/