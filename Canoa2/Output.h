#ifndef H_OUTPUT_DEFINE
#define H_OUTPUT_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pixel.h"

/*
 Interface para colocar na tela o que o programa quiser. Escrita de forma genérica,
 para que possa ser usada com implementações de qualquer tipo (seja em ASCII na saída
 padrão, seja numa janela separada em cores.
*/

/*
 Protótipos
*/

void freeOutput ();
void outputArray (pixel **array, int altura, int largura, int indice, int player_x, int player_y, float x2, float y2, int tamPixel);



#endif
