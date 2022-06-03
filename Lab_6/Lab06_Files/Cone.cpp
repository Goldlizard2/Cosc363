/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
/**
 * Cylinder's intersection method.  The input is a ray.
 */
float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
    glm::vec3 vdif = p0 - center;
    float a = (dir.x * dir.x) + (dir.z * dir.z);
    float b = 2 * (dir.x * vdif.x + dir.z * vdif.z);
    float c = vdif.x * vdif.x + vdif.z * vdif.z - (radius * radius);
    float delta = b * b - 4 * (a * c);

    if ((delta) < 0.0)
        return -1.0; // includes zero and negative values
    if ((fabs(delta)) < 0.001)
        return -1.0;

    float t1 = (-b - sqrt(delta)) / (2 * a);
    float t2 = (-b + sqrt(delta)) / (2 * a);
    float t;

    if (t1 > t2)
        t = t2;
    else
        t = t1;

    float r = p0.y + (t * dir.y);

    if ((r >= center.y) && (r <= center.y + height))
    {
        return t;
    }
    else
        return -1;
}

/**
 * Returns the unit normal vector at a given point.
 * Assumption: The input point p lies on the cylinder.
 */
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 dif = p - center;
    glm::vec3 n = glm::vec3(dif.x, 0 , dif.z);
    n = glm::normalize(n);
    return n;
}
