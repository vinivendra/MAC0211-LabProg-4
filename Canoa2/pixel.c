#include <stdlib.h>
#include <stdio.h>
#include "pixel.h"

/*
 Implementações
    Setters e getters
*/

float velocidade(pixel *p){
  return p->velocidade;
}

char tipo(pixel *p){
  return p->tipo;
}

void setaVelocidade(pixel* p, float vel){
  p->velocidade = vel;
}

void setaTipo(pixel* p, char type){
  p->tipo = type;
}

