%{
  #include <stdio.h>
  #include <stdlib.h>

  int fps, largura, semente, fluxo, tamPixel, verbose, dIlha;
  float pIlha, lMargem;
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

def_fps: FPS ' ' '=' ' ' VALORI {fps = $5;};

def_semente: SEMENTE ' ' '=' ' ' VALORI {semente = $5;};

def_largura: LARGURA  ' ' '=' ' ' VALORI {largura = $5;};

def_fluxo: FLUXO ' ' '=' ' ' VALORI {fluxo = $5;};

def_tamPixel: PIXEL ' ' '=' ' ' VALORI {tamPixel = $5;};

def_verbose: VERBOSE ' ' '=' ' ' VALORI {verbose = $5;};

def_pIlha: PROBABILIDADE ' ' '=' ' ' VALORF {pIlha = $5;};

def_dIlha: DISTANCIA ' ' '=' ' ' VALORI {dIlha = $5;};

def_lMargem: LIMITE ' ' '=' ' ' VALORF {lMargem = $5;};

%%

int main(int argc, char** argv){
   yyparse();
   
    printf ("\t \t Opcoes disponiveis: \n"
                "-b = %d  - FPS inicial\n"
                "-l = %d  - Largura do Rio\n"
                "-s = %d  - Semente para o gerador aleatorio\n"
                "-f = %d  - Fluxo da agua\n"
                "-v = %d  - Verbose\n"
                "-pI = %f - Probabilidade de haver obstaculos\n"
                "-dI = %d - Distancia minima entre obstaculos\n"
                "-lM = %f - Limite de tamanho das margens (de 0 a 1)\n"
                "-D = %d - Tamanho de cada pixel\n"
                "\n", fps,largura , semente, fluxo, verbose, pIlha, dIlha, lMargem, tamPixel);

}

yyerror (char * s) {
   fprintf(stderr,"%s\n", s);
}
