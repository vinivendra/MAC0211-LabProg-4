#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Vector_2D.h"

struct vector_2D {
    float x;
    float y;
    float h;
};

/* Protótipos */

float size (Vector_2D v);
int sign (int x);



/*
 Funções públicas
*/

/* Initializers */

Vector_2D *v_initZero () {
    Vector_2D *v = malloc(sizeof(Vector_2D));
    v->x = 0;
    v->y = 0;
    v->h = 0;
    return v;
}



/* Setters */

void v_setX (Vector_2D *v, float x) {
    v->x = x;
    v->h = size(*v);
}

void v_setY (Vector_2D *v, float y) {
    v->y = y;
    v->h = size(*v);
}

void v_setXY (Vector_2D *v, float x, float y) {
    v->x = x;
    v->y = y;
    v->h = size(*v);
}

void v_rotateA (Vector_2D *v, float angle) {
    float cosV = v_getCos(v);
    float sinV = v_getSen(v);
    float sinA = sinf(angle);
    float cosA = cosf(angle);
    
    float newCos = cosV*cosA - sinV*sinA;
    float newSin = sinV*cosA + sinA*cosV;
    
    v->x = v->h*newCos;
    v->y = v->h*newSin;
}

void v_rotate_SC (Vector_2D *v, float sen, float cos) {
    float cosV = v_getCos(v);
    float sinV = v_getSen(v);
    
    float newCos = cosV*cos - sinV*sen;
    float newSin = sinV*cos + sen*cosV;
    
    v->x = v->h*newCos;
    v->y = v->h*newSin;
}

void v_rotate_SC_up (Vector_2D *v, float sen, float cos) {
    float cosV = v_getCos(v);
    float sinV = v_getSen(v);
    
    float newCos = cosV*cos - sinV*sen;
    float newSin = sinV*cos + sen*cosV;
    
    if (newSin > 0) {
        v->x = v->h*newCos;
        v->y = v->h*newSin;
    }
    else {
        if (sen > 0)
            v->x = -v->h;
        else
            v->x = v->h;
        v->y = 0;
    }
}

void v_setSize (Vector_2D *v, float size) {
    float cosV = v_getCos(v);
    float sinV = v_getSen(v);
    
    v->x = size*cosV;
    v->y = size*sinV;
    v->h = size;
}



/* Getters */

float v_getX (Vector_2D *v) {
    return v->x;
}

float v_getY (Vector_2D *v) {
    return v->y;
}

float v_getSize (Vector_2D *v) {
    return v->h;
}

float v_getSize2 (Vector_2D *v) {
    return v->h*v->h;
}

float v_absX (Vector_2D *v) {
    if (v->x < 0)
        return -v->x;
    return v->x;
}

float v_absY (Vector_2D *v) {
    if (v->y < 0)
        return -v->y;
    return v->y;
}

int v_signX (Vector_2D *v) {
    if (v->x > 0) return 1;
    else if (v->x < 0) return -1;
    return 0;
}

int v_signY (Vector_2D *v) {
    if (v->y > 0) return 1;
    else if (v->y < 0) return -1;
    return 0;
}

float v_getTg (Vector_2D *v) {
    return v->y/v->x;
}

float v_getSen (Vector_2D *v) {
    return v->y/v->h;
}

float v_getCos (Vector_2D *v) {
    return v->x/v->h;
}



/* Funções privadas */

float size (Vector_2D v) {
    return sqrtf((v.x*v.x)+(v.y*v.y));
}
