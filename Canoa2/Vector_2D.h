#ifndef Allegro_Vector_2D_h
#define Allegro_Vector_2D_h

typedef struct vector_2D Vector_2D;

/*
 Esta implementação foi pensada de forma a evitar ao máximo chamadas a funções 'pesadas'.
 A 'sqrt' será chamada apenas nas funções v_setX, v_setY e v_setXY, uma vez em cada.
 Dessa forma, é mais eficiente chamar uma vez v_setXY do que chamar v_setX e v_setY separadamente.
 O método rotate também é um pouco mais pesado, pois vai chamar uma vez a
 'cosf' e uma vez a 'sinf'.
 Os outros métodos (sejam initializers, setters ou getters) são muito mais eficientes.
 
*/


/* Initializers */

Vector_2D *v_initZero ();                           /* Malloca e retorna um vetor v de tamanho 0 */

/* Setters */

void v_setX (Vector_2D *v, float x);                /* Insere um novo valor para a coordenada x do vetor; o y se mantém, e o tamanho é recalculado */
void v_setY (Vector_2D *v, float y);                /* Idem, para Y */
void v_setXY (Vector_2D *v, float x, float y);      /* Insere novos valores de x e y; mais eficiente que a inserção separada */

void v_rotate (Vector_2D *v, float angle);          /* Roda o vetor 'angle' radianos para a esquerda */
void v_setSize (Vector_2D *v, float size);          /* Muda o tamanho do vetor, mas mantém seu ângulo */

/* Getters */

float v_getX (Vector_2D *v);                        /* Retorna a coordenada x do vetor */
float v_getY (Vector_2D *v);                        /* Retorna a coordenada y do vetor */
float v_getSize (Vector_2D *v);                     /* Retorna o tamanho do vetor */
float v_getSize2 (Vector_2D *v);                    /* Retorna o tamanho do vetor ao quadrado */

float v_absX (Vector_2D *v);                        /* Retorna o módulo da coordenada x */
float v_absY (Vector_2D *v);                        /* Retorna o módulo da coordenada y */
int v_signX (Vector_2D *v);                         /* Retorna o sinal (sentido) da coordenada x */
int v_signY (Vector_2D *v);                         /* Retorna o sinal (sentido) da coordenada y */

float v_getTg (Vector_2D *v);                       /* Retorna a tangente */
float v_getSen (Vector_2D *v);                      /* Retorna o seno */
float v_getCos (Vector_2D *v);                      /* Retorna o cosseno */


#endif
