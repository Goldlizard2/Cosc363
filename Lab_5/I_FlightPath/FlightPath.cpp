//  ========================================================================
//  COSC363: Computer Graphics (2022)
//
//  FILE NAME: FlightPath.cpp
//  See Lab05.pdf for details.
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <cmath> 
#include <GL/freeglut.h>
using namespace std;

float angle = -40;		//Scene rotation angle
const int NPTS = 70;	//Number of points on the flight path
int option = 1;			//View modes:  1 = model view, 2 = room view
float ptx[NPTS], pty[NPTS], ptz[NPTS];
GLUquadric *q;


//Reads flight path data from FlightPath.txt
void loadFlightPath()
{
	ifstream ifile;
	ifile.open("FlightPath.txt");
	for (int i = 0; i < NPTS; i++)
		ifile >> ptx[i] >> pty[i] >> ptz[i];
		
	ifile.close();
}


void drawFin()
{
	glColor3f(1, 0, 0);
	glNormal3f(0, 0, 1);
	glBegin(GL_TRIANGLES);
	  glVertex3f(-1, 0.5, 0);
	  glVertex3f(2, 0.5, 0);
	  glVertex3f(-1, 4, 0);
	glEnd();
}

void drawModel()
{
	glColor3f(0, 1, 1);
	glPushMatrix();  //body
	  glRotatef(90, 0, 1, 0);
	  gluCylinder(q, 1, 1, 7, 36, 5);
	  gluDisk(q, 0, 1, 36, 3);
	glPopMatrix();

	glColor3f(0, 0, 1);
	glPushMatrix();  //nose
	  glTranslatef(7, 0, 0);
	  glRotatef(90, 0, 1, 0);
	  glutSolidCone(1, 2, 36, 3);
	glPopMatrix();

	for (int i = 0; i < 3; i++)  //3 fins
	{
		glPushMatrix();
		  glRotatef(120 * i, 1, 0, 0);
		  drawFin();
		glPopMatrix();
	}
}

void drawRoom()
{
	glDisable(GL_LIGHTING);
	glColor3f(0.7, 0.7,  0.7);			//Floor colour
	for(int i = -100; i <= 100; i +=5)
	{
		glBegin(GL_LINES);			//A set of grid lines on the floor-plane
			glVertex3f(-100, 0, i);
			glVertex3f(100, 0, i);
			glVertex3f(i, 0, -100);
			glVertex3f(i, 0, 100);
		glEnd();
	}

	glEnable(GL_LIGHTING);
	glBegin(GL_QUADS);			//4 walls
	  glColor3f(0.75, 0.75, 0.75); 
	  glNormal3f(0, 0, 1);
	  glVertex3f(-100, 0, -100);
	  glVertex3f(100, 0, -100);
	  glVertex3f(100, 140, -100);
	  glVertex3f(-100, 140, -100);

	  glNormal3f(0, 0, -1);
	  glVertex3f(-100, 0, 100);
	  glVertex3f(100, 0, 100);
	  glVertex3f(100, 140, 100);
	  glVertex3f(-100, 140, 100);

	  glColor3f(1, 0.75, 0.75);
	  glNormal3f(1, 0, 0);
	  glVertex3f(-100, 0, -100);
	  glVertex3f(-100, 140, -100);
	  glVertex3f(-100, 140, 100);
	  glVertex3f(-100, 0, 100);

	  glNormal3f(-1, 0, 0);
	  glVertex3f(100, 0, -100);
	  glVertex3f(100, 140, -100);
	  glVertex3f(100, 140, 100);
	  glVertex3f(100, 0, 100);

	  glColor3f(1, 1, 0.6);
	  glNormal3f(0, -1, 0);
	  glVertex3f(-100, 140, 100);
	  glVertex3f(100, 140, 100);
	  glVertex3f(100, 140, -100);
	  glVertex3f(-100, 140, -100);
	glEnd();

}

void drawFlightPath()
{
	glColor3f(0.0, 0.0, 1.0); 
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < NPTS; i++)
		glVertex3f(ptx[i], pty[i], ptz[i]);
	glEnd();
	glEnable(GL_LIGHTING);
}

//---------------------------------------------
void initialise(void) 
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
	float black[4] = { 0, 0, 0, 1 };
	q = gluNewQuadric();

	loadFlightPath();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
    glClearColor (1.0, 1.0, 1.0, 0.0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(80.0, 1.0, 10.0, 500.0);
}


//------------------------------------------
void display(void)
{
	float lgt_pos[]={0, 50.0f, 0.0f, 1.0f};  //light0 position (above the origin) 

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
	if (option == 2)
	   gluLookAt(0., 10, 30.0, 0., 0., 0., 0., 1., 0.);
	else
		gluLookAt(0., 80, 100.0, 0., 50., 0., 0., 1., 0.);


	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

	glRotatef(angle, 0, 1, 0);

	drawRoom();
	drawFlightPath();
	drawModel();

	glutSwapBuffers();
}



//  Keyboard call-back function
//  Used for selecting a view mode
void keyboard(unsigned char key, int x, int y)
{
	if (key == '1') option = 1;
	else if (key == '2') option = 2;
	glutPostRedisplay();
}


//  Special keyboard call-back function
//  Used for rotating the scene about y-axis
void special(int key, int x, int y)
{
	if(key==GLUT_KEY_LEFT) angle--; 
	else if(key==GLUT_KEY_RIGHT) angle++;
	glutPostRedisplay();
}

//----------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Flight");
   initialise ();
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
