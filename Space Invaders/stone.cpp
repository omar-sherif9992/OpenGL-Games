#include <iostream>
#include <string>
#include "stone.h"

#define ARRAYSIZE(array) (sizeof(array)/sizeof(array[0]))

using namespace std;
using std::string;

Stone::Stone(int windowWidth, int windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    cout << "Stone created" << endl;
    // array size
    int size = 10;
    // array of stones
 


    for(int i = 0;i < ARRAYSIZE(this->randomStoneIndices) ;i++) {
		randomStoneIndices[i]=rand()%MAX_STONE_TYPES;
	}

}
void Stone::draw()
{
    cout << "draw" << endl;
}
void Stone::move()
{
    cout << "Stone moved" << endl;
}
