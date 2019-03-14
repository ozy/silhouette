#include "vector.h"
#include <math.h>
#include "matrix.h"

V3f V3fAdd(V3f v1, V3f v2){
    return (V3f){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

V3f V3fSub(V3f v1, V3f v2){
    return (V3f){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

V3f V3fDiv(V3f v1, V3f v2){
    return (V3f){v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}

V3f V3fMul(V3f v1, V3f v2){
    return (V3f){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

V3f V3fCrossProd(V3f v1, V3f v2){
    return (V3f){
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

float V3fDotProd(V3f v1, V3f v2){
    V3f result = V3fMul(v1, v2);
    return result.x + result.y + result.z;
}

float V3fLen(V3f v){
    return ( sqrtf( powf(v.x,2) + powf(v.y,2) + powf(v.z,2) ) );
}

V3f V3fNormalize(V3f v){
    float len = V3fLen(v);
    return (V3f){v.x / len, v.y / len, v.z / len};
}

Matrix44f lookAt(const V3f from, const V3f to){
    const V3f tmp = (V3f){0, 0, 1};

    V3f forward = V3fNormalize(V3fSub(from, to));
    V3f right = V3fCrossProd(V3fNormalize(tmp), forward);
    V3f up = V3fCrossProd(forward, right);

    Matrix44f camToWorld;

    /*
    Rightx      Righty      Rightz      0
    Forwardx    Forwardy    Forwardz    0
    Upx         Upy         Upz         0
    Fromx       Fromy       Fromz       1(?)
    */

    camToWorld.matrix[0][0] = right.x;
    camToWorld.matrix[0][1] = right.y;
    camToWorld.matrix[0][2] = right.z;
    camToWorld.matrix[1][0] = forward.x;
    camToWorld.matrix[1][1] = forward.y;
    camToWorld.matrix[1][2] = forward.z;
    camToWorld.matrix[2][0] = up.x;
    camToWorld.matrix[2][1] = up.y;
    camToWorld.matrix[2][2] = up.z;

    camToWorld.matrix[3][0] = from.x;
    camToWorld.matrix[3][1] = from.y;
    camToWorld.matrix[3][2] = from.z;

    return camToWorld;
}