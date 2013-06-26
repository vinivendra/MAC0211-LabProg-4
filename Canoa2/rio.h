#ifndef H_RIO_DEFINE
#define H_RIO_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include "pixel.h"

/*
 Defines
*/

#define TERRA '#'
#define AGUA '.'

/*
 Protótipos
*/

void primeiraLinha(pixel* linha, int largura, float limiteDasMargens, int fluxoDesejado, int distanciaEntreIlhas, float probIlha);
void aleatorizaMargem(pixel* linhaAnterior, pixel *linha, float limiteDasMargens, int largura);
void proximaLinha (pixel* linhaAnterior, pixel *linha, int largura, float limiteDasMargens, int fluxoDesejado, int distanciaEntreIlhas, float probIlha);
int margemEsquerda (pixel *linha);
int margemDireita (pixel *linha, int largura);

#endif
