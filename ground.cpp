#include "ground.h"
#include "gamedata.h"
#include "imageFactory.h"



void Ground::draw() const { 
  image[0].draw(getX(), getY(), 1.0);
}
