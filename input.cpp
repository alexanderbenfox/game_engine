#include "input.h"


//resets keys from last frame
void Input::beginNewFrame(){
  _pressed_keys.clear();
  _released_keys.clear();
}


void Input::keyDownEvent(const SDL_Event &event){
  _pressed_keys[event.key.keysym.scancode] = true;
  _held_keys[event.key.keysym.scancode] = true;
}

void Input::keyUpEvent(const SDL_Event &event){
  _released_keys[event.key.keysym.scancode] = true;
  _held_keys[event.key.keysym.scancode] = false;
}

bool Input::keyWasPressed(SDL_Scancode key){
  return _pressed_keys[key];
}

bool Input::keyWasReleased(SDL_Scancode key){
  return _released_keys[key];
}

bool Input::keyIsHeld(SDL_Scancode key){
  return _held_keys[key];
}
