#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include<iostream>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <MMSystem.h>
#include <mciapi.h>
#include <stdlib.h>
#include <glut.h>
#include<algorithm>
#include <math.h>
#include <random>
#include <Mmsystem.h>
#include <mciapi.h>
#include <playsoundapi.h>
#include <string>     // to use std::string, std::to_string() and "+" operator acting on strings 
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>


#pragma comment(lib,"winmm.lib")

using namespace std;

using std::string;


#define getMin(a,b) (((a) > (b)) ? (b) : (a))
#define getMax(a,b) (((a) > (b)) ? (a) : (b))
float fmodf(float x, float y);
void drawGroundTimer(int dx);
void invaulnerabilityTimer(int x);
using std::string;

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
char title[] = "Subway Surfers Game";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

int cameraZoom = 0;

// Model Variables
Model_3DS model_ground;
Model_3DS model_player;
Model_3DS model_barrier;
Model_3DS model_key;
Model_3DS model_jetpack;
Model_3DS model_shield;
Model_3DS model_doubleTime;
Model_3DS model_chest;
Model_3DS model_train;
Model_3DS model_light;
Model_3DS model_house;



// Textures
GLTexture tex_ground;
GLTexture tex_grass;

// Global variables variables
// boundaries
float upperBoundary = 3.5;
float lowerBoundary = -1.5;
float leftBoundary = -6;
float rightBoundary = 10.0;

// Ground
bool isGroundTimerInitiated = false;
double groundX = 0; // dx for ground 

// lanes
//float leftLane = -10.0;// level 2 player position on it  and newer player left boundary +=leftLane 
//float rightLane = 10.0; // level 2 player position on it and newer player right boundary +=rightLane
int lane = 1; // initial starting lane (can alternate between lane 1 and 2 only @level 2)


// player movements
float playerTrans[3] = { -21,-1.5,0 }; // dx for forward/backward movement, dy for up & down, dz for right & left
float jumpSpeed = 0.035;
float playerScale = 0.02;

// camera
Vector Eye(-37.4, 16.6, 0.20); // initial 3rd person camera pos 
Vector FPEye(-21 + 1.5, -1.5 + 8, 0); // initial FP camera pos
Vector At(0, 0, 0);
Vector FPAt(0, 0, 0);
Vector Up(0, 1, 0);

bool is3rdPerson = true;

// Jump-Over Barrier
vector<double> barriers; // store dx of barriers
vector<double> trainsX; // store dx of trains
vector<int> trainsZ; // store dz of trains
vector<double> keyX; //store dx for keys
vector<double> keyZ; //store dz for keys
bool barrierDrawn = false;
bool barrierTimerIntiated = false;
bool fireDrawBarrier = false;
bool trainDrawn = false;
bool trainTimerIntiated = false;
bool fireDrawtrain = false;
double barrierX = -5; // dx of player to be drawn
int trainX = 17;
int trainLane = 1;

// Half Timer Power Up
double dxHalfTimer1 = 65;
double dxHalfTimer2 = 0;



boolean isPlayerJumping = false;
boolean isPlayerJumpingDown = false;
bool isPlayerJumpingDownWithJetPack = false;
boolean isPlayerGoingLeft = false;
boolean isPlayerGoingRight = false;
boolean isPlayerGoingForward = true; // move forward until time down
float playerJetpackTime = 30; // move forward until time down

// player abilities activated
boolean isPlayerShieldActivate = false;
boolean isPlayerJetpackActivate = false;

// powerups
float keyPowerUpRespawnPos[3] = { -1.5,0.1,0 };
float jetpackPowerUpRespawnPos[3] = { -1.5,0.1,0 };
float shieldPowerUpRespawnPos[3] = { -1.5,0.1,0 };


float houseRespawnPos[3] = { 10,1.5,rightBoundary * 2 };

boolean isJetpackPowerupActivate = false;
boolean isShieldPoweupActivate = false;
boolean isKeyPowerupActivate = false;
bool spawnShield = false;
bool respawnShield = true;
bool spawnKey = false;
bool invaulnerable = false;
double sheildPowerUpTimer = 5;
float dxShield = -40;
float dzShield = -20;
double shieldRot = 0;
int keysDrawn = 0;


double dxJetPack = 0; // initial Jet pack location




// game-over related
int playerKeysCollected = 0; // if player gets three keys 
int level = 1;
bool isLost = false;
bool isWonLevel1 = false;
int timerSecsLevel1 = 60;
bool timerInitiated = false;


// sounds (flags to play song only once)
bool isLostSoundPlayed = false;
bool isWonSoundL1Played = false;
bool isWonSoundL2Played = false;
bool isBackgroundSoundL1Played = false;
bool isBackgroundSoundL2Played = false;
bool isMovementPlayed = false;
bool isPowerUpCollisionPlayed = false;


// Lights
double dxLight = 1;
bool lightRed = true;




//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	//// Define Light source 0 ambient light
	//GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	//// Define Light source 0 diffuse light
	//GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	//// Define Light source 0 Specular light
	//GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//// Finally, define light source 0 position in World Space
	//GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}
void print(int x, int y, int z, string str)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos3d(x, y, z);

	//get the length of the string to display
	len = (int)(str.length());

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}
//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);*/

	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}


void drawHouse() {
	// Draw jetpack powerup Model
	glPushMatrix();

	glTranslated(houseRespawnPos[0], houseRespawnPos[1], houseRespawnPos[2]);
	glScaled(2, 2, 2);
	glRotated(180, 0, 1, 0);
	glRotated(90, 1, 0, 0);
	model_house.Draw();
	glPopMatrix();
}

//=======================================================================
//  Ground Function
//=======================================================================
void drawGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, leftBoundary);
	glTexCoord2f(5, 0);
	glVertex3f(500, 0, leftBoundary); // x to make it look like infinite
	glTexCoord2f(5, 5);
	glVertex3f(500, 0, rightBoundary);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, rightBoundary);
	glEnd();
	glPopMatrix();


	glBindTexture(GL_TEXTURE_2D, tex_grass.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, rightBoundary);
	glTexCoord2f(5, 0);
	glVertex3f(500, 0, rightBoundary); // x to make it look like infinite
	glTexCoord2f(5, 5);
	glVertex3f(500, 0, rightBoundary + 50);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, rightBoundary + 50);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, leftBoundary);
	glTexCoord2f(5, 0);
	glVertex3f(500, 0, leftBoundary); // x to make it look like infinite
	glTexCoord2f(5, 5);
	glVertex3f(500, 0, leftBoundary - 50);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, leftBoundary - 50);
	glEnd();
	glPopMatrix();


	drawHouse();


	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void drawJetPack() {
	// Draw jetpack powerup Model
	glPushMatrix();
	glScaled(0.01, 0.01, 0.01);
	model_jetpack.Draw();
	glTranslated(100, 0, 0);
	model_jetpack.Draw();
	glPopMatrix();
}



void drawBarrier() {
	// Draw barrier Model
	glPushMatrix();
	glScalef(3, 3, 2);
	model_barrier.Draw();
	glPopMatrix();
}
void drawKey() {
	// Draw key Model
	glPushMatrix();
	glRotated(shieldRot, 0, 1, 0);
	glTranslated(-2, 0, 0.5);
	model_key.Draw();
	glColor3f(1, 1, 0.2);
	glPopMatrix();
}
void drawHalfTimer(double dx) {
	// Draw clock Model
	glPushMatrix();
	glTranslated(dx, 0, 0);
	glScaled(0.1, 0.1, 0.1);
	model_doubleTime.Draw();
	glPopMatrix();
}
void drawChest() {
	// Draw chest Model
	glPushMatrix();
	//glTranslated(playerTrans[0], playerTrans[1], playerTrans[2]);
	glRotated(90, 0, 1, 0);
	glScaled(2, 2, 2);
	model_chest.Draw();
	glPopMatrix();
}

void drawBulb()
{
	glPushMatrix();
	model_light.Draw();
	glPopMatrix();
}

void drawShield() {
	// Draw shield Model
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glScalef(7, 7, 7);
	glColor3f(1, 1, 1);
	model_shield.Draw();
	glPopMatrix();
}
void drawTrain() {
	// Draw train Model
	glPushMatrix();
	glTranslated(0, 1, -4.4);
	glRotated(-90, 0, 1, 0);
	glScaled(0.0021, 0.0021, 0.0021);
	model_train.Draw();
	glPopMatrix();

}

void drawPlayer() {
	// Draw player Model
	glPushMatrix();

	glTranslated(playerTrans[0], playerTrans[1], playerTrans[2]);
	glRotated(90, 0, 1, 0);

	if (isPlayerJetpackActivate) { // player jetpack is activated
		is3rdPerson = true;
		playerTrans[1] = getMin(upperBoundary + 6, playerTrans[1] + jumpSpeed);
		glRotated(90, 1, 0, 0);
		glPushMatrix();
		glTranslated(-0.45, 3.9, 0.8);
		drawJetPack();
		glPopMatrix();
	}
	else if (isPlayerJumping) {  // player cant jump with jetpack on 

		playerTrans[1] = getMin(upperBoundary, playerTrans[1] + jumpSpeed);
		//std::cout << " playerPos[1] : ";
		//std::cout << playerPos[1];
		if (playerTrans[1] == upperBoundary) {

			isPlayerJumping = false;
			isPlayerJumpingDown = true;
		}


	}
	else if (isPlayerJumpingDown || isPlayerJumpingDownWithJetPack) {

		playerTrans[1] = getMax(lowerBoundary, playerTrans[1] - jumpSpeed);

		if (playerTrans[1] == lowerBoundary) {
			isPlayerJumping = false;
			isPlayerJumpingDown = false;
		}
	}

	if (invaulnerable) { // on player's back 
		glPushMatrix();
		glRotated(90, 0, 1, 0);
		glTranslated(-0.1, 4, -7.4);
		glScaled(0.15, 0.15, 0.15);
		drawShield();
		glPopMatrix();
	}


	glPushMatrix();
	glScalef(playerScale, playerScale, playerScale);
	model_player.Draw();
	glPopMatrix();
	glPopMatrix();
}

void HandleCollisionWithBarrier(double dxBarrier)
{
	double barrierHeight = 3 * 0.8; // scale y * original height
	double barrierY = -1.5 + barrierHeight;// ground y + height of barrier
	double diffX = playerTrans[0] - dxBarrier;
	//std::cout << playerTrans[0] + 18.5-dxBarrier;
	if (diffX >= -2.9 && diffX <= -1.5 && playerTrans[1] <= barrierY && timerSecsLevel1 != 0) // didn't win yet
	{
		isLost = true;
	}
	else if (diffX >= -2.9 && diffX <= -1.5 && playerTrans[1] >= barrierY) // player jumped and avoided collision
	{
		barriers.erase(barriers.begin());
	}

}

void HandleCollisionWithJetPack()
{
	double JetPackHeight = 2.5;
	double JetPackY = -1.5 + JetPackHeight;// ground y + height of jetpack
	double diffX = playerTrans[0] - dxJetPack;
	if (diffX >= -2.9 && diffX <= -1.5 && playerTrans[1] <= JetPackY && lane == 1)
	{
		// after-collision logic here
		cout << "Collided with jetpack";
		isPlayerJetpackActivate = true;
		if (!isPowerUpCollisionPlayed) {
			sndPlaySound(TEXT("powerupCollision.wav"), SND_ASYNC);
			isPowerUpCollisionPlayed = true;
		}



	}
	else if (diffX >= -2.9 && diffX <= -1.5 && playerTrans[1] >= JetPackY) // player jumped and avoided collision
	{
		// do nothing
	}

}

void HandleCollisionWithTrain(double dxTrain, double dzTrain)
{
	double diffX = playerTrans[0] - dxTrain;
	if (diffX >= -25 && diffX <= 20 && playerTrans[2] == dzTrain && !invaulnerable && !isPlayerJetpackActivate)
	{
		isLost = true;
	}
	else if (diffX > 20) // player passed the train
	{
		trainsX.erase(trainsX.begin());
		trainsZ.erase(trainsZ.begin());
	}

}

void HandleCollisionWithHalfTimer()
{

	double timerHeight = 3.3; // scale y * original height
	double timerY = -1.5 + timerHeight;// ground y + height of barrier
	double diffXTimer1 = playerTrans[0] - dxHalfTimer1;
	double diffXTimer2 = playerTrans[0] - dxHalfTimer2;
	if (diffXTimer1 >= -4.8 && diffXTimer1 <= -4.6 && playerTrans[1] <= timerY)
	{
		timerSecsLevel1 /= 2;
		dxHalfTimer1 = -100;
		if (!isPowerUpCollisionPlayed) {
			sndPlaySound(TEXT("powerupCollision.wav"), SND_ASYNC);
			isPowerUpCollisionPlayed = true;
		}
	}
	else if (diffXTimer2 >= -4.8 && diffXTimer2 <= -4.6 && playerTrans[1] <= timerY)
	{
		timerSecsLevel1 /= 2;
		dxHalfTimer2 = -100;
	}




}

void HandleCollisionWithShield()
{
	double diff1 = playerTrans[0] - dxShield;
	if (diff1 <= 2 && diff1 >= -2 && !isPlayerJetpackActivate) {
		isShieldPoweupActivate = true;
		invaulnerable = true;
		if (!isPowerUpCollisionPlayed) {
			sndPlaySound(TEXT("powerupCollision.wav"), SND_ASYNC);
			isPowerUpCollisionPlayed = true;
		}
		glutTimerFunc(5000, invaulnerabilityTimer, 0);
	}
	else if (diff1 > 2) {
		respawnShield = true;
	}
}
void HandleCollisionWithKey(double dxKey, double dzKey)
{
	double diffX = playerTrans[0] - dxKey;
	if (diffX >= -2 && diffX <= 2 && playerTrans[2] == dzKey)
	{
		playerKeysCollected++;
		keyX.erase(keyX.begin());
		keyZ.erase(keyZ.begin());
		printf("collected");
		if (!isPowerUpCollisionPlayed) {
			sndPlaySound(TEXT("powerupCollision.wav"), SND_ASYNC);
			isPowerUpCollisionPlayed = true;
		}
	}
	else if (diffX > 20) // player passed the  key
	{
		keyX.erase(keyX.begin());
		keyZ.erase(keyZ.begin());
	}

}
//void OutputStringOnScreen(double x, double y, double z, bool isWon, bool isKey, bool isTimeRemaining)
void OutputStringOnScreen(double x, double y, double z, bool isWon)
{
	int len, i;

	// set the position of the text in the window using the x and y coordinates
	glRasterPos2i(x, y);

	// get the length of the string to display
	string str = "";
	if (isWon)
	{

		glColor3f(0, 1, 0);
		if (level == 1)
			str = "CONGRATS! YOU WON.... Moving to Level 2!!";
		else
			str = "GAME OVER YOU WON!! Hooray !";
	}
	//else if (!isWon && !isKey && !isTimeRemaining)
	else if (!isWon)
	{
		glColor3f(1, 0, 0);
		str = "YOU LOST";
	}

	len = (int)(str.length());

	// loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}

void timerLevel1(int value)
{
	timerSecsLevel1 = max(timerSecsLevel1--, 0);
	lightRed = lightRed ? false : true;
	glutTimerFunc(1000, timerLevel1, 0);
	glutPostRedisplay();
}

void barrierTimer(int value)
{

	// call next timer between 4000-8000 ms
	/*int timer = rand() % (8 - 4 + 1) + 4;
	timer *= 1000;*/
	fireDrawBarrier = true;
	int dxBarrier = rand() % (20 - 10 + 1) + 10;
	barrierX += dxBarrier;
	glutPostRedisplay();
	glutTimerFunc(3000, barrierTimer, 0);

}

void trainTimer(int value)
{

	// call next timer between 4000-8000 ms
	/*int timer = rand() % (8 - 4 + 1) + 4;
	timer *= 1000;*/
	fireDrawtrain = true;
	int dxTrain = rand() % (80 - 55 + 1) + 55;
	int lane = rand() % (2 - 1 + 1) + 1;
	trainLane = lane;
	trainX += dxTrain;
	int xSheild = rand() % ((trainX + 21) - (trainX - 25) + 1) + (trainX - 25);
	shieldPowerUpRespawnPos[0] = xSheild;
	if (lane == 1) {
		shieldPowerUpRespawnPos[2] = 8;
		keyPowerUpRespawnPos[2] = 8;
	}
	else {
		shieldPowerUpRespawnPos[2] = 0;
		keyPowerUpRespawnPos[2] = 0;
	}
	glutPostRedisplay();
	glutTimerFunc(5000, trainTimer, 0);

}



void shieldTimer(int value) {
	// call next timer between 5000-15000 ms
	int timer = rand() % (10 - 5 + 1) + 5;
	timer *= 1000;
	if (respawnShield)
		spawnShield = true;
	glutPostRedisplay();
	glutTimerFunc(timer, shieldTimer, 0);
}
void keyTimer(int value) {
	spawnKey = true;
	int xKey = rand() % ((trainX + 22) - (trainX - 26) + 1) + (trainX - 26);
	keyPowerUpRespawnPos[0] = xKey;
	glutPostRedisplay();
	if (keysDrawn != 3)
		glutTimerFunc(10000, keyTimer, 0);
}

void invaulnerabilityTimer(int value) {
	invaulnerable = false;
}

void transitionToLevel2Timer(int value)
{
	level = 2;
	timerSecsLevel1 = 120;
	isLost = false;

	// reset player position
	playerTrans[0] = -21;
	playerTrans[1] = -1.5;
	playerTrans[2] = 0;

	////// reset camera
	Eye.x = -37.4;
	Eye.y = 16.6;
	Eye.z = 0.20;

	At.x = 0;
	At.y = 0;
	At.z = 0;

	/*FPEye.x = playerTrans[0] + 3;
	FPEye.y = playerTrans[1] + 8;
	FPEye.z = playerTrans[2];*/

	//-21+1.5, -1.5+8, playerTrans[2]

	FPEye.x = -21 + 1.5;
	FPEye.y = -1.5 + 8;
	FPEye.z = -0;

	FPAt.x = 0;
	FPAt.y = 0;
	FPAt.z = 0;

	//
	groundX = 0;
	//isGroundTimerInitiated = false;
	//reset light
	dxLight = 1;

	//is3rdPerson = true;
};



void transitionToLevel3Timer()
{
	level = 3;
	timerSecsLevel1 = 120;
	isLost = false;
	isWonSoundL1Played = true;
	// reset player position
	playerTrans[0] = -21;
	playerTrans[1] = -1.5;
	playerTrans[2] = 0;

	////// reset camera
	Eye.x = -37.4;
	Eye.y = 16.6;
	Eye.z = 0.20;

	At.x = 0;
	At.y = 0;
	At.z = 0;

	/*FPEye.x = playerTrans[0] + 3;
	FPEye.y = playerTrans[1] + 8;
	FPEye.z = playerTrans[2];*/

	//-21+1.5, -1.5+8, playerTrans[2]

	FPEye.x = -21 + 1.5;
	FPEye.y = -1.5 + 8;
	FPEye.z = -0;

	FPAt.x = 0;
	FPAt.y = 0;
	FPAt.z = 0;

	//
	groundX = 0;
	//isGroundTimerInitiated = false;
	//reset light
	dxLight = 1;

	//is3rdPerson = true;
};


void setupLights()
{
	//GLfloat l0Diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//GLfloat l0Spec[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//GLfloat l0Ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	//GLfloat l0Position[] = { 0.0f, 5.0f ,-5.0f, true };
	//GLfloat l0Direction[] = { 0.0f, 0.0f, -7.0f};
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	//glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
	////glLightfv(GL_LIGHT0, GL_ATTENT)
	////glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 90.0);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);

	/*GLfloat l1Diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat l1Spec[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l1Ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat l1Position[] = { -5.0f, 5.0f ,-19.0f, true };
	GLfloat l1Direction[] = {0.0f, 0.0f, -17.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Direction);*/


	/*GLfloat l1Diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat l1Spec[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l1Ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat l1Position[] = { 0.0f, 0.0f ,0.0f, true };
	GLfloat l1Direction[] = { 0.0f, 0.0f, -7.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);*/


	//GLfloat l1Diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat l1Diffuse[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	if (lightRed)
	{
		l1Diffuse[0] = 1;
		l1Diffuse[2] = 0;
		//lightRed = false;
	}
	else
	{
		l1Diffuse[0] = 0;
		l1Diffuse[2] = 1;
		//lightRed = true;
	}
	GLfloat l1Spec[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l1Ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat l1Position[] = { -5.0f, 0.0f ,0.0f, true };
	GLfloat l1Direction[] = { 0.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);

}
void wonScene() {
	drawGround();
	drawChest();
	drawPlayer();
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int posy = 5;
	if (is3rdPerson) {
		posy = 15;
	}
	

	//if (!isLost) {
	//	//OutputStringOnScreen(playerTrans[0] + 15, 15,  - 15, false, false, true);
	//	glColor3f(0, 0, 0);
	//	if (level == 1)
	//		print(playerTrans[0] + 15, posy, -15, "Time Remaining:" + std::to_string(timerSecsLevel1));
	//	if (level == 2)
	//		print(playerTrans[0] + 15, posy, 15, "Keys Collected:" + std::to_string(playerKeysCollected));

	//}
	//	OutputStringOnScreen(playerTrans[0] +15, 25,  20, false, true, false);
	//}



	if (isLost || (timerSecsLevel1 == 0 && level == 2)) // collided either in level 1 or 2
	{
		OutputStringOnScreen(playerTrans[0], playerTrans[1] + 10, playerTrans[2], false);
		if (!isLostSoundPlayed)
		{
			cout << "You lost";
			isLostSoundPlayed = true;
			sndPlaySound(TEXT("gameOver.wav"), SND_ASYNC);
		}

	}
	else if (timerSecsLevel1 == 0 && level == 1) // time's up in level 1 without collision -> WON
	{
		OutputStringOnScreen(playerTrans[0], playerTrans[1] + 10, playerTrans[2], true);
		if (!isWonSoundL1Played)
		{
			sndPlaySound(TEXT("gameWin.wav"), SND_ASYNC);
			isWonSoundL1Played = true;
		}
		glutTimerFunc(3000, transitionToLevel2Timer, 0);

	}
	else if (playerKeysCollected == 3 && level == 2) { // won in level 2

		//OutputStringOnScreen(playerTrans[0], playerTrans[1] + 10, playerTrans[2], true);
		if (!isWonSoundL2Played)
		{
			cout << "You Won level 2!!";
			isWonSoundL2Played = true;
			sndPlaySound(TEXT("gameWin.wav"), SND_ASYNC);
		}
		transitionToLevel3Timer();
		level = 3;
	}
	else // game should run normally
	{	
		if (level != 3)
		{
			glPushMatrix();
			glColor3f(0, 0, 0);
			print(playerTrans[0] + 15, posy, -15 + (lane == 1 ? 0 : 8), "Time Remaining:" + std::to_string(timerSecsLevel1));
			glPopMatrix();
		}
		
		

		if (level == 2)
		{
			print(playerTrans[0] + 15, posy, 15, "Keys Collected:" + std::to_string(playerKeysCollected));
		}
		glColor3f(1, 1, 1);


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (is3rdPerson)
			gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
		else
			gluLookAt(FPEye.x, FPEye.y, FPEye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);

		glPushMatrix();
		glTranslated(dxLight, 0, 0);
		setupLights();
		glPopMatrix();

		// left light bulb
		glPushMatrix();
		glTranslated(dxLight, 0, 0);
		glTranslated(0, 0, -20);
		glRotated(-90, 0, 1, 0);
		glScaled(2, 2, 2);
		drawBulb();
		glPopMatrix();

		// right light bulb
		glPushMatrix();
		glTranslated(dxLight, 0, 0);
		glTranslated(0, 0, 20);
		glRotated(90, 0, 1, 0);
		glScaled(2, 2, 2);
		drawBulb();
		glPopMatrix();

		if (level == 1)
		{
			if (!isBackgroundSoundL1Played)
			{
				sndPlaySound(TEXT("theme.wav"), SND_ASYNC | SND_LOOP);
				isBackgroundSoundL1Played = true;
			}
			if (!timerInitiated)
			{
				glutTimerFunc(1000, timerLevel1, 0);
				timerInitiated = true;
			}

			if (!isGroundTimerInitiated)
			{
				// Simulation Infinite Ground
				glutTimerFunc(5000, drawGroundTimer, 0);
				isGroundTimerInitiated = true;
			}

			glPushMatrix();
			glTranslated(groundX, 0, 0);
			drawGround();
			glPopMatrix();


			glPushMatrix();
			drawPlayer();
			glPopMatrix();

			if (dxHalfTimer1 != -100) // signifies no collision
			{
				glPushMatrix();
				drawHalfTimer(dxHalfTimer1);
				glPopMatrix();
			}

			if (dxHalfTimer2 != -100)
			{
				glPushMatrix();
				drawHalfTimer(dxHalfTimer2);
				glPopMatrix();
			}


			// Barrier
			if (!barrierDrawn) // initial drawings
			{
				glPushMatrix();
				//for (double dx = -5; dx <= 25;dx += 10)
				for (; barrierX <= 25; barrierX += 10)
				{
					glPushMatrix();
					glTranslated(barrierX, 0, 0);
					drawBarrier();
					glPopMatrix();
					barriers.push_back(barrierX);
				}
				barrierDrawn = true;
				glPopMatrix();
			}
			else // drawn already - checking collisions & draw new ones
			{
				if (fireDrawBarrier)
				{
					//glPushMatrix();
					//glTranslated(barrierX, 0, 0);
					//drawBarrier();
					//glPopMatrix();
					barriers.push_back(barrierX);

					fireDrawBarrier = false;
				}
				glPushMatrix();
				for (int i = 0; i < barriers.size(); i++)
				{
					double dxBarrier = barriers[i];
					glPushMatrix();
					glTranslated(dxBarrier, 0, 0);
					drawBarrier();
					glPopMatrix();


					HandleCollisionWithBarrier(dxBarrier);
				}
				glPopMatrix();

				if (!barrierTimerIntiated)
				{
					glutTimerFunc(5000, barrierTimer, 0);
					barrierTimerIntiated = true;
				}
			}

		}
		else if (level == 2)
		{// level 2 

			if (!isBackgroundSoundL2Played)
			{
				sndPlaySound(TEXT("theme.wav"), SND_ASYNC | SND_LOOP);
				isBackgroundSoundL2Played = true;
			}

			if (!isGroundTimerInitiated)
			{
				// Simulation Infinite Ground
				glutTimerFunc(10000, drawGroundTimer, 0);
				isGroundTimerInitiated = true;
			}
			if (fireDrawtrain)
			{
				//glPushMatrix();
				//glTranslated(barrierX, 0, 0);
				//drawBarrier();
				//glPopMatrix();
				trainsX.push_back(trainX);
				trainsZ.push_back(trainLane);
				fireDrawtrain = false;
			}
			glPushMatrix();
			for (int i = 0; i < trainsX.size(); i++)
			{
				double dxTrain = trainsX[i];
				double dzTrain = 0;
				if (trainsZ[i] == 2)
					dzTrain = 8;
				glPushMatrix();
				glTranslated(dxTrain, 0, dzTrain);
				drawTrain();
				glPopMatrix();

				HandleCollisionWithTrain(dxTrain, dzTrain);
			}
			glPopMatrix();

			if (!trainTimerIntiated)
			{
				glutTimerFunc(3000, trainTimer, 0);
				glutTimerFunc(3000, shieldTimer, 0);
				glutTimerFunc(5000, keyTimer, 0);
				trainTimerIntiated = true;
			}


			glPushMatrix();
			for (int i = 0; i < keyX.size(); i++)
			{
				double dxKey = keyX[i];
				double dzKey = keyZ[i];
				double dyKey = 0;
				if (isPlayerJetpackActivate)
					dyKey = upperBoundary + 6;
				glPushMatrix();
				glTranslated(dxKey, dyKey, dzKey);
				drawKey();
				glPopMatrix();

				HandleCollisionWithKey(dxKey, dzKey);
			}
			glPopMatrix();
			glPushMatrix();
			glTranslated(groundX, 0, 0);
			drawGround();
			glPopMatrix();
			if (keysDrawn != 3 && spawnKey) {
				keysDrawn++;
				spawnKey = false;
				keyX.push_back(keyPowerUpRespawnPos[0]);
				keyZ.push_back(keyPowerUpRespawnPos[2]);
			}


			glPushMatrix();
			glTranslated(dxJetPack, 0, 0); // set dx to change JetPack location (only 1 per level 2)
			drawJetPack();
			glPopMatrix();


			glPushMatrix();
			drawPlayer();
			glPopMatrix();

			if (spawnShield) {
				dxShield = shieldPowerUpRespawnPos[0];
				dzShield = shieldPowerUpRespawnPos[2];
				spawnShield = false;
				respawnShield = false;
			}
			if (!respawnShield || !invaulnerable)
			{
				glPushMatrix();
				glTranslated(dxShield, 0, dzShield);
				glRotated(shieldRot, 0, 1, 0);
				glTranslated(0, 3.5, -7.2);
				glScaled(0.15, 0.15, 0.15);
				drawShield();
				glPopMatrix();
			}

		}
		else if (level == 3) {
			is3rdPerson = false;
			print(playerTrans[0] + 15, posy, 0, "Hooray you won");


			wonScene();
		}

		
	}




	//sky box
	if (!isLost) {
		glPushMatrix();
		glTranslated(playerTrans[0], playerTrans[1], playerTrans[2]);

		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 70, 70, 70);
		gluDeleteQuadric(qobj);
		glPopMatrix();
	}


	glutSwapBuffers();
}

void Animate() {

	if (isLost) // collided either in level 1 or 2
	{

	}
	else if (timerSecsLevel1 == 0 && level == 1) // time's up in level 1 without collision -> WON
	{

	}
	else if (playerKeysCollected == 3 && level == 3) // won in level 2
	{
		if (playerTrans[0] <= -7) {
			Eye.x += 0.01;
			FPEye.x += 0.01;
			At.x = 0;
			FPAt.x = 0;
			dxLight += 0.01;
			playerTrans[0] += 0.01;
			if (!isWonSoundL1Played)
			{
				sndPlaySound(TEXT("gameWin.wav"), SND_ASYNC);
				isWonSoundL1Played = true;
			}
		}
		else {
			exit(0);
		}

	}
	else // game running normally
	{
		Eye.x += 0.03;
		FPEye.x += 0.03;
		At.x += 0.03;
		FPAt.x += 0.03;
		dxLight += 0.03;
		if (isPlayerJumping && !isPlayerJetpackActivate)
		{
			FPEye.y += jumpSpeed;
			FPAt.y += jumpSpeed;
		}
		else if (isPlayerJumpingDown && !isPlayerJetpackActivate)
		{
			FPEye.y -= jumpSpeed;
			FPAt.y -= jumpSpeed;
		}
		if (level == 1)
		{
			playerTrans[0] += 0.03; // dx Player movement speed on the field	
			HandleCollisionWithHalfTimer();
		}
		else
		{
			Eye.x += 0.03;
			FPEye.x += 0.03;
			At.x += 0.03;
			FPAt.x += 0.03;
			dxLight += 0.03;

			// level 2 animation here
			shieldRot += 0.3;
			playerTrans[0] += 0.06;
			HandleCollisionWithShield();
			HandleCollisionWithJetPack();
		}
	}





	glutPostRedisplay();
}


void KeyboardMovements(unsigned char button, int x, int y)
{
	//switch (button)
	//{

	//case 'r':
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	break;
	//case 27:
	//	exit(0);
	//	break;

	// Camera Movements
	if (button == 'd')
		Eye.x += 0.2;
	if (button == 'a')
		Eye.x -= 0.2;
	//y axis
	if (button == 'w')
		Eye.y += 0.2;
	if (button == 's')
		Eye.y -= 0.2;
	// z axis
	if (button == 'l')
		Eye.z += 0.2;
	if (button == 'k')
		Eye.z -= 0.2;
	//camera
	if (button == 'f' && !isPlayerJetpackActivate) { // first person
		is3rdPerson = false;
	}
	if (button == 'g') // third person
		is3rdPerson = true;


	// Jumping
	if (button == ' ' && !isPlayerJetpackActivate && !isPlayerJumping && !isPlayerJumpingDown) // if jetpack not activated you can jump, and not jumping up or going down from a jump
	{
		isPlayerJumping = true;

		sndPlaySound(TEXT("movement.wav"), SND_ASYNC);
		isMovementPlayed = true;

	}


	// for testing only
	if (button == '1')
	{
		// top camera view
		Eye.x = -24;
		Eye.y = 54;
		Eye.z = -0.2;
	}
	glutPostRedisplay();
}

void SpecialKeyboardMovements(int keyPressed, int x, int y)
{
	if (level == 2)
	{
		if (keyPressed == GLUT_KEY_RIGHT && lane == 1)
		{
			playerTrans[2] += 8;
			Eye.z += 8;
			FPEye.z += 8;
			At.z += 8;
			lane = 2;
			sndPlaySound(TEXT("movement.wav"), SND_ASYNC);
			isMovementPlayed = true;

		}
		else if (keyPressed == GLUT_KEY_LEFT && lane == 2)
		{
			playerTrans[2] -= 8;
			Eye.z -= 8;
			FPEye.z -= 8;
			At.z -= 8;
			lane = 1;

			sndPlaySound(TEXT("movement.wav"), SND_ASYNC);
			isMovementPlayed = true;



		}
		else if (keyPressed == GLUT_KEY_DOWN)
		{
			// ???
		}

	}


	// for testing only
	/*if (keyPressed == GLUT_KEY_RIGHT)
	{
		playerTrans[2] += 8;
		isPlayerGoingRight = true;
	}
	else if (keyPressed == GLUT_KEY_LEFT)
	{
		playerTrans[2] -= 8;
		isPlayerGoingLeft = true;
	}
	if (keyPressed == GLUT_KEY_UP)
		playerTrans[0] += 0.3;
	else if (keyPressed == GLUT_KEY_DOWN)
		playerTrans[0] -= 0.3;*/

	glutPostRedisplay();
}




void Timer(int value) {

	if (isPlayerJetpackActivate && playerJetpackTime < 0) {
		isPlayerJetpackActivate = false;
		isPlayerJumpingDownWithJetPack = true;
		playerJetpackTime = 30;
	}
	if (isPlayerJetpackActivate) {
		playerJetpackTime -= 1;
	}
	glutTimerFunc(500, Timer, 0);

	glutPostRedisplay();
}


//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	/*if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}*/

	x = WIDTH - x;



	//if (x > HEIGHT / 2 && isPlayerJetpackActivate) // go left
	//	//playerTrans[2] = getMax(leftBoundary, playerTrans[2] - 0.25);
	//	playerTrans[2] = getMax(leftBoundary, playerTrans[2] - 8);
	//else if (x < HEIGHT / 2 && isPlayerJetpackActivate) // go right
	//	//playerTrans[2] = getMin(rightBoundary, playerTrans[2] + 0.25);
	//	playerTrans[2] = getMin(rightBoundary, playerTrans[2] + 8);

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix


	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	/*GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	//y = HEIGHT - y;
	//x = WIDTH - x;

	//if (x > HEIGHT / 2 && isPlayerJetpackActivate) { // go left

	//	//playerTrans[2] = getMax(leftBoundary, playerTrans[2] - 0.15);
	//	playerTrans[2] = getMax(leftBoundary, playerTrans[2] - 8);

	//}
	//else if (x < HEIGHT / 2 && isPlayerJetpackActivate) { // go right
	//	//playerTrans[2] = getMin(rightBoundary, playerTrans[2] + 0.15);
	//	playerTrans[2] = getMin(rightBoundary, playerTrans[2] + 8);

	//}

	if (level == 2)
	{
		if (x > HEIGHT / 2 && isPlayerJetpackActivate && lane == 1)
		{
			playerTrans[2] += 8;
			Eye.z += 8;
			FPEye.z += 8;
			At.z += 8;
			lane = 2;
		}
		else if (x < HEIGHT / 2 && isPlayerJetpackActivate && lane == 2)
		{
			playerTrans[2] -= 8;
			Eye.z -= 8;
			FPEye.z -= 8;
			At.z -= 8;
			lane = 1;
		}

	}

}



//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading ground
	model_ground.Load("Models/rails/track.3ds");

	// player
	model_player.Load("Models/boy/Character.3ds");

	//barrier
	model_barrier.Load("Models/barrier/cmt_barrier.3ds");

	model_key.Load("Models/key/sk3.3ds");

	//double time
	model_doubleTime.Load("Models/doubleTime/clock.3ds");

	// model train
	model_train.Load("Models/train/AVEENG_L.3ds");

	//shield
	model_shield.Load("Models/shield/shiled.3ds");

	//jetpack
	model_jetpack.Load("Models/rocket/rocket.3ds");

	//light
	model_light.Load("Models/light/lamp.3ds");

	// house
	model_house.Load("Models/house/house.3ds");

	// chest
	model_chest.Load("Models/chest/chest.3ds");



	// Loading texture files
	tex_ground.Load("Textures/rails.bmp");
	tex_grass.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}


void drawGroundTimer(int dx)
{
	//groundX += 50;
	if (level == 1)
	{
		groundX += 20;
	}
	else //level 2
	{
		groundX += 5;
	}

	houseRespawnPos[0] = playerTrans[0] + 20;
	houseRespawnPos[1] = 1.5;
	int lane = rand() % (2 - 1 + 1) + 1;
	if (lane == 2) {
		houseRespawnPos[2] = rightBoundary * 2;
	}
	else {
		houseRespawnPos[2] = leftBoundary - 10;
	}

	if (level == 2 && isBackgroundSoundL2Played && isPowerUpCollisionPlayed) {
		isBackgroundSoundL2Played = false;
		isPowerUpCollisionPlayed = false;

	}	if (level == 2 && isBackgroundSoundL2Played && isMovementPlayed) {
		isBackgroundSoundL2Played = false;
		isMovementPlayed = false;

	}
	if (level == 1 && isBackgroundSoundL1Played && isPowerUpCollisionPlayed) {
		isBackgroundSoundL1Played = false;
		isPowerUpCollisionPlayed = false;
	}if (level == 1 && isBackgroundSoundL1Played && isMovementPlayed) {
		isBackgroundSoundL1Played = false;
		isMovementPlayed = false;
	}

	glutTimerFunc(5000, drawGroundTimer, 0);

}



void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);



	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(KeyboardMovements);
	glutSpecialFunc(SpecialKeyboardMovements);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);
	glutIdleFunc(Animate);
	glutTimerFunc(0, Timer, 0);
	glutReshapeFunc(myReshape);




	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	/*glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);*/
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}