#ifndef STL_H_
#define STL_H_
#include <inttypes.h>
#include "vector.h"

typedef struct Triangle{
    V3f normal;
    V3f points[3];
}Triangle;

typedef struct STLFile{
    char header[80];
    uint32_t numTriangles;
    Triangle* triangle;
}STLFile;

typedef struct Object{
    V3f Pos; // for future
    STLFile* STL;
    V3f Color;
}Object;

STLFile loadSTL(const char* filename);

#endif