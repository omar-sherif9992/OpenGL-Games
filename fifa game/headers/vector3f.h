#include <string>

#pragma once
#ifndef VECTOR3F_H
#define VECTOR3F_H

class Vector3f
{

public:
    float x, y, z;
    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
    Vector3f operator+(Vector3f &v);
    Vector3f operator-(Vector3f &v);
    Vector3f operator*(float n);
    Vector3f operator/(float n);
    Vector3f cross(Vector3f v);
    Vector3f unit();
};

#endif // VECTOR3F_H