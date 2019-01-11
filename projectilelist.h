#ifndef PROJECTILELIST__H
#define PROJECTILELIST__H
#include <list>
#include "projectile.h"

class ProjectileList {
public: 
  ProjectileList(const std::string&);
  ~ProjectileList();
  void update(Uint32 ticks);
  void add();
  void draw() const;
  
private:
  std::list<Projectile*> projectiles;
  std::list<Projectile*> freeList;
};

#endif
