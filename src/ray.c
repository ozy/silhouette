#include "ray.h"
#include "stl.h"
#include <stdbool.h>
#include <math.h>

bool rayIntersectsTriangle(Ray ray, Triangle triangle, Hit* hit){
    const float EPSILON = 0.0000001;

    V3f vertex0 = triangle.points[0];
    V3f vertex1 = triangle.points[1];  
    V3f vertex2 = triangle.points[2];
    V3f edge1, edge2, h, s, q;
    float a,f,u,v;
    
    edge1 = V3fSub(vertex1, vertex0);
    edge2 = V3fSub(vertex2, vertex0);

    h = V3fCrossProd(ray.direction,edge2);
    a = V3fDotProd(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.

    f = 1.0/a;
    s = V3fSub(ray.origin, vertex0);
    u = f * (V3fDotProd(s,h));
    if (u < 0.0 || u > 1.0)
        return false;
    q = V3fCrossProd(s,edge1);
    v = f * V3fDotProd(ray.direction,q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * V3fDotProd(edge2,q);
    if (t > EPSILON) // ray intersection
    {
        hit->hitIntersectionPoint = V3fAdd(ray.origin, V3fMul(ray.direction, (V3f){t,t,t}));
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;

}

bool rayIntersectsObject(Ray ray, Object obj, Hit* hit){
    V3f NearestOutIntersectionPoint = {+INFINITY, +INFINITY, +INFINITY};

    for (int TriangleId = 0; TriangleId < obj.STL->numTriangles; TriangleId++){
        Triangle tr = obj.STL->triangle[TriangleId];
        if (rayIntersectsTriangle(ray, tr, hit)){
            if (V3fLen(V3fSub(hit->hitIntersectionPoint,ray.origin)) < V3fLen(V3fSub(NearestOutIntersectionPoint, ray.origin))){
                NearestOutIntersectionPoint = hit->hitIntersectionPoint;
                hit->hitNormal = tr.normal;
            }
        }
    }
    if (NearestOutIntersectionPoint.x != +INFINITY){
        hit->hitIntersectionPoint = NearestOutIntersectionPoint;
        return true;
    }
    return false;
}