#include <ctime>
#include <random>
#include "playermultisprite.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "explodingSprite.h"

void PlayerMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	
	if (timeSinceLastFrame > frameInterval) {
    if (isJumping && getVelocityX() > 0)
      currentFrame = ((currentFrame+1) % (numberOfFrames/4)) + 4;
    else if (isJumping && getVelocityX() < 0)
      currentFrame = ((currentFrame+1) % (numberOfFrames/4)) + 6;
	  else if (getVelocityX() > 0)
			currentFrame = (currentFrame+1) % (numberOfFrames/4);
		else
			currentFrame = ((currentFrame+1) % (numberOfFrames/4)) + 2;	

    timeSinceLastFrame = 0;
  }	
}

PlayerMultiSprite::~PlayerMultiSprite( ) { if (explosion) delete explosion; }

PlayerMultiSprite::PlayerMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance()->getImages(name) ),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  isJumping(false)
{ 
  srand(rand());
  if (rand()%2)
    setX( getX() + rand()%80);
  else
    setX( getX() - rand()%80);
}

PlayerMultiSprite::PlayerMultiSprite(const PlayerMultiSprite& s) :
  Drawable(s), 
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  isJumping(s.isJumping)
  { }

PlayerMultiSprite& PlayerMultiSprite::operator=(const PlayerMultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  isJumping = (s.isJumping);
  return *this;
}


void PlayerMultiSprite::explode() {
  if ( !explosion ) {
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite); 
    setVelocityY(0);
    setY(getY()-3);
  }
}

void PlayerMultiSprite::draw() const { 
  if ( explosion ){ explosion->draw(); }
  else {
    images[currentFrame]->draw(getX(), getY(), getScale());
  }
}

void PlayerMultiSprite::update(Uint32 ticks) { 

  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
