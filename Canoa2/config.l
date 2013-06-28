%{  #include <stdlib.h>
    #include "config.tab.h"  
%}
NUM_INT     [0-9]+
NUM_REAL    [0-9]+[[.][0-9]{1,2}]?
BRANCO      [' ']*
%%
{NUM_INT} {sscanf(yytext, "%d", &yylval); return VALORI;}
{NUM_REAL} {sscanf(yytext, "%f", &yylval); return VALORF;}
{BRANCO} {return ' ';}
"Largura" {return LARGURA;}
"FPS" {return FPS;}
"Semente" {return SEMENTE;}
"Fluxo" {return FLUXO;}
"Pixel" {return PIXEL;}
"Verbose" {return VERBOSE;}
"Limite" {return LIMITE;}
"Probabilidade" {return PROBABILIDADE;}
"Distancia" {return DISTANCIA;}
.|\n {return yytext[0];}
%%
