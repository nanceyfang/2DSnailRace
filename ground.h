#ifndef GROUND__H
#define GROUND__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "image.h"
#include "sprite.h"

class Ground{

  public:
  Ground(int x, int y, const Image* img) : sprite("Ground"), xPos(x), 
    yPos(y), image (img) {
      sprite.setX(xPos);
      sprite.setY(yPos);
    }

  Ground(const Ground& g) : sprite("Ground"), xPos(g.xPos), yPos(g.yPos), 
  image (g.image) {}

  void draw() const;

  int getX() const {return xPos;}
  int getY() const {return yPos;}
  void setX(int x) {xPos+=x;}

  const Sprite* getSprite() const { return &sprite; }

  Ground& operator=(const Ground&) = delete;

  private:
  
  Sprite sprite;
  
  int xPos;
  int yPos;
  
  const Image * image;
  
};

#endif
