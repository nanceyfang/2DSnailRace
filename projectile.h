#ifndef PROJECTILE__H
#define PROJECTILE__H

#include "multisprite.h"


class ExplodingSprite;

class Projectile : public MultiSprite {
public: 
  explicit Projectile(const std::string&);
  Projectile(const Projectile&);
  ~Projectile();
  Projectile& operator=(const Projectile&);
  
  void update(Uint32 ticks);
  bool goneTooFar() const {return tooFar;}
  void reset(){
  tooFar = false;
  distance = 0;
  }
  void explode();
  
private:

  float distance;
  float maxDistance;
  bool tooFar;
  std::vector<Image *> images;
  ExplodingSprite* explosion;
  

};

#endif
