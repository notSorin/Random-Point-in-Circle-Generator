/*
Program that demonstrates how to generate a random point inside of a circle, given its radius.

The idea behind the first solution is to generate a random point between 0 and the RADIUS of the circle,
and then rotate that point around the center of the circle by a a random value between 0 and 2*PI radian degrees.
This solution works, however the results are not uniformly distributed in the circle
(there will always be more points generated towards the center of the circle).

The second (uniform) solution is based on the same principle from the first solution, however the random
point between 0 and the RADIUS of the circle is generated in a slightly different manner which assures
uniformity (compare addNonUniformPoint() and addUniformPoint() below to see the difference in points generation).
*/

#include "GL/glut.h"
#include <glm/glm.hpp>
#include <ctime>
#include <list>

using namespace glm;
using namespace std;

//screen constants
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 350;

//other constants
const int CIRCLE_RADIUS = 100;
const float TWICE_PI = 2.0f * M_PI;
const vec2 NON_UNIFORM_CIRCLE_CENTER(150, 150);
const vec2 UNIFORM_CIRCLE_CENTER(500, 150);

//variables
list<vec2> nonUniformPoints, uniformPoints; //lists to hold random generated points

void addUniformPoint()
{
    //first generate a uniform random number in range [0, CIRCLE_RADIUS]
    float randX = sqrt(float(rand()) / float(RAND_MAX)) * CIRCLE_RADIUS;

    //secondly, generate a random rotation angle in range [0, 2*PI]
	float rotationAngle = (float(rand()) / float(RAND_MAX)) * TWICE_PI;

    vec2 randomPoint;

    //rotate the point
    randomPoint.x = randX * cos(rotationAngle);
    randomPoint.y = randX * sin(rotationAngle);

    //place the point relative to the center of the non uniform circle
    randomPoint.x += UNIFORM_CIRCLE_CENTER.x;
    randomPoint.y += UNIFORM_CIRCLE_CENTER.y;

	uniformPoints.push_back(randomPoint);
}

void addNonUniformPoint()
{
    //first generate a non uniform random number in range [0, CIRCLE_RADIUS]
	float randX = (float(rand()) / float(RAND_MAX)) * CIRCLE_RADIUS;

	//secondly, generate a random rotation angle in range [0, 2*PI]
	float rotationAngle = (float(rand()) / float(RAND_MAX)) * TWICE_PI;

    vec2 randomPoint;

    //rotate the point
    randomPoint.x = randX * cos(rotationAngle);
    randomPoint.y = randX * sin(rotationAngle);

    //place the point relative to the center of the uniform circle
    randomPoint.x += NON_UNIFORM_CIRCLE_CENTER.x;
    randomPoint.y += NON_UNIFORM_CIRCLE_CENTER.y;

	nonUniformPoints.push_back(randomPoint);
}

void generatePoints()
{
    nonUniformPoints.clear();
    uniformPoints.clear();

    const int SAMPLES = 5000;

	for(int i = 0; i < SAMPLES; i++)
		addNonUniformPoint();

    for(int i = 0; i < SAMPLES; i++)
		addUniformPoint();
}

// Keyboard function
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a':
		case 'A':
			generatePoints();
			break;
		default:
			break;
	}

	glutPostRedisplay();
}

void drawFilledPolygon(GLfloat x, GLfloat y, GLfloat radius, int sides)
{
	glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i <= sides; i++)
			glVertex2f(x + (radius * cos(i *  TWICE_PI / sides)), y + (radius * sin(i * TWICE_PI / sides)));
	glEnd();
}

void drawStrokePolygon(GLfloat x, GLfloat y, GLfloat radius, int sides)
{
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i <= sides; i++)
			glVertex2f(x + (radius * cos(i *  TWICE_PI / sides)), y + (radius * sin(i * TWICE_PI / sides)));
	glEnd();
}

void display()
{
	glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);

	//non uniform circle
	glColor3f(0, 0, 0);
	drawStrokePolygon(NON_UNIFORM_CIRCLE_CENTER.x, NON_UNIFORM_CIRCLE_CENTER.y, CIRCLE_RADIUS, 30);

	//non uniform points
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
		for(vec2 point : nonUniformPoints)
			glVertex2f(point.x, point.y);
	glEnd();

	//uniform circle
	glColor3f(0, 0, 0);
	drawStrokePolygon(UNIFORM_CIRCLE_CENTER.x, UNIFORM_CIRCLE_CENTER.y, CIRCLE_RADIUS, 30);

	//uniform points
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
		for(vec2 point : uniformPoints)
			glVertex2f(point.x, point.y);
	glEnd();

    glFlush();
}

void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

	srand(unsigned(time(0))); //for random number generation
	generatePoints();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Random Points In Circle");
	glutDisplayFunc(display);
	init();
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
