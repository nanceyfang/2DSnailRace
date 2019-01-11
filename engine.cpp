#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <iomanip>
#include <sstream>
#include "sprite.h"
#include "playermultisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "opponent.h"
#include "collisionStrategy.h"
#include "scoreData.h"


Engine::~Engine() { 
  for ( Player* opponent : opponents ) {
    delete opponent;
  }
  delete player;
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  } 
  for ( Ground* ground : grounds ) {
    delete ground;
  }
  for ( Drawable* sprite : sprites ) {
    delete sprite;
  }
  if (isEnd && timer < bestTime) {
    bestTime = timer;
    std::cout << "Updating High Score" << std::endl;
    ScoreData::getInstance().setHighScore(bestTime);
    ScoreData::getInstance().writeTheXml();
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  menuEngine(),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  grass("grass", Gamedata::getInstance().getXmlInt("grass/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Player")),
  grounds(),
  opponents(),
  sprites(),
  currentSprite(1),
  hud( Gamedata::getInstance().getXmlInt("Hud/xLoc"), 
       Gamedata::getInstance().getXmlInt("Hud/yLoc"),
       godMode,
       player 
  ),
  strategies(),
  currentStrategy(0),
  makeVideo( false ),
  godMode( false),
  timer(0),
  startTime( std::chrono::high_resolution_clock::now() ),
  isEnd(false),
  isWinner(false),
  bestTime( ScoreData::getInstance().getHighScore() ),
  sound()
{

// make opponents
int num = Gamedata::getInstance().getXmlInt("Opponent/num");
  for (int i = 0; i < num; i++) {
    opponents.push_back( new Opponent("Opponent", 
      Vector2f(player->getX(),player->getY() ), 
        Gamedata::getInstance().getXmlInt("Opponent/imageWidth"),
        Gamedata::getInstance().getXmlInt("Opponent/imageHeight"), 
        Gamedata::getInstance().getXmlInt("Opponent/speedX") ) );
    player->attach( opponents[i]);
  }
  
//Create Obstacle Course

  int groundWidth = Gamedata::getInstance().getXmlInt("Ground/imageWidth");
  for (int i = 0 ; i < 33; ++i){
    grounds.push_back( new Ground( 
      Gamedata::getInstance().getXmlInt("Ground/xLoc") + (i * groundWidth),
      Gamedata::getInstance().getXmlInt("Ground/yLoc"), 
      ImageFactory::getInstance()->getImage("Ground") ));
  }

  for (int i = 0; i < 11; ++i) {
    grounds.push_back( new Ground(0,
      Gamedata::getInstance().getXmlInt("Ground/yLoc") - (i*groundWidth), 
      ImageFactory::getInstance()->getImage("Ground") )
    );
  }
  
  for (int i = 0; i < 11; ++i) {
    grounds.push_back( new Ground( Gamedata::getInstance().getXmlInt("world/width") - 64,
      Gamedata::getInstance().getXmlInt("Ground/yLoc") - (i*groundWidth), 
      ImageFactory::getInstance()->getImage("Ground") )
    );
  }
  for (int i = 0 ; i < num; ++i ) {
    for (int j = 0 ; j < 2; ++j){
      grounds.push_back( new Ground(groundWidth + 64*j , 
        Gamedata::getInstance().getXmlInt("Ground/yLoc") - 130 - 70*i,  
        ImageFactory::getInstance()->getImage("Ground") ) );
    }
  }
  
  // ground w/ the flag
  for (int i = 0; i < 6; ++i) {
    grounds.push_back( new Ground(
      Gamedata::getInstance().getXmlInt("Wall/xLoc") + (i * groundWidth),
      Gamedata::getInstance().getXmlInt("Wall/yLoc")+30, 
      ImageFactory::getInstance()->getImage("Ground") )
    );
  }
  
  for (int i = 0; i < 10; ++i) {
    grounds.push_back ( new Ground(
      745 + (i * groundWidth), 360, 
      ImageFactory::getInstance()->getImage("Ground") )
    );
  }
  
  for (int i = 0; i < 4; ++i) {
    grounds.push_back ( new Ground(
      1500 + (i * groundWidth), 500, 
      ImageFactory::getInstance()->getImage("Ground") )
    );
  }
  
  for (int i = 0; i < 4; ++i) {
    grounds.push_back ( new Ground(
      550 , 360 - (i * groundWidth), 
      ImageFactory::getInstance()->getImage("Ground") )
    );
  }
  
  grounds.push_back( new Ground (330, 415, ImageFactory::getInstance()->getImage("Ground")));
  
  // add flag
  sprites.push_back ( new Sprite("Flag") );
  
  // 0 - rect  1 - pPixel  2 - MidPoint
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player->getPlayer());
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw()  {
  world.draw();
  grass.draw();

  for (auto o: opponents) {
    o->draw();
  }
  player->draw();
  for (unsigned int i = 0; i < grounds.size(); i++)
  {
    grounds[i]->draw();
  }

  sprites[0]->draw();

  hud.draw();
  if (isEnd)
    hud.drawEnd(isWinner);
    
  viewport.draw();

  if (!isEnd){
  std::chrono::duration<double> endTime =
  std::chrono::duration_cast<std::chrono::duration<double>>
  (std::chrono::high_resolution_clock::now() - startTime);
  timer = endTime.count();

  }
  
  std::stringstream ssBTime;
  ssBTime << std::setprecision(2) << std::fixed << bestTime;
  string bTimeString;
  ssBTime >> bTimeString;
  
  io.writeText("Best Time: " + bTimeString + " secs", 900, 60);
  
  std::stringstream ssTime;
  ssTime << std::setprecision(2) << std::fixed << timer ;
  string timeString;
  ssTime >> timeString;
  
  io.writeText("Timer: " + timeString + " secs", 900, 90);
  
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions(){

  // player collide w/ opponents
  auto it = opponents.begin();
  while ( it != opponents.end() ) {
    if (!(*it)->getPlayer()->isExploding() ) {
      if ( strategies[currentStrategy]->execute(*player, 
          *((*it)->getPlayer()) ) ) {
        if (player->getX() < (*it)->getX())
          player->left();
        else if (player->getX() > (*it)->getX()){
          player->right();       
        }
        else if ( player->getY() > (*it)->getY() )
          player->up((*it)->getY() - 33);
        else
          player->down();
      }
    }
    ++it;
  }
  
  
  // collide w/ flag
  if(!isEnd){
      if ( strategies[currentStrategy]->execute(*player, *sprites[0] )  ){
        endGame(true);
        sound[0];
      }
  }
  
  for (unsigned int i = 0; i < opponents.size(); ++i){
    if ( strategies[currentStrategy]->execute(*opponents[i], *sprites[0] ) ) {
      endGame(false);    
    } 
  }
  
  // player/opponents collide w/ ground
  auto it2 = grounds.begin();
  while ( it2 != grounds.end()) {
    if ( strategies[currentStrategy]->execute(*player, *(*it2)->getSprite() ) ) 
    {     
      
      //player above
      if ( player->getY() < (*it2)->getY() - 50 && player->getX() > (*it2)->getX() - 50 && player->getX() < (*it2)->getX() + 50 ){
        player->up((*it2)->getY() - 
          (Gamedata::getInstance().getXmlInt("Player/imageHeight") +1));
          player->setJumpState(false);
      }
      //player below
      else if (player->getY() > (*it2)->getY()) {
        player->down();
        player->setJumpState(true);
      } 
      // player rightSide
      if (player->getX() > (*it2)->getX() && player->getY() > (*it2)->getY()) {
        player->right();
        player->setJumpState(false);
      }
      // player leftSide
      else if (player->getX() < (*it2)->getX() && player->getY() > (*it2)->getY())
        player->left(); 
        player->setJumpState(false);
    }
    
    for (unsigned int i = 0; i < opponents.size(); ++i) {
      if ( strategies[currentStrategy]->execute( *opponents[i],
          *(*it2)->getSprite() ) ) {
        opponents[i]->setJumpState(false);       
        if ( opponents[i]->getY() < (*it2)->getY()-50 && opponents[i]->getX() > (*it2)->getX() - 50 && opponents[i]->getX() < (*it2)->getX() + 50 ){
          opponents[i]->up((*it2)->getY() - 
            (Gamedata::getInstance().getXmlInt("Opponent/imageHeight") + 1) );
        }
        else if (opponents[i]->getY() > (*it2)->getY()) {
          opponents[i]->down();
        } 
        if (opponents[i]->getX() > (*it2)->getX() && opponents[i]->getY() > (*it2)->getY()) {
          opponents[i]->right();
        }
        else if (opponents[i]->getX() < (*it2)->getX() && 
          opponents[i]->getY() > (*it2)->getY()) {
          opponents[i]->left();
        }
      }
    }
    ++it2;
  }
  
  // projectile collide w/ enemies
  auto it3 = player->getProjectiles().begin();
  for (unsigned int i = 0; i < player->getProjectiles().size(); i++){
    for (unsigned int i = 0; i < opponents.size(); ++i) {
      if(strategies[0]->execute(*opponents[i], **it3) ){
        opponents[i]->explode();
      }     
    }            
  }
  
  //opponent projectiles collide w/ player
  if (!godMode){
    for (unsigned int i = 0; i < opponents.size(); ++i) {
      auto it3 = opponents[i]->getProjectiles().begin();
      for (unsigned int j = 0; j < opponents[i]->getProjectiles().size(); j++){
        if(strategies[0]->execute(*player, **it3) ){
          player->explode();
        }         
      }
    }
  }
}

void Engine::update(Uint32 ticks) {
  if (!isEnd){
      checkForCollisions();
      for (unsigned int i = 0;  i < opponents.size(); ++i)
        opponents[i]->update(ticks);
      player->update(ticks);
      world.update();
      grass.update();
      viewport.update(); // always update viewport last
  }
}

void Engine::endGame(bool w) {
  player->endGame();
  for(Opponent* opponent : opponents)
    (*opponent).endGame();
  isEnd = true;
  isWinner = w;
}

bool Engine::play() {

  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {

    for (unsigned int i = 0; i < opponents.size() ; ++i) {
      opponents[i]->setPlayerPos( Vector2f(player->getX(), player->getY()) );
    }
    
    //hud update
    hud.setMode(godMode);
    
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        // SHOOT
        if (keystate[SDL_SCANCODE_SPACE]) {      
          player->shoot();
          sound[1];
        }
        //Menu
        if ( keystate[SDL_SCANCODE_P] ) {
          clock.pause();
          menuEngine.play();
          clock.unpause();
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
          hud.toggle();
        }
        // restart
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        //godmode
        if ( keystate[SDL_SCANCODE_G] ) {
          if(godMode)
            godMode = false;
          else
            godMode = true;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      // JUMP
      if (keystate[SDL_SCANCODE_W]){
        static_cast<Player*>(player)->jump();
      }
     
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
      
    }

  }
  return false;
}
