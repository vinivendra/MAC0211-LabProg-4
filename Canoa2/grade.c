#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grade.h"

/*
 Implementações
 */

pixel **initGrade(int altura, int largura) {    /* Malloca espaço para a grade */
    int i = 0;
    pixel **grade;
    
    grade = malloc(altura*sizeof(pixel*));      /* Primeira dimensão */
    
    for (i = 0; i < altura; i++)                /* Linhas da segunda dimensão */
        grade[i] = malloc(largura*sizeof(pixel));
    
    return grade;
}

void criaPrimeiroFrame(pixel **grade, int altura, int largura, float limiteDasMargens, int fluxoDesejado, int distanciaEntreIlhas, float probIlha) {
  /* Popula a grade com os números do primeiro frame */
  /*A primeira linha a ser gerada e' a ultima linha da matriz, que e' entao passada com "seed" para geracao das demais */
    
    int i = 1;
    
    primeiraLinha(grade[altura-1], largura, limiteDasMargens, fluxoDesejado, distanciaEntreIlhas, probIlha);      /* Popula a primeira linha; as outras serão criadas em cima dela */
    
    for (i = altura-2; i >= 0; i--)
        proximaLinha (grade[i+1], grade[i], largura, limiteDasMargens, fluxoDesejado, distanciaEntreIlhas, probIlha);   /* Popula as próximas linhas, com base na 1a */
}


void criaProximoFrame (pixel **grade, int altura, int largura, float limiteDasMargens, int fluxoDesejado, int indice, int distanciaEntreIlhas, float probIlha) {
    proximaLinha(grade[(indice +1)%altura], grade[indice], largura, limiteDasMargens, fluxoDesejado, distanciaEntreIlhas, probIlha);    /* Cria a linha nova do frame */
}

void freeGrade(pixel** grade, int altura, int largura){
    int i;
    
    for(i = 0; i < altura; i++)
        free(grade[i]);
    
    free(grade);
}


