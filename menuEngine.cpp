#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include "sprite.h"
#include "gamedata.h"
#include "menuEngine.h"
#include "renderContext.h"

MenuEngine::~MenuEngine() { }

MenuEngine::MenuEngine() :
  clock( Clock::getInstance() ),
  renderer( RenderContext::getInstance()->getRenderer() ),
  menu( renderer )
{
}

void MenuEngine::draw() const {
  menu.draw();
  SDL_RenderPresent(renderer);
}

void MenuEngine::update(Uint32) {
}

void MenuEngine::drawTips() const{
  menu.drawTips();
  SDL_RenderPresent(renderer);
}

void MenuEngine::drawCreds() const {
  menu.drawCreds();
  SDL_RenderPresent(renderer);
}

void MenuEngine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;

  while ( !done ) {
  int option;
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
    
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_S] ) menu.incrIcon();
        if (keystate[SDL_SCANCODE_W] ) menu.decrIcon();
        if (keystate[SDL_SCANCODE_SPACE] ) {
          menu.lightOn();
          option = menu.getOptionNo();
          if ( option == 0 ) done = true;
        }
      }
      if(event.type == SDL_KEYUP) {
        menu.lightOff();
      }
    }
    // In this section of the event loop we allow key bounce:
    if ( option == 1 ) {
      drawTips();
    }
    else if ( option == 2) {
      drawCreds();
    }
    else
      draw();
    //update(ticks);
  }
}
