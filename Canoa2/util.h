#ifndef H_UTIL_DEFINE
#define H_UTIL_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Protótipos
*/

int getArgs(int argc,char *argv[],float *velocidadeDoBarco, int *larguraDoRio, int *seed, int *fluxoDesejado, int *verbose, int *dIlha, float *pIlha, float *limiteMargens, int *tamPixel);
void corrigeArgs ();
#endif
