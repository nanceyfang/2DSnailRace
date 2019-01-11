#include <cmath>
#include <ctime>
#include <random>
#include <functional>
#include "opponent.h"
#include "gamedata.h"
#include "renderContext.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

Opponent::Opponent(const std::string& name, const Vector2f pos, const int w,
                   const int h, float velX) :
  Player(name),              
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  velocityX(velX),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance"))
{
  srand(rand());
  if (rand()%2)
    velocityX += rand()%80;
  else
    velocityX-=rand()%8;
}


void Opponent::update(Uint32 ticks){

  Player::update(ticks);

  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) {
      currentMode = FAST;
    }
    else {
      velocityX = Gamedata::getInstance().getXmlInt("Opponent/speedX");
      setVelocityX(velocityX);
    }
  }
  else if  ( currentMode == FAST ) {
    if(distanceToEnemy > safeDistance){
      currentMode=NORMAL;
    }
    else {
      if (getPlayer()->getVelocityX() > 0)
        setVelocityX( Gamedata::getInstance().getXmlInt("Opponent/speedX") + 
        Gamedata::getInstance().getXmlInt("Opponent/speedUp"));
      else
        setVelocityX( - (Gamedata::getInstance().getXmlInt("Opponent/speedX")) - 
        Gamedata::getInstance().getXmlInt("Opponent/speedUp"));
    }
  }

  if ( distanceToEnemy < safeDistance){
    shoot();
  }
  
  int xPos = getX();
  int yPos = getY();
  //pathing
  if ( xPos < 1350 && yPos > 400 )
    right();
  if ( xPos >= 1350 && xPos < 1500 && yPos > 450){
    jump(); 
    right();  
  }
  if ( yPos < 450 && xPos >= 1450){
    left();
    jump();
  }
  if ( yPos < 350 && xPos > 550){
    left();
  }
  if ( xPos < 730 && xPos > 500 && yPos < 350 ){
    jump();
    left();
  }
  if ( xPos < 570 && xPos > 560 && yPos < 300) {
    jump();
    left();
  }
  if (xPos < 550 && yPos < 300 && xPos > 300) {
    left();
    jump();
  }
  if (xPos < 301 && yPos < 300)
    left();
}
