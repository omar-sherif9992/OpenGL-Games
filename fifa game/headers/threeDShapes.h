#include <string>

#pragma once
#ifndef SHAPES_H
#define SHAPES_H

class ThreeDShapes
{

private:
    int windowWidth;
    int windowHeight;

public:
    ThreeDShapes(int windowWidth, int windowHeight);
    void drawWall(double thickness);
    void drawSemiCircle(int x, int y, float r);
    void drawSemiDownCircle(int x, int y, float r);
    void drawHeart(int x, int y);
    void drawRect(int x, int y, int w, int h);
    void drawBullet(int x, int y);
    void drawDownBullet(int x, int y);
    void writeLose(double x, double y);
    void writeWin(double x, double y);

};

#endif // SHAPES_H