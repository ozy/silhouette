#include "stl.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <endian.h>
// Endian.h is platform dependent and unnecessary in modern desktop
// cpus. Will be replaced with a possible macro in the future.

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
        stl.triangle[triangleId] = (Triangle){};
        fread(&stl.triangle[triangleId].normal.x, sizeof(float), 1, fd);
        stl.triangle[triangleId].normal.x = le32toh(stl.triangle[triangleId].normal.x);

        fread(&stl.triangle[triangleId].normal.y, sizeof(float), 1, fd);
        stl.triangle[triangleId].normal.y = le32toh(stl.triangle[triangleId].normal.y);

        fread(&stl.triangle[triangleId].normal.z, sizeof(float), 1, fd);
        stl.triangle[triangleId].normal.z = le32toh(stl.triangle[triangleId].normal.z);

        for(int pointId = 0; pointId < 3; pointId++){
            fread(&stl.triangle[triangleId].points[pointId].x, sizeof(float), 1, fd);
            stl.triangle[triangleId].points[pointId].x = le32toh(stl.triangle[triangleId].points[pointId].x);
    
            fread(&stl.triangle[triangleId].points[pointId].y, sizeof(float), 1, fd);
            stl.triangle[triangleId].points[pointId].y = le32toh(stl.triangle[triangleId].points[pointId].y);

            fread(&stl.triangle[triangleId].points[pointId].z, sizeof(float), 1, fd);
            stl.triangle[triangleId].points[pointId].z = le32toh(stl.triangle[triangleId].points[pointId].z);
        }

        //printf ("%d: Normal: %f, %f, %f Points: %f, %f, %f\n", triangleId, stl.triangle[triangleId].normal.x,stl.triangle[triangleId].normal.y,stl.triangle[triangleId].normal.z, stl.triangle[triangleId].points[0].x,stl.triangle[triangleId].points[0].y,stl.triangle[triangleId].points[0].z);

        // Skip attribute data.
        uint16_t attrByteCount;
        fread(&attrByteCount, sizeof(uint16_t), 1, fd);
        attrByteCount = le16toh(attrByteCount);
        fseek ( fd , attrByteCount , SEEK_CUR );
    }
    return stl;
}