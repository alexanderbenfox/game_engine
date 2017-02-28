#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SDL2/SDL.h>

class Input{
public:
  Input();
  void beginNewFrame();
  void clearKeys(std::map<SDL_Scancode, bool> *keyMap);
  void keyUpEvent(const SDL_Event &event);
  void keyDownEvent(const SDL_Event &event);

  bool keyWasPressed(SDL_Scancode key);
  bool keyWasReleased(SDL_Scancode key);
  bool keyIsHeld(SDL_Scancode key);
  
  void updateKeys(const Uint8 *keystates);
private:
    
  std::map<SDL_Scancode, bool> _held_keys;
  std::map<SDL_Scancode, bool> _pressed_keys;
  std::map<SDL_Scancode, bool> _released_keys;
  
  std::map<SDL_Scancode,bool> oldkeystates;
  const Uint8 *keystates;
  
  SDL_Scancode A = SDL_SCANCODE_A;
  SDL_Scancode S = SDL_SCANCODE_S;
  SDL_Scancode D = SDL_SCANCODE_D;
  SDL_Scancode Q = SDL_SCANCODE_Q;
  SDL_Scancode W = SDL_SCANCODE_W;
  SDL_Scancode E = SDL_SCANCODE_E;
  SDL_Scancode LEFT = SDL_SCANCODE_LEFT;
  SDL_Scancode RIGHT = SDL_SCANCODE_RIGHT;
  SDL_Scancode UP = SDL_SCANCODE_UP;
  SDL_Scancode DOWN = SDL_SCANCODE_DOWN;
  SDL_Scancode RETURN = SDL_SCANCODE_RETURN;
  
  SDL_Scancode keys[11] = {A,S,D,Q,W,E,LEFT,RIGHT,UP,DOWN,RETURN};

  
};


#endif
