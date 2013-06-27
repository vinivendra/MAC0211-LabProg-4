#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "rio.h"
#include "Output.h"
#include "pixel.h"

#define YES 1
#define NO 0
typedef int BOOL;

#define TERRA '#'
#define AGUA '.'

/*
 Implementação
 */

void outputArray (pixel **array, int altura, int largura, int indice, int player_x, int player_y, int tamPixel, ALLEGRO_BITMAP *boat, float angle,int min, int seg, ALLEGRO_FONT *fonte) {
    ALLEGRO_COLOR terra = al_map_rgb(110, 60, 40);      /* Cores */
    ALLEGRO_COLOR agua = al_map_rgb(51, 153, 255);
    ALLEGRO_COLOR ilha = al_map_rgb(6, 96, 0);
    
    int i, j;
    int ilha0 = 999999, ilhaf = 0;      /* Variáveis que guardam o começo de uma ilha e o final dela */
    
    
    int playerSize = tamPixel + largura*0.1;
    if(playerSize > 30) playerSize = 30;
    
    
    al_clear_to_color(agua);
    
    for (i = 0; i < altura; i ++) { /* Imprime uma linha de cada vez */
        
        int TMargemEsquerda = margemEsquerda(array[(i+indice)%altura]);         /* Os tamanhos das margens */
        int TMargemDireita = margemDireita(array[(i+indice)%altura], largura);
        
        /* Desenha a margem esquerda */
        al_draw_filled_rectangle(0, tamPixel*i, tamPixel*(TMargemEsquerda - 2), tamPixel*(i+1), terra);
        
        j = TMargemEsquerda - 2;
        
        
        /* Desenha o encontro da terra com a água, criando triângulos ou retângulos conforme necessário */
        if (tipo(&array[(i+indice+1)%altura][j+2]) == TERRA) {          /* Se a linha de baixo era maior */
            al_draw_filled_rectangle(tamPixel*j, tamPixel*i, tamPixel*(j+1), tamPixel*(i+1), terra);
            j++;
            al_draw_filled_triangle(tamPixel*j, tamPixel*i, tamPixel*j, tamPixel*(i+1), tamPixel*(j+1), tamPixel*(i+1), terra);
        }
        else if (tipo(&array[(i+indice+1)%altura][j+1]) == TERRA) {     /* Se era igual */
            al_draw_filled_rectangle(tamPixel*j, tamPixel*i, tamPixel*(j+1), tamPixel*(i+1), terra);
            j++;
        }
        else {      /* Se era menor */
            al_draw_filled_triangle(tamPixel*j, tamPixel*i, tamPixel*(j+1), tamPixel*i, tamPixel*j, tamPixel*(i+1), terra);
            j++;
        }
        
        
        
        /* Descobre onde começa e termina a ilha */
        for (j = TMargemEsquerda; j < largura - TMargemDireita - 2; j++)
            if (tipo(&array[(i+indice)%altura][j]) == TERRA) {
                if (j < ilha0)
                    ilha0 = j;
                if (j > ilhaf)
                    ilhaf = j;
            }
        
        
        if (ilhaf != 0)         /* Se existe uma ilha, desenha primeiro as duas partes de água, depois ela em cima */
            al_draw_filled_rounded_rectangle(tamPixel*ilha0,tamPixel*i,tamPixel*(ilhaf+1),tamPixel*(i+1),3,3, ilha);
        ilha0 = 999999;         /* Reseta os valores */
        ilhaf = 0;
        
        /* Desenha o segundo encontro da água com a terra */
        if (tipo(&array[(i+indice+1)%altura][j+2]) == AGUA) {
            j++;
            al_draw_filled_triangle(tamPixel*j, tamPixel*i, tamPixel*(j+1), tamPixel*i, tamPixel*(j+1), tamPixel*(i+1), terra);
        }
        else if (tipo(&array[(i+indice+1)%altura][j+1]) == AGUA) {
            j++;
            al_draw_filled_rectangle(tamPixel*j, tamPixel*i, tamPixel*(j+1), tamPixel*(i+1), terra);
        }
        else {
            al_draw_filled_triangle(tamPixel*(j+1), tamPixel*(i+1), tamPixel*(j+1), tamPixel*i, tamPixel*j, tamPixel*(i+1), terra);
            j++;
            al_draw_filled_rectangle(tamPixel*j, tamPixel*i, tamPixel*(j+1), tamPixel*(i+1), terra);
        }
        
        
        /* Desenha a margem direita */
        al_draw_filled_rectangle(tamPixel*(largura - TMargemDireita), tamPixel*i, tamPixel*(largura), tamPixel*(i+1), terra);
        
    }
    
    /* Desenha o jogador na posição correta */
    al_draw_rotated_bitmap(boat, 12.5, 25, player_x + 12.5, player_y - 25, angle, NULL);

    /* Coloca tudo o que foi desenhado na tela */
    if (seg < 10)
        al_draw_textf(fonte, al_map_rgb(255, 0, 0), largura*tamPixel - 80 ,10, ALLEGRO_ALIGN_RIGHT, "%d:0%d", min, seg);
    else
        al_draw_textf(fonte, al_map_rgb(255, 0, 0), largura*tamPixel - 80 ,10, ALLEGRO_ALIGN_RIGHT, "%d:%d", min, seg);
    al_flip_display();
}
