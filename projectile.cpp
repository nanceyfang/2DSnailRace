#include <iostream>
#include <cmath>
#include "projectile.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "explodingSprite.h"

Projectile::Projectile(const std::string& name) : 
  MultiSprite(name), distance(0), 
  maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
  tooFar(false),
  images( ImageFactory::getInstance()->getImages(name) ),
  explosion(nullptr)
 {}
 
Projectile::~Projectile() {

}
 
Projectile::Projectile(const Projectile& p) : 
  MultiSprite(p), 
  distance(p.distance), 
  maxDistance(p.distance),
  tooFar(p.tooFar),
  images( p.images ),
  explosion( p.explosion )
 {}
 
Projectile& Projectile::operator=(const Projectile& p) {
  if(this == &p) return *this;
//  MultiSprite = p;
  distance = p.distance; 
  maxDistance = p.distance;
  tooFar = p.tooFar;
  images = p.images;
  explosion = p.explosion;
  return *this;
}

void Projectile::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  MultiSprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
  else tooFar = false;
}
