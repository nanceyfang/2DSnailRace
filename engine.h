#include <vector>
#include <SDL.h>
#include <chrono>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "ground.h"
#include "opponent.h"
#include "hud.h"
#include "menuEngine.h"
#include "sound.h"

class CollisionStrategy;
class Player;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  MenuEngine menuEngine;
  World world;
  World grass;
  Viewport& viewport;

  Player*   player;
  std::vector<Ground*> grounds;
  std::vector<Opponent*> opponents;
  std::vector<Drawable*> sprites;
  
  int currentSprite;

  Hud hud;
  
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;

  bool makeVideo;
  bool godMode;
  
  double timer;
  std::chrono::high_resolution_clock::time_point startTime;
  bool isEnd;
  bool isWinner;
  double bestTime;

  SDLSound sound;

  void draw() ;
  void update(Uint32);

  Engine(const Engine&);
  Engine& operator=(const Engine&);
  void printScales() const;
  void checkForCollisions();
  void endGame(bool);
};
