#ifndef OPPONENT__H
#define OPPONENT__H
#include "playermultisprite.h"
#include "player.h"
#include <string>


class Opponent : public Player {
public:
  Opponent(const std::string&, const Vector2f, int, int, float);
  
 // void draw() const { opponent.draw(); }
  virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

private:

  enum MODE {NORMAL, FAST};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float velocityX;
  float safeDistance;
};

#endif
