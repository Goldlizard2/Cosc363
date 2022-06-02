//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//  Road.cpp
//  See Lab04.pdf for details
//  ========================================================================

#include <iostream>
#include <cmath>
#include <GL/freeglut.h>
#include "loadBMP.h"
using namespace std;

const int NPTS = 13;    //Number of points on the polygonal line
float angle = 0;        //View rotation (arrow keys)
GLuint txId;            //Texture ID

//---- Function to load a texture in bmp format ------------------------
void loadTexture()
{
    glGenTextures(1, &txId); 				// Create a Texture object
    glBindTexture(GL_TEXTURE_2D, txId);		//Use this texture
    loadBMP("Road.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}


//---- Ground Plane --------------------------------------------------------
//The floor plane is constructed using a single quad.
void floor()
{
    glDisable(GL_TEXTURE_2D);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glNormal3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
       glVertex3f(-6, 0, -6);
       glVertex3f(6, 0, -6);
       glVertex3f(6, 0, 6);
       glVertex3f(-6, 0, 6);
	glEnd();
}


//---- Initialization function --------------------------------------------
void initialize(void) 
{
 //   loadTexture();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor (1, 1, 1, 1);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 1.0, 100.0);   //Camera frustum
}

//---- Special key event callback ---------------------------------------
// Used for rotating the scene with arrow keys
void special(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT) angle--;
    else if (key == GLUT_KEY_RIGHT) angle++;
    glutPostRedisplay();
}

//---- The main display function----------------------------------------------
//Display event callback
void display(void)
{
    //Vertices of a polygonal line
    float x[] = {-6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6};
    float z[] = { -4, -4, -3.9, -3.7, -3.25, -2.5, -1.7, -1.25, -1.1, -1, -0.9, -0.7, -0.3 };

   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};  //light0 position (directly above the origin)
 
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt (0, 15, 0, 0, 0, 0, 0, 0, -1);         //Top-down view
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);      //light's position

   glRotatef(angle, 0, 1, 0);                       //Scene rotation

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   floor();

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glColor3f(0, 0, 1);

   glBegin(GL_LINE_STRIP);                  //Draw a polygonal line using GL_LINE_STRIP
   for (int i = 0; i < NPTS; i++)
   {
       glVertex3f(x[i], 0.01, z[i]);
   }
   glEnd();

   glFlush();
}

//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Road Model");
   initialize ();

   glutDisplayFunc(display);
   glutSpecialFunc(special);

   glutMainLoop();
   return 0;
}
