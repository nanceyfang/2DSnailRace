#ifndef HUD__H
#define HUD__H
#include <SDL.h>
#include <string>
#include "player.h"

class Hud {
public:
  Hud(int, int, bool, Player *);

  void toggle();
  void draw() const;
  void drawEnd(bool) const;
  bool getIsOn() const {return isOn;}

  void setMode(bool b) {isGod = b;}
private:
  int xLoc;
  int yLoc;
  bool isOn;
  bool isGod;
  SDL_Renderer * renderer;
  Player * player;
};

#endif
