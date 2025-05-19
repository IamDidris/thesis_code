






#include <stdio.h>
#include <ctype.h>
#include <string.h>






#include "keytoktab.h"




#define BUFSIZE 1024
#define LEXSIZE   30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               
static int  plex  = 0;               













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



  

static void pbuffer()
{   
    printf("\n____________________________________________________________");
    printf("\n  THE PROGRAM TEXT");
    printf("\n____________________________________________________________");
    printf("\n%s", buffer);
    printf("\n____________________________________________________________");
}





static void get_char()
{   
   lexbuf[plex++] = buffer[pbuf++];
   }











int get_token()
{  
    

    if(pbuf == 0)
    {
        get_prog();
        pbuffer();
    } 
    memset(lexbuf, '\0', LEXSIZE);       
    plex = 0; 

    
    while(isspace(buffer[pbuf])) pbuf++; 

    get_char(); 

    
    if(isdigit(lexbuf[0]))
    {
        while(!isspace(buffer[pbuf]) && isdigit(buffer[pbuf])) 
        {
            get_char();
        }
        return number;
    }
    
    else if(isalpha(lexbuf[0]))
    {
        
        while(!isspace(buffer[pbuf]) && (isalpha(buffer[pbuf]) || isdigit(buffer[pbuf]))) 
        {
            get_char();
        }
        return key2tok(lexbuf);
        
    } 
    else{
        if((lexbuf[0] == ':') && (buffer[pbuf] == '='))
        {
            get_char();
        } 
        return lex2tok(lexbuf);
    }
    return 0;
   }




char * get_lexeme()
{  
   return lexbuf;
   }



