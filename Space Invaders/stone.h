#pragma once
#ifndef STONE_H
#define STONE_H

class Stone
{
private:
    int max = 5;
    int damage = 1;
    bool alive = true;
    int windowWidth;
    int windowHeight;
    int randomStoneIndices[5]={0,0,0,0,0};
    int MAX_STONE_TYPES = 3;


public:
    Stone(int windowWidth, int windowHeight);
    void draw();
    void move();

};

#endif // ENEMY_H