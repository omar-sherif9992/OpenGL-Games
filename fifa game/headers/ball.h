#pragma once
#ifndef BALL_H
#define BALL_H


class Ball
{
private:

    double windowWidth;
    double windowHeight;
    double posX;
    double posY;
    double posZ;
    double width;
    double height;
    double step = 0.5;

public:
    Ball(double windowWidth, double windowHeight);
    void draw();
    void die();
    void shoot();
    void getHit();
    void getPowerUp();
    int getHealth();
    void changeHealthBar();
    void setHealth(int health);
    int getDamage();
    void setDamage(int damage);
    void setLives(int lives);
    bool isAlive();
    void setAlive(bool alive);

    // movment
    void move();
    void toogleStep();
    int getStep();

    void moveBullets();
    void drawBullets();

    // Getters and Setters
    int getPosX();
    void setPosX(int posX);
    int getPosY();
    void setPosY(int posY);
    int getWidth();
    void setWidth(int width);
    int getHeight();
    void setHeight(int height);

    void moveBullet();
};

#endif // ENEMY_H