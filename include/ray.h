#ifndef RAY_H_
#define RAY_H_
#include "vector.h"
#include "stl.h"
#include <stdbool.h>

typedef struct Ray{
    V3f origin;
    V3f direction;
}Ray;

bool rayIntersectsTriangle(Ray ray, Triangle obj, V3f* outIntersectionPoint);
bool rayIntersectsObject(Ray ray, Object obj, V3f* outIntersectionPoint);
#endif