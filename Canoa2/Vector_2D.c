#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Vector_2D.h"
#include "MathPlus.h"

struct vector_2D {
    float x;
    float y;
    float h;
};

#pragma mark Declarações

float size (Vector_2D v);

#pragma mark Funções públicas

Vector_2D *v_initZero () {
    Vector_2D *v = malloc(sizeof(Vector_2D));
    v_setXY(v, 0, 0);
    return v;
}

void v_setX (Vector_2D *v, float x) {
    v->x = x;
    v->h = v_getSize(v);
}

void v_setY (Vector_2D *v, float y) {
    v->y = y;
    v->h = v_getSize(v);
}

void v_setXY (Vector_2D *v, float x, float y) {
    v->x = x;
    v->y = y;
    v->h = size(*v);
}

void v_rotate (Vector_2D *v, float angle) {
    float cosV = v_getCos(v);
    float sinV = v_getSen(v);
    
    float newCos = cosV*cosf(angle) - sinV*sinf(angle);
    float newSin = sinV*cosf(angle) + sinf(angle)*cosV;
    
    v_setXY(v, v->h*newCos, v->h*newSin);
}

void v_setSize (Vector_2D *v, float size) {
    v_setXY(v, v->h*v_getCos(v), v->h*v_getSen(v));
}




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

#pragma mark Funções privadas

float size (Vector_2D v) {
    return sqrtf((v.x*v.x)+(v.y*v.y));
}

