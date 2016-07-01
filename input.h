#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SDL2/SDL.h>

class Input{
public:
  void beginNewFrame();
  void keyUpEvent(const SDL_Event &event);
  void keyDownEvent(const SDL_Event &event);

  bool keyWasPressed(SDL_Scancode key);
  bool keyWasReleased(SDL_Scancode key);
  bool keyIsHeld(SDL_Scancode key);
private:
    
  std::map<SDL_Scancode, bool> _held_keys;
  std::map<SDL_Scancode, bool> _pressed_keys;
  std::map<SDL_Scancode, bool> _released_keys;

  
};


#endif
