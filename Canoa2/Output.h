#ifndef H_OUTPUT_DEFINE
#define H_OUTPUT_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
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
void outputArray (pixel **array, int altura, int largura, int indice, int player_x, int player_y,float x2, float y2, int tamPixel, ALLEGRO_BITMAP *boat, float angle, float bla,int min, int seg, ALLEGRO_FONT *fonte);



#endif
