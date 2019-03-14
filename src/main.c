#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "stl.h"
#include <endian.h>
#include "ray.h"
#include <pthread.h>
#include <sys/sysinfo.h>

typedef struct Job{
    int x, y, width, height;
    int framew, frameh;
    uint8_t *frameBuf;
    Object* obj;
    Ray camRay;
}Job;

void depthShader (int px, int py, Job* job){
    // this shader is for debugging
    uint8_t (*framebuf)[job->frameh][job->framew][3] = (uint8_t (*)[job->frameh][job->framew][3])job->frameBuf;
    job->camRay.direction = (V3f){px-(job->framew/2),py-(job->frameh/2),job->camRay.direction.z};
    V3f outIntersectionPoint;
    if (rayIntersectsObject(job->camRay, *job->obj, &outIntersectionPoint)){
        (*framebuf)[py][px][0] = outIntersectionPoint.z*5-30;
        (*framebuf)[py][px][1] = outIntersectionPoint.z*5-30;
        (*framebuf)[py][px][2] = outIntersectionPoint.z*5-30;
    }
    else
        (*framebuf)[py][px][0] = 0;
}

void* render (void* jobPtr){
    Job* job = (Job*)jobPtr;
    for (int y = job->y; y<job->height; y+=1){
        for (int x = job->x; x<job->width; x+=1){
            depthShader(x, y, job);
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
        Ray camRay;
        camRay.origin = (V3f){0,0,100}; // orig: z 50cm
        camRay.direction = (V3f){0,0,0};
        job->camRay = camRay;
        
        if(pthread_create(&inc_x_thread[i], NULL, render, job)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    for (int i=0; i<8; i++){
        if(pthread_join(inc_x_thread[i], NULL)) {

            fprintf(stderr, "Error joining thread\n");
            return 2;

        }
    }

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