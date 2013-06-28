%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "options.h"

  extern yyinput;
%}

%union{ 
int i; float f; char c; char* s;
}
%error-verbose
%token <i> VALORI
%token <f> VALORF  
%token <s> FPS LARGURA SEMENTE FLUXO PIXEL VERBOSE PROBABILIDADE DISTANCIA LIMITE

%%

arquivo : /* vazio */ | arquivo linha;

linha: '\n'| def_fps '\n'| def_largura '\n'| def_semente '\n'| def_fluxo '\n'| def_tamPixel '\n'| def_verbose '\n'| def_pIlha '\n'| def_dIlha '\n'| def_lMargem '\n';

def_fps: FPS ' ' '=' ' ' VALORI {FPSInicial = $5;} | FPS ' ' '=' ;

def_semente: SEMENTE ' ' '=' ' ' VALORI {seed = $5;} | SEMENTE ' ' '=';

def_largura: LARGURA  ' ' '=' ' ' VALORI {larguraDoRio = $5;} | LARGURA ' ' '=' ;

def_fluxo: FLUXO ' ' '=' ' ' VALORI {fluxoDesejado = $5;} | FLUXO ' ' '=';

def_tamPixel: PIXEL ' ' '=' ' ' VALORI {tamPixel = $5;} | PIXEL ' ' '=';

def_verbose: VERBOSE ' ' '=' ' ' VALORI {verbose = $5;} | VERBOSE ' ' '=';

def_pIlha: PROBABILIDADE ' ' '=' ' ' VALORF {pIlha = $5;} | PROBABILIDADE ' ' '=';

def_dIlha: DISTANCIA ' ' '=' ' ' VALORI {dIlha = $5;} | DISTANCIA ' ' '=';

def_lMargem: LIMITE ' ' '=' ' ' VALORF {limiteMargens = $5;} | LIMITE ' ' '=';

%%

yyerror (char * s) {
   fprintf(stderr,"%s\n", s);
}
