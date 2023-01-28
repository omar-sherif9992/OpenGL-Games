#pragma once
#ifndef HERO_H
#define HERO_H

class Hero
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
    double width;
    double height;
    double shieldTime = 6;

private:
    bool shieldsUp = false;
    double windowWidth;
    double windowHeight;

public:
    Hero(double windowWidth, double windowHeight);
    void draw();
    void shoot();
    bool isShieldsUp();
    void setShieldsUp(bool isShieldsUp);
    int getLives();
    void setLives(int lives);

    void drawLifes();
    void colorShield();
    void die();
    void getHit();
    void getPowerUp();
    int getLifes();

    // Getters and Setters
    double getPosX();
    void setPosX(double posX);
    double getPosY();
    void setPosY(double posY);
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