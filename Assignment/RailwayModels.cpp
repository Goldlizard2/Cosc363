#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>
#include "RailModels.h"
#include "loadBitmap.h"

GLuint txId[2];
void loadTexture(int i, char *filename)
{
	glGenTextures(1, &txId[i]);			   // Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txId[i]); // Use this texture
	loadBMP(filename);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void floor()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glNormal3f(0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glBegin(GL_QUADS);
	for (int i = -150; i < 150; i++)
	{
		for (int j = -150; j < 150; j++)
		{
			float pt1 = (((float)i + (float)150) / (float)300);
			float pt2 = (((float)j + (float)150) / (float)300);
			glTexCoord2f(pt1, pt2);
			glVertex3f(i, 0, j);
			glTexCoord2f(pt1 + 0., pt2);
			glVertex3f(i, 0, j + 1);
			glTexCoord2f(pt1 + 0.0033, pt2 + 0.0033);
			glVertex3f(i + 1, 0, j + 1);
			glTexCoord2f(pt1, pt2 + 0.0033);
			glVertex3f(i + 1, 0, j);
		}
		
	}
	glEnd();
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glDisable(GL_TEXTURE_2D);
}

void tunnel()
{
	const int N = 9;
	float vx_init[N] = {30, 4, 4, 17, 18, 19, 20, 24, 30};
	float vy_init[N] = {30, 30, 24, 20, 17, 15, 8, 0, 0};
	float zy_init[N] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i;
	int j;

	for (j = 0; j < 2; j++)
	{
		glBegin(GL_TRIANGLE_FAN);

		glVertex3f(vx_init[0], vy_init[0], j * 60);

		for (i = 0; i < N; i++)
		{
			glVertex3f(vx_init[i], vy_init[i], j * 60);
		}
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 8; i++)
	{
		glTexCoord2f(0.25, 0.25);
		glVertex3f(vx_init[i], vy_init[i], zy_init[i]);
		glTexCoord2f(0.25, 0.5);
		glVertex3f(vx_init[i + 1], vy_init[i + 1], zy_init[i]);
		glTexCoord2f(0.7, 0.5);
		glVertex3f(vx_init[i], vy_init[i], 60);
		glTexCoord2f(0.7, 0.25);
		glVertex3f(vx_init[i + 1], vy_init[i + 1], 60);
	}
	glEnd();
	glBegin(GL_QUADS);
	glTexCoord2f(0.25, 0.0);
	glVertex3f(vx_init[0], vy_init[0], zy_init[0]);
	glTexCoord2f(0.7, 0);
	glVertex3f(vx_init[0], vy_init[0], 60);
	glTexCoord2f(0.7, 0.25);
	glVertex3f(vx_init[8], vy_init[8], 60);
	glTexCoord2f(0.25, 0.25);
	glVertex3f(vx_init[8], vy_init[8], 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void boomarm()
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glBegin(GL_QUADS);
	// top
	glTexCoord2f(0.72, 1);
	glVertex3f(0, 10, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0, 10, 3);
	glTexCoord2f(1, 0);
	glVertex3f(30, 10, 3);
	glTexCoord2f(0.72, 0);
	glVertex3f(30, 10, 0);
	// front
	glTexCoord2f(0.72, 1);
	glVertex3f(30, 8, 0);
	glTexCoord2f(1, 1);
	glVertex3f(30, 10, 0);
	glTexCoord2f(1, 0);
	glVertex3f(30, 10, 3);
	glTexCoord2f(0.72, 0);
	glVertex3f(30, 8, 3);
	// back

	glTexCoord2f(0.72, 1);
	glVertex3f(0, 8, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0, 10, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 10, 3);
	glTexCoord2f(0.72, 0);
	glVertex3f(0, 8, 3);

	// bottom
	glTexCoord2f(0.72, 1);
	glVertex3f(0, 8, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0, 8, 3);
	glTexCoord2f(1, 0);
	glVertex3f(30, 8, 3);
	glTexCoord2f(0.72, 0);
	glVertex3f(30, 8, 0);
	// back side
	glTexCoord2f(0.72, 1);
	glVertex3f(30, 10, 0);
	glTexCoord2f(0.72, 0);
	glVertex3f(0, 10, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, 8, 0);
	glTexCoord2f(1, 1);
	glVertex3f(30, 8, 0);
	// front side
	glTexCoord2f(0.72, 1);
	glVertex3f(30, 10, 3);
	glTexCoord2f(0.72, 0);
	glVertex3f(0, 10, 3);
	glTexCoord2f(1, 0);
	glVertex3f(0, 8, 3);
	glTexCoord2f(1, 1);
	glVertex3f(30, 8, 3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void boomBase()
{
	glColor4f(0.5, 0.6, 0.1, 1.0);
	glBegin(GL_QUADS);

	glVertex3f(5, 0, 0);
	glVertex3f(5, 0, 5);
	glVertex3f(5, 10, 5);
	glVertex3f(5, 10, 0);

	glVertex3f(5, 0, 5);
	glVertex3f(5, 0, 8);
	glVertex3f(5, 8, 8);
	glVertex3f(5, 8, 5);

	glVertex3f(5, 0, 8);
	glVertex3f(5, 0, 13);
	glVertex3f(5, 10, 13);
	glVertex3f(5, 10, 8);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 5);
	glVertex3f(0, 10, 5);
	glVertex3f(0, 10, 0);

	glVertex3f(0, 0, 5);
	glVertex3f(0, 0, 8);
	glVertex3f(0, 5, 8);
	glVertex3f(0, 5, 5);

	glVertex3f(0, 0, 8);
	glVertex3f(0, 0, 13);
	glVertex3f(0, 10, 13);
	glVertex3f(0, 10, 8);
	// side right
	glVertex3f(10, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);
	glVertex3f(10, 10, 0);
	// side left
	glVertex3f(10, 0, 13);
	glVertex3f(0, 0, 13);
	glVertex3f(0, 10, 13);
	glVertex3f(10, 10, 13);
	// top right
	glVertex3f(10, 10, 5);
	glVertex3f(0, 10, 5);
	glVertex3f(0, 10, 0);
	glVertex3f(10, 10, 0);
	// top left
	glVertex3f(10, 10, 8);
	glVertex3f(0, 10, 8);
	glVertex3f(0, 10, 13);
	glVertex3f(10, 10, 13);
	// floor
	glVertex3f(0, 0.1, 0);
	glVertex3f(0, 0.1, 13);
	glVertex3f(10, 0.1, 13);
	glVertex3f(10, 0.1, 0);

	glEnd();
	glColor4f(0, 0, 0, 1);
}
void station()
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);

	glBegin(GL_QUADS);

	// back side

	glTexCoord2f(0.25, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(80, 0, 0);
	glTexCoord2f(0, 0.5);
	glVertex3f(80, 20, 0);
	glTexCoord2f(0.25, 0.5);
	glVertex3f(0, 20, 0);
	// bottom
	glTexCoord2f(0.25, 0);
	glVertex3f(0, 5, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0, 5, 20);
	glTexCoord2f(0, 0.5);
	glVertex3f(80, 5, 20);
	glTexCoord2f(0.25, 0.5);
	glVertex3f(80, 5, 0);
	// front side
	glTexCoord2f(0.0, 0);
	glVertex3f(0, 0, 20);
	glTexCoord2f(0.0, 0.5);
	glVertex3f(80, 0, 20);
	glTexCoord2f(0.25, 0.5);
	glVertex3f(80, 5, 20);
	glTexCoord2f(0.25, 0.0);
	glVertex3f(0, 5, 20);

	glColor4f(0, 0, 0, 0);
	glVertex3f(0, 20, 0);
	glVertex3f(0, 20, 20);
	glVertex3f(80, 20, 20);
	glVertex3f(80, 20, 0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void base()
{
	glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 4.0, 0.0);
	glScalef(30.0, 2.0, 12.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-15, 4.0, 0);
	glScalef(-10.0, 2, -2.0);
	glutSolidCube(1.0);
	glPopMatrix();

	float wx[4] = {-8, 8, -8, 8};
	float wz[4] = {5.1, 5.1, -5.1, -5.1};
	glColor4f(0.5, 0., 0., 1.0);
	GLUquadric *q = gluNewQuadric();

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(wx[i], 2.0, wz[i]);
		gluDisk(q, 0.0, 2.0, 20, 2);
		glPopMatrix();
	}
}

void engine()
{
	base();

	glColor4f(0.8, 0.8, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(7.0, 8.5, 0.0);
	glScalef(6.0, 7.0, 10.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.0, 14.0, 0.0);
	glScalef(4.0, 4.0, 8.0);
	glutSolidCube(1.0);
	glPopMatrix();

	// Boiler
	GLUquadric *q = gluNewQuadric(); // Cylinder
	glPushMatrix();
	glColor4f(0.5, 0., 0., 1.0);
	glTranslatef(4.0, 10.0, 0.0);
	glRotatef(-90.0, 0., 1., 0.);
	gluCylinder(q, 5.0, 5.0, 14.0, 20, 5);
	glTranslatef(0.0, 0.0, 14.0);
	gluDisk(q, 0.0, 5.0, 20, 3);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	glTranslatef(0.0, 4.0, 0.2);
	gluDisk(q, 0.0, 1.0, 20, 2); // headlight!
	glPopMatrix();
}

void wagon()
{
	base();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0); // Facing +z (Front side)
	glTexCoord2f(0.25, 0.5);
	glVertex3f(-15.0, 5.0, 6.0);
	glTexCoord2f(0.25, 1);
	glVertex3f(15.0, 5.0, 6.0);
	glTexCoord2f(0.36, 1);
	glVertex3f(15.0, 17.0, 6.0);
	glTexCoord2f(0.36, 0.5);
	glVertex3f(-15.0, 17.0, 6.0);

	glNormal3f(0.0, 0.0, -1.0); // Facing -z (Back side)
	glTexCoord2f(0.72, 1);
	glVertex3f(15.0, 5.0, -6.0);
	glTexCoord2f(0.72, 0.5);
	glVertex3f(-15.0, 5.0, -6.0);
	glTexCoord2f(0.59, 0.5);
	glVertex3f(-15.0, 17.0, -6.0);
	glTexCoord2f(0.59, 1);
	glVertex3f(15.0, 17.0, -6.0);

	glNormal3f(0.0, 1.0, 0.0); // Facing +y (Top side)
	glTexCoord2f(0.36, 0.5);
	glVertex3f(-15.0, 17.0, 6.0);
	glTexCoord2f(0.36, 1);
	glVertex3f(15.0, 17.0, 6.0);
	glTexCoord2f(0.59, 1);
	glVertex3f(15.0, 17.0, -6.0);
	glTexCoord2f(0.59, 0.5);
	glVertex3f(-15.0, 17.0, -6.0);
	glEnd();

	// 2 small side polygons (left, right)
	glColor4f(0.5, 0.5, 0.0, 1.0);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0); // Facing -x (Left side)
	glVertex3f(-15.0, 5.0, -6.0);
	glVertex3f(-15.0, 5.0, 6.0);
	glVertex3f(-15.0, 17.0, 6.0);
	glVertex3f(-15.0, 17.0, -6.0);

	glNormal3f(1.0, 0.0, 0.0); // Facing +x (Right side)
	glVertex3f(15.0, 5.0, 6.0);
	glVertex3f(15.0, 5.0, -6.0);
	glVertex3f(15.0, 17.0, -6.0);
	glVertex3f(15.0, 17.0, 6.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}