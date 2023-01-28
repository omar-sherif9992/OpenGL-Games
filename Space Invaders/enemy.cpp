#include <iostream>
#include <string>
#include <GL/glut.h>
#include <stdlib.h>
#include <queue> // std::queue

#include <iostream>

#include "enemy.h"
#include "shapes.h"

using namespace std;
using std::string;


Enemy::Enemy(double windowWidth, double windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->posX = windowWidth / 2;
    this->posY = (4 * windowHeight) / 5;
    this->width = 200;
    this->height = 90;
}

void Enemy::draw()
{
    Shapes shapes = Shapes(0, 0);
    if(!this->isAlive()) {
        shapes.writeWin(windowWidth / 3,(4 * windowHeight) / 5);
        return;
    }
    changeHealthBar();

    // black control room
    glColor3f(0, 0, 0);
    shapes.drawSemiCircle(this->posX + this->width / 2, this->posY + 3 * this->height / 4, 2.5 * this->height / 4);

    // person
    glColor3f(0.5, 1, 1);
    shapes.drawRect(posX + 2 * width / 4 - 10, posY + height - 10, width / 10, height / 4);
    glColor3f(1, 1, 1);
    shapes.drawCircle(posX + width / 2, posY + height + 10, 10);
    glColor3f(0, 0, 0);
    shapes.drawCircle(posX + width / 2 + 4, posY + height + 15, 3);
    shapes.drawCircle(posX + width / 2 - 5, posY + height + 15, 3);

    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5);
    glVertex2f(this->posX, this->posY);
    glVertex2f(this->posX + this->width / 4, this->posY + this->height);
    glVertex2f(this->posX + 3 * this->width / 4, this->posY + this->height);
    glVertex2f(this->posX + this->width, this->posY);
    glEnd();

    glColor3f(1, 1, 0);

    // left lamp
    shapes.drawCircle(this->posX + this->width / 4, this->posY + 2 * this->height / 5, this->height / 7);
    glColor3f(1, 1, 0);
    // right lamp
    shapes.drawCircle(this->posX + 3 * this->width / 4, this->posY + 2 * this->height / 5, this->height / 7);

    /*
        // left eyebrow
        glColor3f(0.5, 0.5, 0.3);
        shapes.drawSemiCircle(this->posX + this->width / 4, this->posY + 3 * this->height / 4, 2.5 * this->height / 4);

        // right eyebrow
        shapes.drawSemiCircle(this->posX + 3 * this->width / 4, this->posY + 3 * this->height / 4, 2.5 * this->height / 4);
    */

    // left eye
    glColor3f(0.3, 0.3, 0);
    shapes.drawSemiCircle(this->posX + this->width / 4, this->posY + 2 * this->height / 5, this->height / 7);

    // right eye
    shapes.drawSemiCircle(this->posX + 3 * this->width / 4, this->posY + 2 * this->height / 5, this->height / 7);

    // shooter
    glColor3f(0, 0, 0);
    shapes.drawRect(this->posX + this->width / 2 - 5, this->posY + this->height / 2, 10, this->height / 2);

    shapes.drawRect(this->posX + this->width / 3 + 24, this->posY - 10, this->height / 4, this->height / 2);

    shapes.drawCircle(this->posX + this->width / 2, this->posY, this->height / 5);


}

void Enemy::move()
{
    if ((this->posX + this->width) >= (10 * this->windowWidth / 11))
    {
        toogleStep();
    }
    else if ((this->posX) <= (this->windowWidth / 11))
    {
        toogleStep();
    }

    this->posX += this->step;
}

void Enemy::changeHealthBar()
{
    glColor3f(1.0, 0.0, 0.0);
    Shapes shapes = Shapes(0, 0);
    if (this->health == 100)
    {
        shapes.drawSemiCircle(30, this->posY + 158, 15);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);

        shapes.drawSemiCircle(30, this->posY + 158, 15);
    }
    glColor3f(1.0, 0.0, 0.0);

    if (health >= 1)
        shapes.drawSemiDownCircle(30, this->posY - 90, 15);
    else
    {
        glColor3f(1.0, 1.0, 1.0);

        shapes.drawSemiDownCircle(30, this->posY - 90, 15);
    }
    glColor3f(1.0, 1.0, 1.0);

    shapes.drawRect(15, this->posY - 90, 30, 100 * 2.5);
    glColor3f(1.0, 0.0, 0.0);

    shapes.drawRect(15, this->posY - 90, 30, this->health * 2.5);
}

void Enemy::toogleStep()
{
    this->step = this->step * -1;
}
int Enemy::getStep()
{
    return this->step;
}
void Enemy::die()
{
    this->health = 0;
    this->alive = false;
}

int Enemy::getHealth()
{
    return this->health;
}
void Enemy::getHit()
{
    this->health -= 10;
    this->changeHealthBar();
    if (this->health <= 0)
    {
        this->die();
    }
}

bool Enemy::isAlive()
{
    return this->alive;
}

// Getters and Setters
int Enemy::getPosX()
{
    return this->posX;
}
void Enemy::setPosX(int posX)
{
    this->posX = posX;
}
int Enemy::getPosY()
{
    return this->posY;
}
void Enemy::setPosY(int posY)
{
    this->posY = posY;
}
int Enemy::getWidth()
{
    return this->width;
}
void Enemy::setWidth(int width)
{
    this->width = width;
}
int Enemy::getHeight()
{
    return this->height;
}
void Enemy::setHeight(int height)
{
    this->height = height;
}
