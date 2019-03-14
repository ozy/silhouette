#ifndef VECTOR_H_
#define VECTOR_H_
#include "matrix.h"

typedef struct V3f
{
    float x, y, z;
}V3f;

V3f V3fAdd(V3f v1, V3f v2);
V3f V3fSub(V3f v1, V3f v2);
V3f V3fDiv(V3f v1, V3f v2);
V3f V3fMul(V3f v1, V3f v2);

V3f V3fCrossProd(V3f v1, V3f v2);
float V3fDotProd(V3f v1, V3f v2);

float V3fLen(V3f v);

V3f V3fNormalize(V3f v);

Matrix44f lookAt(const V3f from, const V3f to);
#endif