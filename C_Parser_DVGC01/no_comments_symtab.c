






#include <stdio.h>
#include <ctype.h>
#include <string.h>




#include "keytoktab.h"




#define TABSIZE 1024                   
#define NAMELEN   20                   

typedef char tname[NAMELEN];

static tname  name[TABSIZE];
static toktyp role[TABSIZE];
static toktyp type[TABSIZE];
static int    size[TABSIZE];
static int    addr[TABSIZE];

static int numrows=0;                  
static int startp =0;                  







static char * get_name(int ftref)   { return name[ftref]; }
static toktyp get_role(int ftref)   { return role[ftref]; }
static toktyp get_type(int ftref)   { return type[ftref]; }
static int    get_size(int ftref)   { return size[ftref]; }
static int    get_addr(int ftref)   { return addr[ftref]; }




static void set_name(int ftref, char * fpname) { strcpy(name[ftref], fpname);}
static void set_role(int ftref, toktyp frole)  { role[ftref] = frole; }
static void set_type(int ftref, toktyp ftype)  { type[ftref] = ftype; }
static void set_size(int ftref, int    fsize)  { size[ftref] = fsize; }
static void set_addr(int ftref, int    faddr)  { addr[ftref] = faddr; }




static void addrow(char *fname, toktyp frole, toktyp ftype, 
                   int fsize, int faddr)
{
    set_name(numrows, fname);
    set_role(numrows, frole);
    set_type(numrows, ftype);
    set_size(numrows, fsize);
    set_addr(numrows++, faddr);

}



static void initst()
{
    addrow(tok2lex(predef),  typ, predef, 0, 0);
    addrow(tok2lex(undef),   typ, predef, 0, 0);
    addrow(tok2lex(error),   typ, predef, 0, 0);
    addrow(tok2lex(integer), typ, predef, 4, 0);
    addrow(tok2lex(boolean), typ, predef, 4, 0);
    addrow(tok2lex(real),    typ, predef, 8, 0);
}




 static int get_ref(char * fpname)
 {
   for(int i =0; i<numrows; i++)
   {
       if(!strcmp(fpname, get_name(i)))
       {
           return i;
       }
   }
   return nfound;
 }








static void p_symrow(int ftref)
{
   printf("\n%10s   %10s  %10s  %5i  %5i", get_name(ftref), tok2lex(get_role(ftref)), tok2lex(get_type(ftref)), get_size(ftref), get_addr(ftref));

   }

void p_symtab()
{
    printf("\n____________________________________________________________");
    printf("\n  THE SYMBOL TABLE");
    printf("\n____________________________________________________________");
    printf("\n%10s %10s %10s %10s %10s", "Name", "Role", "Type", "Size", "Address");
    printf("\n____________________________________________________________");
    for(int i = startp; i<numrows; i++)
    {
        p_symrow(i);
    }
    printf("\n____________________________________________________________");
    printf("\n  STATIC STORAGE REQUIRED is %i BYTES", get_size(startp));
    printf("\n____________________________________________________________");
}




void addp_name(char * fpname) 
{ 
    initst();
    startp = numrows; 
    addrow(fpname, program, program, 0, 0);
    
}




void addv_name(char * fpname) 
{ 
    addrow(fpname, var, nfound, 0, 0); 
}





int find_name(char * fpname) 
{
    int result = get_ref(fpname);
    if(result == nfound) return 0;
    return 1; 
}




void setv_type(toktyp ftype) 
{
    int size = 0;

    if(ftype == integer || ftype == boolean) size = 4;
    else if (ftype == real) size = 8;

   for(int i = startp; i<numrows; i++)
   {
       if(get_type(i) == nfound)
       {
           set_type(i, ftype); 
           set_size(i, size); 
           
           set_size(startp, (get_size(startp) + size)); 

           
           if(i == startp+1)
           {
               set_addr(i, 0);
           } else set_addr(i, (get_size(i-1) + get_addr(i-1))); 

       } 
   }
}




toktyp get_ntype(char * fpname) 
{ 
    if(get_ref(fpname) != nfound) return get_type(get_ref(fpname));
    else return undef;
}



