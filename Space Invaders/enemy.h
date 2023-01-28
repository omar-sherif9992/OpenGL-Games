#pragma once
#ifndef ENEMY_H
#define ENEMY_H


class Enemy
{
private:
    double health = 100;
    double damage = 1;
    bool alive = true;
    double windowWidth;
    double windowHeight;
    double posX;
    double posY;
    double width;
    double height;
    double step = 0.5;

public:
    Enemy(double windowWidth, double windowHeight);
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