#include "hud.h"
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"
#include <SDL2/SDL.h>
#include <sstream>

Hud::Hud(int x, int y, bool b, Player* p) : 
  xLoc(x), yLoc(y), isOn(true), isGod(b),
  renderer(RenderContext::getInstance()->getRenderer() ) ,
  player(p)
{ }

void Hud::toggle(){
  if(isOn)
    isOn=false;
  else
    isOn=true;
}

void Hud::drawEnd(bool w) const {
  SDL_Rect r;
  r.x = Gamedata::getInstance().getXmlInt("view/width")/2 - 200;
  r.y = Gamedata::getInstance().getXmlFloat("view/height")/2 - 100;
  r.w = 400;
  r.h = 200;
  
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Now set the color for the hud:
  SDL_SetRenderDrawColor( renderer, 225, 225, 225, 255 );

  // Render rect
  SDL_RenderFillRect( renderer, &r );
  SDL_RenderDrawRect( renderer, &r );
  
  if (w)
    IoMod::getInstance().writeText("You Win!", r.x + 140, r.y + 50);
  else
    IoMod::getInstance().writeText("You Lose!", r.x + 140, r.y + 50);
    
  IoMod::getInstance().writeText("Press 'R' to restart!", r.x + 90, r.y + 100);
}


void Hud::draw()const{
  
    SDL_Rect r;
    r.x = xLoc-20;
    r.y = yLoc-80;
    r.w = 300*Gamedata::getInstance().getXmlFloat("Hud/sizeScale");
    r.h = 100*Gamedata::getInstance().getXmlFloat("Hud/sizeScale");

    // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Now set the color for the hud:
    SDL_SetRenderDrawColor( renderer, 200, 200, 200, 255/2 );

    // Render rect
    SDL_RenderFillRect( renderer, &r );

    // Now set the color for the outline of the hud:
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255/2 );
    SDL_RenderDrawRect( renderer, &r );
    std::stringstream bListStream;
    bListStream << player->projSize();
    
    std::stringstream fListStream;
    fListStream << player->freeSize();
    
    string bList = "Bullet List: " + bListStream.str();
    string fList = "Free List: " + fListStream.str();
    string godMode = "God Mode: ";

    if (isGod)
      godMode += "ON";
    else
      godMode += "OFF";    
    
    IoMod::getInstance().writeText(bList, xLoc, yLoc - 80);
    IoMod::getInstance().writeText(fList, xLoc, yLoc - 55);
    IoMod::getInstance().writeText(godMode, xLoc, yLoc - 30);
    IoMod::getInstance().writeText("Turn HUD On/Off: 'F1'", xLoc, yLoc - 5);

    if (isOn) {
      // Set the color for the background:
      SDL_Rect r;
      r.x = xLoc-20;
      r.y = yLoc+30;
      r.w = 300*Gamedata::getInstance().getXmlFloat("Hud/sizeScale");
      r.h = 300*Gamedata::getInstance().getXmlFloat("Hud/sizeScale");

      // First set the blend mode so that alpha blending will work;
      // the default blend mode is SDL_BLENDMODE_NONE!
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      // Now set the color for the hud:
      SDL_SetRenderDrawColor( renderer, 200, 200, 200, 255/2 );

      // Render rect
      SDL_RenderFillRect( renderer, &r );

      // Now set the color for the outline of the hud:
      SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255/2 );
      SDL_RenderDrawRect( renderer, &r );
        
      IoMod::getInstance().writeText("Player Controls", xLoc, yLoc + 30);
      IoMod::getInstance().writeText("Move Left: 'A'", xLoc, yLoc + 60);
      IoMod::getInstance().writeText("Move Right: 'D'", xLoc, yLoc + 90);
      IoMod::getInstance().writeText("Jump: 'W'", xLoc, yLoc + 120);
      IoMod::getInstance().writeText("Shoot: 'Space bar'", xLoc, yLoc + 150);
      IoMod::getInstance().writeText("Pause: 'P'", xLoc, yLoc + 180);
      IoMod::getInstance().writeText("Quit: 'Esc'", xLoc, yLoc + 210);
      IoMod::getInstance().writeText("Restart: 'R'", xLoc, yLoc + 240);
      IoMod::getInstance().writeText("Enable God Mode: 'G'", xLoc, yLoc + 270);
    }

}
