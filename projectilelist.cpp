#include "projectilelist.h"
#include <list>

ProjectileList::ProjectileList(const std::string&) : 
  projectiles(),
  freeList()
{
}

ProjectileList::~ProjectileList(){
  for ( Projectile* p : projectiles ) {
    delete p;
  }
  for ( Projectile* p : freeList ) {
    delete p;
  }
  projectiles.clear();     // clearing a vector is a waste of time, but ...
  freeList.clear();   // still ...

}

void ProjectileList::add(){
  // if free list is empty make a new projectile
  if (freeList.empty()) {
 //   Projectile* proj = new Projectile(p);
 //   projectiles.push_back(p);
  }
  // else 
  else {
    auto ptr = freeList.begin();
    projectiles.push_back(*ptr);
    freeList.erase(ptr);
  }
}

void ProjectileList::draw() const {
  auto ptr = projectiles.begin();
  while(ptr != projectiles.end()) {
    (*ptr)->draw();
    ptr++;
  }
}

void ProjectileList::update(Uint32 ticks){
  std::list<Projectile*>::iterator ptr = projectiles.begin();
  while (ptr != projectiles.end()) {
    (*ptr)->update(ticks);
    if ( (*ptr)->goneTooFar()) {  // Check to see if we should free a projectile
      freeList.push_back(*ptr);
      ptr = projectiles.erase(ptr);
    }   
    else ++ptr;
  }
}
