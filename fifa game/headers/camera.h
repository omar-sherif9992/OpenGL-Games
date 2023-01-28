#include <string>

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

class Camera
{

private:
    int windowWidth;
    int windowHeight;
    // View
    enum View
    {
        SIDE = 'SIDE',
        FRONT = 'FRONT',
        TOP = 'TOP'
    };

public:
    Camera(int windowWidth, int windowHeight);
    void changeView();
    void setupCamera();
    void setupLight();
};

#endif // CAMERA_H