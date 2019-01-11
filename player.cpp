#include "player.h"
#include "gamedata.h"

Player::Player( const std::string& name) :
  player(name),
  initialVelocity(player.getVelocity()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  observers(), 
  isJumping(false),
  projectiles(), 
  freeList(),
  minSpeed( Gamedata::getInstance().getXmlInt("Projectile/speedX") ),
  projInterval( Gamedata::getInstance().getXmlInt("Projectile/interval") ),
  timeSinceLastFrame(0.0),
  isEnd(false)
{ }

Player::~Player(){
  for(auto p : projectiles)
    delete p;
  for(auto p : freeList)
    delete p;
}

void Player::stop() { 
  player.setVelocity( Vector2f( player.getVelocity()[0] * .90 , 
    player.getVelocity()[1] + Gamedata::getInstance().getXmlInt("gravity")) );
}

void Player::right() {  
    player.setVelocityX(initialVelocity[0]);
} 

void Player::left()  { 
  if ( player.getX() > 0) {
    player.setVelocityX(-initialVelocity[0]);
  }
} 

//JUMP
void Player::up(float y){ 
  player.setY(y);
  player.setVelocityY(0);
} 

void Player::down()  { 
    player.setVelocityY( initialVelocity[1] );
}
 
void Player::detach( Player* o ) {
  std::list<Player*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::jump(){ 
  if(!isJumping){
    player.setVelocityY(-Gamedata::getInstance().getXmlInt("gravity") * 45);
    isJumping = true;
    player.setIsJumping(true);
  }
} 

void Player::shoot() {
  if ( timeSinceLastFrame < projInterval ) return;
  
  if ( !player.isExploding() & !isEnd ) {
      Vector2f vec = Vector2f(player.getVelocity()[0], 0);
      // if free list is empty make a new projectile
      if (freeList.empty()) {
        float deltaX = getScaledWidth();
        float deltaY = getScaledHeight()/2;
        // I need to add some minSpeed to velocity:
        Projectile* proj = new Projectile("Projectile");
        
        if (vec[0] >= 0) {
          proj->setVelocity( vec + Vector2f(minSpeed, 0) );
          proj->setPosition( player.getPosition() +Vector2f(deltaX, deltaY) );
        }
        else {                
          proj->setVelocity( vec - Vector2f(minSpeed, 0) );
          proj->setPosition( player.getPosition() + Vector2f(-15, deltaY) );
        }
        projectiles.push_back( proj );
      }
      // else 
      else {
        float deltaX = getScaledWidth();
        float deltaY = getScaledHeight()/2;
        auto ptr = freeList.begin();

        if (vec[0] >= 0) {
          (*ptr)->setVelocity( vec + Vector2f(minSpeed, 0) );
           (*ptr)->setPosition( player.getPosition() +Vector2f(deltaX, deltaY) );     
        }
        else {       
          (*ptr)->setVelocity( vec - Vector2f(minSpeed, 0) );
          (*ptr)->setPosition( player.getPosition() + Vector2f(-15, deltaY) );
        } 
     
        projectiles.push_back(*ptr);
        freeList.erase(ptr);
      }
  }
  timeSinceLastFrame = 0;
}

void Player::draw() const { 
  player.draw(); 
  
  for ( auto bullet : projectiles ) {
    bullet->draw();
  }
}

void Player::endGame() {
  isEnd = true;
  player.setVelocity( Vector2f(0,0) );
}

void Player::update(Uint32 ticks) { 
  player.update(ticks);
   
  stop();

  timeSinceLastFrame += ticks;
//  projectiles.update(ticks);
  auto ptr = projectiles.begin();
  while ( ptr!= projectiles.end() ) {
      (*ptr)->update(ticks);
    // add to free list if gone too far
    if ( (*ptr)->goneTooFar() ){
      freeList.push_back(*ptr);
      (*ptr)->reset();
      ptr = projectiles.erase(ptr);
    }
    else
    ptr++;
  }
}
