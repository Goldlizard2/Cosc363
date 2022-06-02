
#include <math.h>
#include <GL/glut.h>
#include "RailModels.h"
#include <time.h>
//---------------------------------------------------------------------
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
uint16_t counter;
bool run = 1;
const int NPTS = 307;
int option = 1;
float ptx[NPTS * 3], ptz[NPTS * 3];
float ptxoriginal[NPTS], ptzoriginal[NPTS];
int k = 0;
GLUquadric *q;

float angle = -39, look_x = -56, look_z = -10, eye_x = 40, eye_y = 100, eye_z = -35;

void loadtrack()
{
    ifstream ifile;
    ifile.open("Dshape.txt");
    for (int i = 0; i < NPTS; i++)
    {
        ifile >> ptxoriginal[i] >> ptzoriginal[i];
    }
    ifile.close();
}

void equilizePoints()
{

    int i = 0; // index of the original array
    float xcurr = ptxoriginal[0], ycurr = ptzoriginal[0];
    float xprev, yprev;
    float d, d1 = 0, d2, dreq = 0.99;

    while (i < NPTS - 1)
    {
        glm::vec2 curr(xcurr, ycurr);
        glm::vec2 ptss(ptxoriginal[i], ptzoriginal[i]);
        d = glm::distance(curr, ptss);
        // distance(xcurr, ycurr, ptxoriginal[i], ptzoriginal[i]);
        if (d < dreq)
        {
            d1 = d;
            xprev = ptxoriginal[i];
            yprev = ptzoriginal[i];
            i++;
        }
        else
        {
            d2 = d;
            xcurr = xprev + (dreq - d1) * (ptxoriginal[i] - xprev) / (d2 - d1);
            ycurr = yprev + (dreq - d1) * (ptzoriginal[i] - yprev) / (d2 - d1);
            ptx[k] = xcurr; // Store values
            ptz[k] = ycurr;
            k++;
            d1 = 0;
            xprev = xcurr;
            yprev = ycurr;
        }
    }
}

void initialize(void)
{


    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4] = {1.0, 1.0, 1.0, 1.0};

    q = gluNewQuadric();
    loadTexture(0, "photos2.bmp");
    loadTexture(1, "floor.bmp");

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    //	Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.0, 0.0, 0.0, 0.0); // Background colour

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 20.0, 500.0);
    loadtrack();
    equilizePoints();
}

void drawLocomotive()
{
    int8_t wagonNumber = 5;
    int8_t i;
    float theta = 0;
    int offset = 34;
    int location = counter;

    for (i = 0; i < wagonNumber + 1; i++)
    {
        if (i > 0)
        {
            location -= offset;
        }

        if (location - 1 < 0)
        {
            location = k + location - 1;
        }
        glm::vec2 pts_1(ptx[location - 1], ptz[location - 1]);
        glm::vec2 pts_curr(ptx[location], ptz[location]);
        if (location + 1 > k - 1)
        {
            location = -1;
        }
        glm::vec2 pts_2(ptx[location + 1], ptz[location + 1]);
        glm::vec2 pts_delta = pts_2 - pts_1;
        glm::vec2 unit_vector = glm::normalize(pts_delta);
        theta = (atan2(unit_vector[1], -unit_vector[0])) * (180 / M_PI);

        glPushMatrix();
        glTranslatef(pts_curr[0], 1, pts_curr[1]);
        glRotatef(theta, 0, 1, 0);
        if (i == 0)
        {
            engine();
        }
        else
        {
            wagon();
        }

        glPopMatrix();
    }
}

void drawstation()
{
    float theta;
    theta = 0;
    glm::vec2 pts_curr(ptx[50], ptz[70]);
    glm::vec2 pts_1(ptx[50], ptz[50]);
    glm::vec2 pts_2(ptx[60], ptz[60]);
    glm::vec2 pts_delta = pts_2 - pts_1;
    glm::vec2 unit_vector = glm::normalize(pts_delta);
    theta = (atan2(unit_vector[1], -unit_vector[0])) * (180 / M_PI);
    glPushMatrix();
    glTranslatef(pts_curr[0], 1, pts_curr[1]);
    glTranslatef(-30, 1, 0);
    glRotatef(theta, 0, 1, 0);
    station(); // locomotive
    glPopMatrix();
}

void drawboomArm()
{
    static int boomangle = 0;
    float theta;
    theta = 0;
    glm::vec2 pts_curr(ptx[60], ptz[70]);
    glm::vec2 pts_1(ptx[60], ptz[60]);
    glm::vec2 pts_2(ptx[70], ptz[70]);
    glm::vec2 pts_delta = pts_2 - pts_1;
    glm::vec2 unit_vector = glm::normalize(pts_delta);
    theta = (atan2(unit_vector[1], -unit_vector[0])) * (180 / M_PI);
    
    glPushMatrix();
    glTranslatef(pts_curr[0], 1, pts_curr[1]);
    glTranslatef(-28, 1, 19);
    glRotatef(theta - 90, 0, 1, 0);
    boomBase();
    glPopMatrix();

    if (counter == 80 && boomangle < 60)
    {
        boomangle++;
    }
    else if (counter > 300 && boomangle > 0)
    {
        boomangle--;
    }
    glPushMatrix();
    glTranslatef(pts_curr[0], 1, pts_curr[1]);
    glTranslatef(-20, 1, 25);
    glRotatef(theta - 90, 0, 1, 0);
    glRotatef(boomangle, 0, 0, 1);
    boomarm(); // locomotive
    glPopMatrix();
}

void drawTunnel()
{
    glPushMatrix();
    glTranslatef(ptx[380], 1, ptz[438]);
    tunnel();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(ptx[363], 1, ptz[375]);
    glRotatef(180, 0, 1, 0);
    tunnel();
    glPopMatrix();
}

void drawSleepers(glm::vec2 v1, glm::vec2 pt1, glm::vec2 unit_vector)
{
    float s1 = 8, s2 = 2;

    glm::vec2 a_1 = pt1 + v1 * s1;
    glm::vec2 a_2 = pt1 + v1 * s1 + s2 * unit_vector;
    glm::vec2 b_1 = pt1 + -(v1 * s1);
    glm::vec2 b_2 = pt1 + -(v1 * s1 + s2 * unit_vector);
    glColor4f(0.54, 0.27, 0.07, 1.0);

    // sleepers
    glNormal3f(0., 1., 0.); // Quad 1 facing up
    glVertex3f(a_1[0], 0.1, a_1[1]);
    glVertex3f(a_2[0], 0.1, a_2[1]);
    glVertex3f(b_2[0], 0.1, b_2[1]);
    glVertex3f(b_1[0], 0.1, b_1[1]);
    glColor4f(0.5, 0.3, 0.2, 1.0);
}

void drawTrainTracks()
{

    float ca1, sa1, ca2, sa2;
    float w1 = 5, w2 = 7;
    glm::vec2 pts_2(ptx[0], ptz[0]);
    glm::vec2 pts_3(ptx[0], ptz[0]);
    glBegin(GL_QUADS);
    for (int j = 0; j < 2; j++) // Two parallel tracks (radius = inRad, outRad)
    {
        for (uint16_t i = 0; i < k - 1; i++)
        {

            glm::vec2 pts_1(ptx[i], ptz[i]);

            if ((i + 2) == k)
            {
                pts_2 = glm::vec2(ptx[0], ptz[0]);
                pts_3 = glm::vec2(ptx[1], ptz[1]);
            }
            else
            {
                pts_2 = glm::vec2(ptx[i + 1], ptz[i + 1]);
                pts_3 = glm::vec2(ptx[i + 2], ptz[i + 2]);
            }

            glm::vec2 pts_delta = pts_2 - pts_1;
            glm::vec2 pts_delta2 = pts_3 - pts_2;
            glm::vec2 unit_vector = glm::normalize(pts_delta);
            glm::vec2 unit_vector2 = glm::normalize(pts_delta2);
            glm::vec2 v_1(unit_vector[1], -unit_vector[0]);
            glm::vec2 v_2(unit_vector2[1], -unit_vector2[0]);
            glm::vec2 a_1 = pts_1 + v_1 * w1;
            glm::vec2 a_2 = pts_1 + v_1 * w2;
            glm::vec2 b_1 = pts_2 + v_2 * w1;
            glm::vec2 b_2 = pts_2 + v_2 * w2;
            glm::vec2 c_1 = pts_1 + v_1 * -w1;
            glm::vec2 c_2 = pts_1 + v_1 * -w2;
            glm::vec2 d_1 = pts_2 + v_2 * -w1;
            glm::vec2 d_2 = pts_2 + v_2 * -w2;

            // Inside of the track
            glNormal3f(0., 1., 0.);
            glVertex3f(c_1[0], 1.0, c_1[1]);
            glVertex3f(c_2[0], 1, c_2[1]);
            glVertex3f(d_2[0], 1.0, d_2[1]);
            glVertex3f(d_1[0], 1, d_1[1]);

            glNormal3f(-sa1, 0, -ca1);
            glVertex3f(c_1[0], 0, c_1[1]);
            glVertex3f(c_1[0], 1, c_1[1]);
            glNormal3f(-sa2, 0.0, -ca2);
            glVertex3f(d_1[0], 1, d_1[1]);
            glVertex3f(d_1[0], 0, d_1[1]);

            glNormal3f(sa1, 0, ca1);
            glVertex3f(c_2[0], 0, c_2[1]);
            glVertex3f(c_2[0], 1.0, c_2[1]);
            glNormal3f(sa2, 0.0, ca2);
            glVertex3f(d_2[0], 1, d_2[1]);
            glVertex3f(d_2[0], 0.0, d_2[1]);

            // Outside of the track

            glNormal3f(0., 1., 0.); // Quad 1 facing up
            glVertex3f(a_1[0], 1.0, a_1[1]);
            glVertex3f(a_2[0], 1.0, a_2[1]);
            glVertex3f(b_2[0], 1.0, b_2[1]);
            glVertex3f(b_1[0], 1.0, b_1[1]);

            glNormal3f(-sa1, 0.0, -ca1); // Quad 2 facing inward
            glVertex3f(a_1[0], 0, a_1[1]);
            glVertex3f(a_1[0], 1.0, a_1[1]);
            glNormal3f(-sa2, 0.0, -ca2);
            glVertex3f(b_1[0], 1, b_1[1]);
            glVertex3f(b_1[0], 0, b_1[1]);

            glNormal3f(sa1, 0.0, ca1); // Quad 3 facing outward
            glVertex3f(a_2[0], 0, a_2[1]);
            glVertex3f(a_2[0], 1.0, a_2[1]);
            glNormal3f(sa2, 0.0, ca2);
            glVertex3f(b_2[0], 1, b_2[1]);
            glVertex3f(b_2[0], 0.0, b_2[1]);

            if (i % 5 == 0)
                drawSleepers(v_1, pts_1, unit_vector);
        }
    }
    glEnd();
}

void special(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        angle -= 0.1;
    else if (key == 'c' && option == 1)
        option = 2;
    else if (key == 'c' && option == 2)
        option = 1;
    else if (key == GLUT_KEY_RIGHT)
        angle += 0.1;
    else if (key == GLUT_KEY_PAGE_UP && eye_y < 200)
        eye_y += 3;
    else if (key == GLUT_KEY_PAGE_DOWN && eye_y > 20)
        eye_y -= 3;
    else if (key == GLUT_KEY_DOWN)
    { // Move backward
        eye_x -= 5 * sin(angle);
        eye_z += 5 * cos(angle);
    }
    else if (key == GLUT_KEY_UP)
    { // Move forward
        eye_x += 5 * sin(angle);
        eye_z -= 5 * cos(angle);
    }

    look_x = eye_x + 100 * sin(angle);
    look_z = eye_z - 100 * cos(angle);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c' && option == 1)
    {
        option = 2;
    }
    else if (key == 'c' && option == 2)
    {
        option = 1;
    }

    glutPostRedisplay();
}

void display(void)
{
    float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f}; // light0 position (directly above the origin)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (option == 2)
    {
        gluLookAt(-74.6, 19, -86.10, -117.3, 10, 4.33, 0, 1, 0);
    }
    else
    {
        gluLookAt(eye_x, eye_y, eye_z, look_x, 10, look_z, 0, 1, 0);
    }

    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos); // light position
    glRotatef(angle, 0, 1, 0);
    drawTrainTracks();
    drawLocomotive();
    drawTunnel();
    drawstation();
    drawboomArm();
    floor();

    glutSwapBuffers(); // Useful for animation
}

void timer(int value)
{
    static int counter2 = 0;
    int train_stop = 80;
    int train_start = 100;
    if (counter == train_stop)
    {
        if (counter2 < train_start)
            counter2++;
        else
        {
            counter++;
            counter2 = 0;
        }
    }
    else
    {
        counter++;
    }
    if (counter >= (k - 1))
    {

        counter = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(1, timer, value);
}
//---------------------------------------------------------------------
int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Toy Train");
    initialize();
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(10, timer, 0);
    glutMainLoop();
    return 0;
}
