#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "conf.h"
#include "graphics.h"
#include "input.h"
#include "map.h"
#include "player.h"

class GameState {
public:
  virtual void update(float dt, Graphics *graphics) = 0;
  virtual void draw(Graphics *graphics) = 0;
  virtual void processInput(Input &input) = 0;
};

class PlayState : public GameState{
public:
  PlayState(){}
  PlayState(Graphics *graphics);
  void update(float dt, Graphics *graphics);
  void draw(Graphics *graphics);
  void processInput(Input &input);
private:
  Player player;
  Map map;
  SDL_Rect camera;
  int pauseTime;
  bool transitionMap;
};

class MenuState : public GameState{
public:
  void update(float dt, Graphics *graphics);
  void draw(Graphics *graphics);
  void processInput(Input &input);
};

#endif