#pragma once
#ifndef HERO_H
#define HERO_H

class Player
{
    // Directions
    enum Direction
    {
        UP = 'w',
        DOWN = 's',
        LEFT = 'a',
        RIGHT = 'd'
    };

    int lifes = 3;
    int score;
    double posX;
    double posY;
    double posZ;
    double width;
    double height;

private:
    double windowWidth;
    double windowHeight;

public:
    Player(double windowWidth, double windowHeight);
    void draw();
    void shoot();

    // Getters and Setters
    double getPosX();
    void setPosX(double posX);
    double getPosY();
    void setPosY(double posY);
    double getPosZ();
    void setPosZ(double posZ);
    double getWidth();
    void setWidth(double width);
    double getHeight();
    void setHeight(double height);
    double getShieldTime();

    // Movement
    void moveRight();
    void moveLeft();
    void moveDown();
    void moveUp();
};

#endif // HERO_H