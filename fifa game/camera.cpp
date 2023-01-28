#include <iostream>
#include <unistd.h> //required for usleep()
#include <iostream>
#include <string>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "headers/camera.h"

using namespace std;
using std::string;



void Camera::setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, 0.5, -0.5, 0.5, -1, 1);
	//gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


Vector3f eye, center, top;

	Cam(float X = 1.0f, float Y = 1.0f, float Z = 1.0f, float cenX = 0.0f, float cenY = 0.0f, float cenZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(X, Y, Z);
		center = Vector3f(cenX, cenY, cenZ);
		top = Vector3f(upX, upY, upZ);
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			top.x, top.y, top.z
		);
	}
	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = top.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + top * sin(DEG2RAD(a));
		top = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = top.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(top);
		center = eye + view;
	}
	void moveX(float d) {
		Vector3f right = top.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + top.unit() * d;
		center = center + top.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}