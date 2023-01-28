#include <iostream>
#include <unistd.h> //required for usleep()
#include <iostream>
#include <string>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "headers/player.h"
#include "headers/shapes.h"

using namespace std;
using std::string;

Player::Player(double windowWidth, double windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->lifes = 3;
    this->posX = windowWidth / 2;
    this->posY = windowHeight / 4;
    this->width = 90;
    this->height = 50;
}

void Player::draw()
{
    Shapes shapes = Shapes(windowWidth, windowHeight);

    // shooter
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(this->posX, this->posY);
    glVertex2f(this->posX, this->posY + this->height);
    glVertex2f(this->posX - this->width / 2, this->posY + 60);
    glVertex2f(this->posX - this->width / 5, this->posY + 130 - 20);
    glVertex2f(this->posX, this->posY + 100 - 20);
    glVertex2f(this->posX + this->width / 4, this->posY + 130 - 20);
    glVertex2f(this->posX + this->width / 2, this->posY + 80 - 20);
    glEnd();

    // body
    glColor3f(1.0 / 3, 1.0 / 3, 1.0 / 3);
    glBegin(GL_POLYGON);

    glVertex2f(this->posX - this->width / 2, this->posY + this->height - 10);
    glVertex2f(this->posX - this->width, this->posY - this->height / 2);
    glVertex2f(this->posX + this->width, this->posY - this->height / 2);
    glVertex2f(this->posX + this->width / 2, this->posY + this->height - 10);
    glEnd();

    // left engine
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(this->posX - this->width / 2, this->posY + this->height - 10);
    glVertex2f(this->posX - this->width - 10, this->posY - this->height / 2 - 10);

    glVertex2f(this->posX - this->width / 2 - 10, this->posY - this->height / 2 - 10);
    glVertex2f(this->posX - this->width / 2, this->posY + this->height - 10);
    glEnd();

    // right engine
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(this->posX + this->width / 2, this->posY + this->height - 10);
    glVertex2f(this->posX + this->width + 10, this->posY - this->height / 2 - 10);

    glVertex2f(this->posX + this->width / 2 + 10, this->posY - this->height / 2 - 10);
    glVertex2f(this->posX + this->width / 2, this->posY + this->height - 10);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    shapes.drawSemiCircle(this->posX, this->posY + this->height, this->width / 2);

    glColor3f(1.0, 1.0, 1.0);
    shapes.drawCircle(this->posX - 1 * this->width / 4, this->posY + this->height / 2, this->width / 10);
    shapes.drawCircle(this->posX + 1 * this->width / 4, this->posY + this->height / 2, this->width / 10);
}

void Player::moveRight()
{
    if (this->posX + this->width < (10 * this->windowWidth / 11))
    {
        this->posX += 10;
    }
}

void Player::moveLeft()
{
    if (this->posX > this->windowWidth / 11)
    {
        this->posX -= 10;
    }
}
void Player::moveUp()
{
    if (this->posY < 5 * this->windowHeight / 11)
    {
        this->posY += 10;
    }
}
void Player::moveDown()
{
    if (this->posY > this->windowHeight / 11)
    {
        this->posY -= 10;
    }
}

double Player::getPosX()
{
    return posX;
}
void Player::setPosX(double posX)
{
    this->posX = posX;
}
double Player::getPosY()
{
    return posY;
}
void Player::setPosY(double posY)
{
    this->posY = posY;
}
double Player::getPosZ()
{
    return posZ;
}
void Player::setPosY(double posZ)
{
    this->posZ = posZ;
}
double Player::getWidth()
{
    return width;
}
void Player::setWidth(double width)
{
    this->width = width;
}
double Player::getHeight()
{
    return height;
}
void Player::setHeight(double height)
{
    this->height = height;
}
