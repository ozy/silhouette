#ifndef RAY_H_
#define RAY_H_
#include "vector.h"
#include "stl.h"
#include <stdbool.h>

typedef struct Ray{
    V3f origin;
    V3f direction;
}Ray;

typedef struct Hit{
    V3f hitIntersectionPoint;
    V3f hitNormal;
}Hit;


bool rayIntersectsTriangle(Ray ray, Triangle obj, Hit* hit);
bool rayIntersectsObject(Ray ray, Object obj, Hit* hit);
#endif