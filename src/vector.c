#include "vector.h"
#include <math.h>

V3f V3fSum(V3f v1, V3f v2){
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
