#include "stl.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


STLFile loadSTL(const char* filename){
    STLFile stl;
    FILE* fd;

    if ((fd = fopen(filename,"r")) == NULL){
        printf("Error while opening the file!\n");
    }

    fread(stl.header, sizeof(stl.header), 1, fd);
    fread(&stl.numTriangles, sizeof(stl.numTriangles), 1, fd);
    stl.numTriangles = le32toh(stl.numTriangles);

    assert(stl.numTriangles<50000); // possibly ascii stl.

    stl.triangle = malloc(sizeof(Triangle) * stl.numTriangles);
    for (int triangleId = 0; triangleId < stl.numTriangles; triangleId++){
        fread(&stl.triangle[triangleId].normal, sizeof(V3f), 1, fd);
        fread(&stl.triangle[triangleId].points, sizeof(V3f), 3, fd);
        fread(&stl.triangle[triangleId].attrByteCount, sizeof(uint16_t), 1, fd);
        //printf ("%d: Normal: %f, %f, %f Points: %f, %f, %f\n", triangleId, stl.triangle[triangleId].normal.x,stl.triangle[triangleId].normal.y,stl.triangle[triangleId].normal.z, stl.triangle[triangleId].points[0].x,stl.triangle[triangleId].points[0].y,stl.triangle[triangleId].points[0].z);
    }
    return stl;
}