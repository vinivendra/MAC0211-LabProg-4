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

def_fps: FPS ' ' '=' ' ' VALORI {FPSInicial = $5;};

def_semente: SEMENTE ' ' '=' ' ' VALORI {seed = $5;};

def_largura: LARGURA  ' ' '=' ' ' VALORI {larguraDoRio = $5;};

def_fluxo: FLUXO ' ' '=' ' ' VALORI {fluxoDesejado = $5;};

def_tamPixel: PIXEL ' ' '=' ' ' VALORI {tamPixel = $5;};

def_verbose: VERBOSE ' ' '=' ' ' VALORI {verbose = $5;};

def_pIlha: PROBABILIDADE ' ' '=' ' ' VALORF {pIlha = $5;};

def_dIlha: DISTANCIA ' ' '=' ' ' VALORI {dIlha = $5;};

def_lMargem: LIMITE ' ' '=' ' ' VALORF {limiteMargens = $5;};

%%

yyerror (char * s) {
   fprintf(stderr,"%s\n", s);
}
