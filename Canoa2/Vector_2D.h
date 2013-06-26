#ifndef Allegro_Vector_2D_h
#define Allegro_Vector_2D_h

typedef struct vector_2D Vector_2D;

#pragma mark Initializers

Vector_2D *v_initZero ();

#pragma mark Setters

void v_setX (Vector_2D *v, float x);
void v_setY (Vector_2D *v, float y);
void v_setXY (Vector_2D *v, float x, float y);

void v_rotate (Vector_2D *v, float angle);
void v_setSize (Vector_2D *v, float size);

#pragma mark Getters

float v_getX (Vector_2D *v);
float v_getY (Vector_2D *v);
float v_getSize (Vector_2D *v);
float v_getSize2 (Vector_2D *v);

float v_absX (Vector_2D *v);
float v_absY (Vector_2D *v);
int v_signX (Vector_2D *v);
int v_signY (Vector_2D *v);

float v_getTg (Vector_2D *v);
float v_getSen (Vector_2D *v);
float v_getCos (Vector_2D *v);


#endif
