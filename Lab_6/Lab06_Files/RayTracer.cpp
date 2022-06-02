/*==================================================================================
 * COSC 363  Computer Graphics (2022)
 * Department of Computer Science and Software Engineering, University of Canterbury.
 *
 * A basic ray tracer
 * See Lab06.pdf   for details.
 *===================================================================================
 */
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
#include "Plane.h"
#include "TextureBMP.h"
#include "Cylinder.h"

using namespace std;
TextureBMP texture;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;
const float TransparentIndex = 0;
const float FloorIndex = 3;
const float ETA = 1.003;

vector<SceneObject *> sceneObjects;

//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);		 // Background colour = (0,0,0)
	glm::vec3 lightPos(-10, 15, -5); // Light's position
	glm::vec3 lightPos2(16, 16, 0);
	glm::vec3 color(0);
	int16_t lightDist;
	SceneObject *obj;

	float transparency = 0.2;

	ray.closestPt(sceneObjects); // Compare the ray with all objects in the scene
	if (ray.index == -1)
		return backgroundCol;	   // no intersection
	obj = sceneObjects[ray.index]; // object on which the closest point of intersection is found
	glm::vec3 normalVec = obj->normal(ray.hit);

	if (ray.index == FloorIndex)
	{ // draw checkered plane
		int squareSize = 5;
		int sizeZ = (ray.hit.z) / squareSize;
		int sizeX = (ray.hit.x) / squareSize;
		int k = (sizeZ + sizeX) % 2;
		glm::vec3 floorColor(0);
		switch (k)
		{
		case 0:
			floorColor = glm::vec3(0, 1, 0);
			break;
		default:
			floorColor = glm::vec3(1, 1, 1);
			break;
		}
		obj->setColor(floorColor);
	}
	// refraction for transparent sphere

	color = obj->lighting(lightPos, -ray.dir, ray.hit); // Object's colour
	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);
	shadowRay.closestPt(sceneObjects);
	lightDist = glm::length(lightVec);

	if (shadowRay.index > -1 && shadowRay.dist < lightDist)
		color = 0.2f * obj->getColor();

	if (obj->isReflective() && step < MAX_STEPS)
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	if (ray.index == TransparentIndex && step < MAX_STEPS)
	{
		Ray d(ray.hit, ray.dir);
		d.closestPt(sceneObjects);
		Ray r(d.hit, d.dir);
		color += trace(r, step + 1) * obj->getTransparencyCoeff();
	}

	return color;
}

void fog()
{
	glm::vec4 fogColor(0.7, 0.8, 0.9, 1.0); // bluish grey
	float fogStart = 0.2;
	float fogEnd = 0.8;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;						  // grid point
	float cellX = (XMAX - XMIN) / NUMDIV; // cell width
	float cellY = (YMAX - YMIN) / NUMDIV; // cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS); // Each cell is a tiny quad.

	for (int i = 0; i < NUMDIV; i++) // Scan every cell of the image plane
	{
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j * cellY;

			glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST); // direction of the primary ray

			Ray ray = Ray(eye, dir);

			glm::vec3 col = trace(ray, 1); // Trace the primary ray and get the colour value
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp); // Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);
		}
	}

	glEnd();
	glFlush();
}

//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 1);

	// Sphere Cylinder Cone
	Sphere *sphere1 = new Sphere(glm::vec3(-5, -5, -90), 5.0);
	sphere1->setColor(glm::vec3(0, 0, 0)); // Set colour to blue
	sphere1->setSpecularity(false);
	sphere1->setReflectivity(true, 0.8);
	sceneObjects.push_back(sphere1); // Add sphere to scene objects

	Sphere *sphere2 = new Sphere(glm::vec3(10, -13, -90), 2.0);
	sphere2->setColor(glm::vec3(0, 0, 1)); // Set colour to blue
	sceneObjects.push_back(sphere2);	   // Add sphere to scene objects

	Cylinder *cylinder1 = new Cylinder(glm::vec3(0, -10, -50), 2, 5);
	cylinder1->setColor(glm::vec3(0, 1, 0));
	cylinder1->setSpecularity(false);
	sceneObjects.push_back(cylinder1); // Add sphere to scene

	// Floor and walls
	Plane *plane = new Plane(glm::vec3(-20., -15, -40),
							 glm::vec3(20., -15, -40),
							 glm::vec3(20., -15, -200),
							 glm::vec3(-20., -15, -200));
	plane->setColor(glm::vec3(0.8, 0.8, 0));
	sceneObjects.push_back(plane);
	plane->setSpecularity(false);

	//	Box
	glm::vec3 vertex_a(5, -12, -70);
	glm::vec3 vertex_b(8, -12, -70);
	glm::vec3 vertex_c(8, -12, -80);
	glm::vec3 vertex_d(5, -12, -80);
	glm::vec3 vertex_e(5, -15, -70);
	glm::vec3 vertex_f(8, -15, -70);
	glm::vec3 vertex_g(8, -15, -80);
	glm::vec3 vertex_h(5, -15, -80);
	Plane *front = new Plane(vertex_a, vertex_e, vertex_f, vertex_b);
	front->setColor(glm::vec3(1, 0, 0));
	sceneObjects.push_back(front);
	Plane *back = new Plane(vertex_d, vertex_c, vertex_g, vertex_h);
	back->setColor(glm::vec3(1, 0, 0));
	sceneObjects.push_back(back);
	Plane *top = new Plane(vertex_a, vertex_b, vertex_c, vertex_d);
	top->setColor(glm::vec3(1, 0, 0));
	sceneObjects.push_back(top);
	Plane *bottom = new Plane(vertex_e, vertex_f, vertex_g, vertex_h);
	bottom->setColor(glm::vec3(1, 0, 0));
	sceneObjects.push_back(bottom);
	Plane *left = new Plane(vertex_h, vertex_e, vertex_a, vertex_d);
	left->setColor(glm::vec3(1, 0, 0));
	sceneObjects.push_back(left);
	Plane *right = new Plane(vertex_f, vertex_g, vertex_c, vertex_b);
	right->setColor(glm::vec3(1, 0, 0));
	sceneObjects.push_back(right);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Raytracing");

	glutDisplayFunc(display);
	initialize();

	glutMainLoop();
	return 0;
}
