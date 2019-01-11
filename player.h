#ifndef PLAYER__H
#define PLAYER__H
#include <SDL.h>
#include <string>
#include <vector>
#include <cmath>
#include <list>

#include "playermultisprite.h"
#include "projectile.h"
#include "projectilelist.h"

class Player {
public:

  Player(const std::string&);
  Player(const Player&);

  virtual ~Player();
  

  void update(Uint32 ticks);
  const PlayerMultiSprite* getPlayer() const { return &player; }

  const std::string& getName() const { return player.getName(); }
  int getX() const { return player.getX(); }
  int getY() const { return player.getY(); }
  const Image* getImage() const { 
    return player.getImage();
  }
  int getScaledWidth()  const { 
    return player.getScaledWidth();
  } 
  int getScaledHeight()  const { 
    return player.getScaledHeight();
  } 
  const SDL_Surface* getSurface() const { 
    return player.getSurface();
  }
  
  void setVelocityX(int x){
    player.setVelocityX(x);
  }
  
  void setVelocityY(int y){
    player.setVelocityY(y);
  }
  
  Vector2f getVelocity() const{
    return player.getVelocity();
  }
  
  bool getJumpState(){
    return isJumping;
  }

  void right();
  void left();
  void up(float);
  void down();
  void jump();
  void stop();
  void draw() const;
  
  void attach (Player * o ) {observers.push_back(o); }
  void detach (Player * o);
  
  void setJumpState(bool b) {
    isJumping = b;
    player.setIsJumping(b);
  }

  Player& operator=(const Player&) = delete;
  
  void explode(){
    player.explode();
  }

  void shoot();
  int projSize() {return projectiles.size(); }
  int freeSize() {return freeList.size(); }
  
  std::list<Projectile*> getProjectiles() {
    return projectiles;
  }
  
  void endGame();
  
private:
  PlayerMultiSprite player;
  Vector2f initialVelocity;
  int worldWidth;
  int worldHeight;
  std::list<Player*> observers;
  bool isJumping;

  std::list<Projectile*> projectiles;
  std::list<Projectile*> freeList;
  float minSpeed;
  float projInterval;
  float timeSinceLastFrame;
  bool isEnd;
};
#endif
