#include "input.h"
#include <iostream>
#include <algorithm>
#include <iterator>

Input::Input(){
  for(int i = 0 ; i < 11; i++){
    oldkeystates[keys[i]] = false;
  }
}

//resets keys from last frame
void Input::beginNewFrame(){
  //_pressed_keys.clear();
  //_released_keys.clear();
  //_held_keys.clear();
  clearKeys(&_pressed_keys);
  clearKeys(&_released_keys);
  clearKeys(&_held_keys);
}

void Input::clearKeys(std::map<SDL_Scancode, bool> *keyMap){
  std::map<SDL_Scancode, bool>::iterator it = keyMap->begin();
  while(it != keyMap->end()){
    it->second = false;
    it++;
  }
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

void Input::updateKeys(const Uint8 *newkeystates){
  keystates = newkeystates;
  
  for(int i = 0; i < 11; i++)
  {
    if(keystates[keys[i]] && oldkeystates[keys[i]]){
      _held_keys[keys[i]] = true;
    }
    
    if(keystates[keys[i]] && !oldkeystates[keys[i]]){
      _pressed_keys[keys[i]] = true;
    }
    
    if(!keystates[keys[i]] && oldkeystates[keys[i]]){
      _released_keys[keys[i]] = true;
    }
  }
  
  for(int i = 0; i < 11; i++)
  {
    oldkeystates[keys[i]] = keystates[keys[i]];
  }
  
}
