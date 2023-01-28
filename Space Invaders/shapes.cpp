#include <iostream>
#include <string>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "shapes.h"

using namespace std;
std::string var;

Shapes::Shapes(int windowWidth, int windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
}

// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void Shapes::drawRect(int x, int y, int w, int h)
{
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// draws circles using the (x,y) of the center, and radius (r)
void Shapes::drawCircle(int x, int y, float r)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float degInRad = i * 3.14159 / 180;
        glVertex2f(x + cos(degInRad) * r, y + sin(degInRad) * r);
    }
    glEnd();
}

void Shapes::drawSemiCircle(int x, int y, float r)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 180; i++)
    {
        float degInRad = i * 3.14159 / 180;
        glVertex2f(x + cos(degInRad) * r, y + sin(degInRad) * r);
    }
    glEnd();
}
void Shapes::drawSemiDownCircle(int x, int y, float r)
{
    glBegin(GL_POLYGON);
    for (int i = 180; i < 360; i++)
    {
        float degInRad = i * 3.14159 / 180;
        glVertex2f(x + cos(degInRad) * r, y + sin(degInRad) * r);
    }
    glEnd();
}

void Shapes::drawBullet(int x, int y)
{
    glColor3f(255.0 / 2, 102.0 / 2, 0);
    glBegin(GL_POLYGON);
    glVertex2f(x - 15, y);
    glVertex2f(x + 15, y);
    glVertex2f(x, y - 50);
    glEnd();
    glColor3f(1.0, 0, 0);
    drawCircle(x, y, 15);
}
void Shapes::drawDownBullet(int x, int y)
{

    glColor3f(255.0 / 2, 102.0 / 2, 0);
    glBegin(GL_POLYGON);
    glVertex2f(x - 15, y - 50);
    glVertex2f(x + 15, y - 50);
    glVertex2f(x, y);
    glEnd();
    glColor3f(1.0, 0, 0);
    drawCircle(x, y - 50, 15);
}
void Shapes::writeLose(double x, double y)
{
    int len, i;
    glColor3f(1.0, 0.0, 0.0);

    // set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x-200, y);

    // get the length of the string to display
    string str = "GAME OVER YOU LOSE ! THE ENEMY WON , PRESS E TO EXIT, PRESS R TO RESTART";
    len = (int)(str.length());

    // loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
}
void Shapes::writeWin(double x, double y)
{
    int len, i;
    glColor3f(0.0, 1.0, 0.0);

    // set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x, y);

    // get the length of the string to display
    string str = "GAME OVER YOU WIN , YOU DEFEATED THE ENEMY ! , PRESS E TO EXIT , PRESS R TO RESTART";
    len = (int)(str.length());

    // loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
}
void Shapes::drawHeart(int x, int y)
{
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x - 50, y + 60);
    glVertex2f(x - 25, y + 130 - 20);
    glVertex2f(x, y + 100 - 20);
    glVertex2f(x + 25, y + 130 - 20);
    glVertex2f(x + 50, y + 80 - 20);
    glVertex2f(x, y);
    glEnd();
    drawCircle(x - 30 + 1, y + 103 - 20 - 1 - 1, 29);
    drawCircle(x + 30 - 1, y + 103 - 20 - 1 - 1, 28.5);
}
