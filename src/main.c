#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "stl.h"
#include "ray.h"
#include <pthread.h>
#include <sys/sysinfo.h>
#include "matrix.h"
#include <math.h>

typedef struct Job{
    int x, y, width, height;
    int framew, frameh;
    uint8_t *frameBuf;
    Object* obj;
    Matrix44f camMatrix;
}Job;

void depthShader (int px, int py, V3f rayDir, Job* job){
    Ray camRay;
    camRay.origin = (V3f){job->camMatrix.matrix[3][0], job->camMatrix.matrix[3][1], job->camMatrix.matrix[3][2]};
    camRay.direction = rayDir;
    // this shader is for debugging
    uint8_t (*framebuf)[job->frameh][job->framew][3] = (uint8_t (*)[job->frameh][job->framew][3])job->frameBuf;
    Hit hit;
    if (rayIntersectsObject(camRay, *job->obj, &hit)){
        float shade = V3fLen(V3fSub(hit.hitIntersectionPoint,camRay.origin));
        (*framebuf)[py][px][0] = shade*5;
        (*framebuf)[py][px][1] = shade*5;
        (*framebuf)[py][px][2] = shade*5;
    }
    else{
        (*framebuf)[py][px][0] = 0;
        (*framebuf)[py][px][1] = 0;
        (*framebuf)[py][px][2] = 0;
    }
}

void facingRatioShader (int px, int py, V3f rayDir, Job* job){ 
    // this shader relies on stl normals (not calculated by this program)
    // If you're having problems with this shader, this might be related with that.
    Ray camRay;
    camRay.origin = (V3f){job->camMatrix.matrix[3][0], job->camMatrix.matrix[3][1], job->camMatrix.matrix[3][2]};
    camRay.direction = rayDir;
    uint8_t (*framebuf)[job->frameh][job->framew][3] = (uint8_t (*)[job->frameh][job->framew][3])job->frameBuf;
    Hit hit;
    if (rayIntersectsObject(camRay, *job->obj, &hit)){
        V3f col = V3fCrossProd(hit.hitNormal, V3fMul(rayDir, (V3f){-1,-1,-1}));
        col = V3fMul ((V3f){255,255,255}, col);
        float shade = V3fLen(col);
        // Use col x,y,z for r,g,b respectively to render colorfully
        // or the vector length for all to shade grayscale
        (*framebuf)[py][px][0] = shade;
        (*framebuf)[py][px][1] = shade;
        (*framebuf)[py][px][2] = shade;
    }
    else{
        (*framebuf)[py][px][0] = 0;
        (*framebuf)[py][px][1] = 0;
        (*framebuf)[py][px][2] = 0;
    }
}

void* render (void* jobPtr){
    Job* job = (Job*)jobPtr;
    float invWidth = 1 / (float)job->framew, invHeight = 1 / (float)job->frameh;
    float fov = 90, aspectratio = job->framew / (float)job->frameh;
    float angle = tan(M_PI * 0.5 * fov / 180.0);

    for (int y = job->y; y<job->height; y+=1){
        for (int x = job->x; x<job->width; x+=1){
            V3f camForward = (V3f){job->camMatrix.matrix[1][0], job->camMatrix.matrix[1][1], job->camMatrix.matrix[1][2]}; // forward
            V3f u = (V3f){job->camMatrix.matrix[0][0], job->camMatrix.matrix[0][1], job->camMatrix.matrix[0][2]}; // right
            V3f v = (V3f){job->camMatrix.matrix[2][0], job->camMatrix.matrix[2][1], job->camMatrix.matrix[2][2]}; // up
            V3f w = V3fMul(camForward, (V3f){-1, -1, -1}); // normal vector is the reverse of the camera forward dir.

            float xx = (2 * ((x + 0.5) * invWidth) - 1)/2 * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight))/2 * angle;
            V3f xp = (V3f){xx, xx, xx}; // this is necessary because I didn't implemented the
            V3f yp = (V3f){yy, yy, yy}; // V3fmul against a scalar.

            V3f dir = V3fAdd(V3fAdd( w, V3fMul(xp,u)), V3fMul(yp,v)); // calc camera direction vector.
            dir = V3fNormalize(dir);
            facingRatioShader(x, y, dir, job);
        }
    }
    free(job);
    return NULL;
}

int main(int argc, char* argv[]){
    int numThreads = get_nprocs();
    int framew = 1024, frameh = 1024;
    pthread_t inc_x_thread[numThreads];

    STLFile stl = loadSTL(argv[1]);
    Object obj;

    Matrix44f camMatrix;
    camMatrix = lookAt((V3f){100,100,150},(V3f){0,0,0});
    //                     ^ Camera Origin,    ^ Target
    obj.Pos = (V3f){0,0,0};
    obj.STL = &stl;
    obj.Color = (V3f){255,255,255}; // white

    uint8_t (*frameBuf)[frameh][framew][3] = malloc(frameh*framew*3);
    // split the frame into numThreads horizontally.
    for (int i=0; i<numThreads; i++){
        Job* job = malloc(sizeof(Job));
        job->width = framew;
        job->frameh = frameh;
        job->framew = framew;
        job->height = (i+1) * (frameh/numThreads);
        job->frameBuf = (uint8_t*)frameBuf;
        job->obj = &obj;
        job->x = 0;
        job->y = i * (frameh/numThreads);
        job->camMatrix = camMatrix;
        if(pthread_create(&inc_x_thread[i], NULL, render, job)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    for (int i=0; i<numThreads; i++){
        if(pthread_join(inc_x_thread[i], NULL)) {

            fprintf(stderr, "Error joining thread\n");
            return 2;

        }
    }
    
    // write into file
    printf("P3\n%d %d\n255\n",framew, frameh);

    for (int y = 0; y < frameh; y++){
        for (int x = 0; x < framew; x++){
            printf("%d %d %d ",(*frameBuf)[y][x][0],(*frameBuf)[y][x][1],(*frameBuf)[y][x][2]);
        }
        printf("\n");
    }
    free (stl.triangle);
    free (frameBuf);
}
