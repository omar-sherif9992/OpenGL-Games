// opengl chicken invaders game
//
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <random>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <unistd.h> //required for usleep()
#include "hero.h"
#include "enemy.h"

#include "shapes.h"
#include <queue> // std::queue

using namespace std;
using std::string;
//  Global Variables
Shapes shapes = Shapes(0, 0);

// WINDOW SETTINGS
double windowWidth;
double windowHeight;
double windowPosX = 0;
double windowPosY = 0;

int windowHandle = 0;
int frameCount = 0;
int frameRate = 0;
int lastFrameTime = 0;

struct Bullet
{
  mutable double posX;
  mutable double posY;

  double step;
  Bullet(double posX, double posY, double step)
  {
    this->posX = posX;
    this->posY = posY;

    this->step = step;
  }
  void draw()
  {
    if (step > 0)
    {
      shapes.drawBullet(this->posX, this->posY);
    }
    else
    {
      shapes.drawDownBullet(this->posX, this->posY);
    }
  }
};

struct Powerup
{
  mutable double posX;
  mutable double posY;
  double radius;
  Powerup()
  {
    this->posX = (((rand() % 8) * ((int)windowWidth / 9)) + (int)windowWidth / 11) % (int)windowWidth;
    this->posY = ((rand() % 5) * (windowHeight / 9)) + (int)windowHeight / 10;
    this->radius = (int)windowWidth / 42;
  }
  void draw()
  {
    // white cover circle
    glColor3f(1.0, 1.0, 1.0);
    shapes.drawCircle(this->posX, this->posY, this->radius + 2);
    // blue circle
    glColor3f(0.0, 0.0, 1.0);
    shapes.drawCircle(this->posX, this->posY, this->radius);

    // white shield
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(this->posX + this->radius / 2, this->posY + 10);
    glVertex2f(this->posX + this->radius / 2, this->posY - 10);
    glVertex2f(this->posX - this->radius / 2, this->posY - 10);
    glEnd();
  }
};

//  Global Variables
Hero hero = Hero(0, 0);
Enemy enemy = Enemy(0, 0);
queue<Bullet> enemyBullets;
queue<Bullet> heroBullets;
queue<Powerup> powerUps;

int heroShotDuration = 0;

//-----------------

//  Methods Signatures
void Anim();
void cube(void);
void Key(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();
//-----------------

int main(int argc, char **argr)
{
  glutInit(&argc, argr);

  // Window settings
  windowWidth = (glutGet(GLUT_SCREEN_WIDTH));
  windowHeight = (glutGet(GLUT_SCREEN_HEIGHT));

  // define the Global Variables
  hero = Hero(windowWidth, windowHeight);
  enemy = Enemy(windowWidth, windowHeight);
  shapes = Shapes(windowWidth, windowHeight);

  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(windowPosX, windowPosY);
  glutCreateWindow("Space Invaders");
  glutDisplayFunc(Display);
  glutIdleFunc(Anim);
  glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glClearColor(29.0 / 255, 41.0 / 255, 70.0 / 255, 0.0);
  gluOrtho2D(0, windowWidth, 0, windowHeight);

  glutKeyboardFunc(Key); // sets the Keyboard handler function; called when a key is pressed

  glutMainLoop();
  return 0;
}

void Display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  hero.draw();
  enemy.draw();
  if (!heroBullets.empty())
  {
    for (int i = 0; i < heroBullets.size(); i++)
    {
      Bullet bullet = heroBullets.front();
      heroBullets.pop();
      bullet.draw();
      heroBullets.push(bullet);
    }
  }
  if (!enemyBullets.empty())
  {
    for (int i = 0; i < enemyBullets.size(); i++)
    {
      Bullet enemyBullet = enemyBullets.front();
      enemyBullet.draw();
      enemyBullets.pop();
      enemyBullets.push(enemyBullet);
    }
  }

  if (!powerUps.empty())
  {
    Powerup powerUp = powerUps.front();
    powerUp.draw();
    bool flagDown = hero.getPosY() + hero.getHeight() >= (powerUp.posY - 90) && (hero.getPosY() + hero.getHeight()) <= powerUp.posY + powerUp.radius * 2;
    bool flagUp = hero.getPosY() <= powerUp.posY + powerUp.radius * 2 && hero.getPosY() >= powerUp.posY;
    bool flagRight = hero.getPosX() + hero.getWidth() >= (powerUp.posX - 25) && hero.getPosX() + hero.getWidth() <= powerUp.posX + powerUp.radius * 2;
    bool flagLeft = (hero.getPosX() - 45) <= powerUp.posX + powerUp.radius * 2 && hero.getPosX() >= (powerUp.posX);

    if (flagDown && flagRight || flagDown && flagLeft || flagUp && flagRight || flagUp && flagLeft)
    {
      hero.setShieldsUp(true);

      powerUps.pop();
    }
  }

  glFlush();
}

int enemyBulletDuration = 0;
void Anim()
{
  if (enemy.isAlive() && hero.getLifes() > 0)
  {
    // count down for hero bullet shooting
    heroShotDuration++;
    //  move the enemy
    enemy.move();
    enemyBulletDuration += 1;

    if (enemyBulletDuration == 500)
    {
      enemyBullets.push(Bullet(enemy.getPosX() + enemy.getWidth() / 2, enemy.getPosY(), -1));
      enemyBulletDuration = 0;
    }

    if (!enemyBullets.empty())
    {
      for (int i = 0; i < enemyBullets.size(); i++)
      {
        Bullet enemyBullet = enemyBullets.front();
        enemyBullets.pop();
        if (enemyBullet.posY > 0 &&
            enemyBullet.posY < windowHeight &&
            enemyBullet.posX > 0 &&
            enemyBullet.posX <= hero.getPosX() + hero.getWidth() &&
            enemyBullet.posY <= hero.getPosY() + hero.getHeight() &&
            enemyBullet.posY >= hero.getPosY() &&
            enemyBullet.posX >= (hero.getPosX() - hero.getWidth()) && !hero.isShieldsUp())
        {
          hero.getHit();
        }
        else if (enemyBullet.posY >= 0)
        {
          enemyBullets.push(Bullet(enemyBullet.posX, enemyBullet.posY + enemyBullet.step, enemyBullet.step));
        }
      }
    }
  }
  if (!heroBullets.empty())
  {
    for (int i = 0; i < heroBullets.size(); i++)
    {
      Bullet heroBullet = heroBullets.front();
      heroBullets.pop();
      if (heroBullet.posY > 0 &&
          heroBullet.posY < windowHeight &&
          heroBullet.posX > 0 &&
          heroBullet.posX <= enemy.getPosX() + enemy.getWidth() &&
          heroBullet.posY <= enemy.getPosY() + enemy.getHeight() &&
          heroBullet.posY >= enemy.getPosY() &&
          heroBullet.posX >= (enemy.getPosX()))
      {
        enemy.getHit();
      }
      else if (heroBullet.posY <= windowHeight)
      {
        heroBullets.push(Bullet(heroBullet.posX, heroBullet.posY + heroBullet.step, heroBullet.step));
      }
    }
  }

  // Bullet enemieBullet= Bullet(enemy.getPosX(),enemy.getPosY(),-1);
  glutPostRedisplay();
}

/// @brief  Handles the keyboard input (when a key is pressed) and calls the appropriate movement function
void Key(unsigned char key, int x, int y)
{
  if ((key == 'e' || key == 'E') && (hero.getLifes() <= 0 || !enemy.isAlive()))
  {
    exit(0);
  }
  else if((key == 'r' || key == 'R') && (hero.getLifes() <= 0 || !enemy.isAlive())){
    hero=Hero(windowWidth, windowHeight);
    enemy=Enemy(windowWidth, windowHeight);
  }

  else
  {
    switch (key)
    {
    case 'a':
      // if the key 'a' is pressed, hero moves left
      hero.moveLeft();
      break;
    case 'w':
      // if the key 'w' is pressed, hero moves up
      hero.moveUp();
      break;
    case 's':
      // if the key 's' is pressed, hero moves down
      hero.moveDown();
      break;
    case 'd':
      // if the key 'd' is pressed, hero moves down
      hero.moveRight();
      break;


    case ' ':
      // if the key 'space' is pressed, hero shoots
      if (heroBullets.size() < 4 && heroShotDuration > 70)
      {
        heroShotDuration = 0;
        heroBullets.push(Bullet(hero.getPosX(), hero.getPosY() + hero.getHeight() / 2, 1));
      }
      break;
    }
  }

  // ask OpenGL to recall the display function to reflect the changes on the window
  glutPostRedisplay();
}
int i = 0;

void Timer(int value)
{
  if (powerUps.size() < 1 && hero.isShieldsUp() == false)
  {
    powerUps.push(Powerup());
    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
    // recall the Timer function after 20 seconds (20,000 milliseconds)
    glutTimerFunc(5 * 1000, Timer, 0);
  }
  else if (hero.isShieldsUp())
  {
    if (i == 0)
    {
      i++;
      glutTimerFunc(hero.getShieldTime() * 1000, Timer, 0);
    }
    else
    {

      hero.setShieldsUp(false);
      glutPostRedisplay();
      i = 0;
      glutTimerFunc(1 * 1000, Timer, 0);
    }
  }
  else
  {
    glutTimerFunc(1000, Timer, 0);
  }
}