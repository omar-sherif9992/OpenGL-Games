#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <algorithm>
#include <math.h>
#include <random>

#include <chrono>
#include <thread>

#include "headers/vector3f.h"

Vector3f::Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3f::Vector3f operator+(Vector3f &v)
{
	return Vector3f(x + v.x, y + v.y, z + v.z);
}

Vector3f::Vector3f operator-(Vector3f &v)
{
	return Vector3f(x - v.x, y - v.y, z - v.z);
}

Vector3f::Vector3f operator*(float n)
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
}
;
