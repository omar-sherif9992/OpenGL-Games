#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include "headers/player.h"
#include "headers/ball.h"
class Vector3f
{
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f &v)
	{
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f &v)
	{
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n)
	{
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n)
	{
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit()
	{
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v)
	{
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	void operator+=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector3f Eye(20, 5, 20);
Vector3f At(0, 0, 0);
Vector3f Up(0, 1, 0);

// WINDOW SETTINGS
double windowWidth;
double windowHeight;
double windowPosX = 0;
double windowPosY = 0;

/* // Model Variables
Model_3DS model_house;
Model_3DS model_tree;

// Textures
GLTexture tex_ground; */

//=======================================================================
// Global Variables
//======================================================================

Player player = Player(0, 0);
int cameraZoom = 0;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)windowWidth / (GLdouble)windowHeight;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = {96.0f};
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = {0.1f, 0.1f, 0.1, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = {0.0f, 10.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}
//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	// Window settings
	windowWidth = (glutGet(GLUT_SCREEN_WIDTH));
	windowHeight = (glutGet(GLUT_SCREEN_HEIGHT));
	aspectRatio = (GLdouble)windowWidth / (GLdouble)windowHeight;

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//-----------------

//  Methods Signatures
void Anim();
void cube(void);
void Key(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();

//-----------------

void Display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	// define the Global Variables
	player = Player(windowWidth, windowHeight);

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutCreateWindow("Fifa Game");

	glutDisplayFunc(Display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
	return 0;
}