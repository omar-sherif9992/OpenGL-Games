#include <iostream>
#include <unistd.h> //required for usleep()
#include <iostream>
#include <string>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "hero.h"
#include "shapes.h"

using namespace std;
using std::string;

Hero::Hero(double windowWidth, double windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->lifes = 3;
    this->posX = windowWidth / 2;
    this->posY = windowHeight / 4;
    this->width = 90;
    this->height = 50;
}
void Hero::shoot()
{
    cout << "shoot" << endl;
}
bool Hero::isShieldsUp()
{
    return shieldsUp;
}
void Hero::draw()
{
    Shapes shapes = Shapes(windowWidth, windowHeight);
    if (this->getLifes() <= 0)
    {
        shapes.writeLose(windowWidth / 3, windowHeight / 4);

        return;
    }

    if (this->isShieldsUp())
    {
        colorShield();
    }

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

    // Draw lifes
    this->drawLifes();
}
void Hero::setShieldsUp(bool isShieldsUp)
{
    this->shieldsUp = isShieldsUp;
}

double Hero::getShieldTime()
{
    return this->shieldTime;
}

void Hero::drawLifes()
{
    Shapes shapes = Shapes(0, 0);
    glColor3f(1.0, 0.0, 0.0);
    for (int i = 0; i < this->getLifes(); i++)
    {
        shapes.drawHeart(11.5 * this->windowWidth / 12, 40 + (i * 100));
    }
}

int Hero::getLifes()
{
    return this->lifes;
}
void Hero::setLives(int lifes)
{
    this->lifes = lifes;
}
void Hero::moveRight()
{
    if (this->posX + this->width < (10 * this->windowWidth / 11))
    {
        this->posX += 10;
    }
}

void Hero::moveLeft()
{
    if (this->posX > this->windowWidth / 11)
    {
        this->posX -= 10;
    }
}
void Hero::moveUp()
{
    if (this->posY < 5 * this->windowHeight / 11)
    {
        this->posY += 10;
    }
}
void Hero::moveDown()
{
    if (this->posY > this->windowHeight / 11)
    {
        this->posY -= 10;
    }
}

void Hero::colorShield()
{
    glColor3f(0.5, 25.0 / 3, 0.2);
    // shooter
    glBegin(GL_POLYGON);
    glVertex2f(this->posX - 2, this->posY - 2);
    glVertex2f(this->posX - 2, this->posY + this->height + 2);
    glVertex2f(this->posX - this->width / 2 - 2, this->posY + 60 + 2);
    glVertex2f(this->posX - this->width / 5 - 2, this->posY + 130 - 20 + 2);
    glVertex2f(this->posX - 2, this->posY + 100 - 20 + 2);
    glVertex2f(this->posX + this->width / 4 + 2, this->posY + 130 - 20 + 2);
    glVertex2f(this->posX + this->width / 2 + 2, this->posY + 80 - 20 + 2);
    glEnd();

    // body
    glBegin(GL_POLYGON);

    glVertex2f(this->posX - this->width / 2 - 2, this->posY + this->height - 10 - 2);
    glVertex2f(this->posX - this->width - 2, this->posY - this->height / 2 - 2);
    glVertex2f(this->posX + this->width + 2, this->posY - this->height / 2 - 2);
    glVertex2f(this->posX + this->width / 2 + 2, this->posY + this->height - 10 - 2);
    glEnd();

    // left engine
    glBegin(GL_POLYGON);
    glVertex2f(this->posX - this->width / 2 - 2, this->posY + this->height - 10 - 2);
    glVertex2f(this->posX - this->width - 10 - 2, this->posY - this->height / 2 - 10 - 2);

    glVertex2f(this->posX - this->width / 2 - 10 - 2, this->posY - this->height / 2 - 10 - 2);
    glVertex2f(this->posX - this->width / 2 - 2, this->posY + this->height - 10 + 2);
    glEnd();

    // right engine
    glBegin(GL_POLYGON);
    glVertex2f(this->posX + this->width / 2 + 2, this->posY + this->height - 10 - 2);
    glVertex2f(this->posX + this->width + 10 + 2, this->posY - this->height / 2 - 10 - 2);

    glVertex2f(this->posX + this->width / 2 + 10 + 2, this->posY - this->height / 2 - 10 - 2);
    glVertex2f(this->posX + this->width / 2 + 2, this->posY + this->height - 10 - 2);
    glEnd();
}

void Hero::die()
{
}

void Hero::getHit()
{
    this->lifes -= 1;
}

void Hero::getPowerUp()
{
    cout << "getPowerUp" << endl;
}

double Hero::getPosX()
{
    return posX;
}
void Hero::setPosX(double posX)
{
    this->posX = posX;
}
double Hero::getPosY()
{
    return posY;
}
void Hero::setPosY(double posY)
{
    this->posY = posY;
}
double Hero::getWidth()
{
    return width;
}
void Hero::setWidth(double width)
{
    this->width = width;
}
double Hero::getHeight()
{
    return height;
}
void Hero::setHeight(double height)
{
    this->height = height;
}
