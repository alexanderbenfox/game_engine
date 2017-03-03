#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "conf.h"
#include "graphics.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "menu.h"

enum state{
  game_play, start_menu
};

class StateTransition{
public:
  static StateTransition* getInstance(){
    static StateTransition instance;
    return &instance;
  }
  state getCurrentGameState(){
    return _currentGameState;
  }
  void setCurrentGameState(state nextState){
    _currentGameState = nextState;
  }
  StateTransition(StateTransition const&) = delete;
  void operator=(StateTransition const&) = delete;
private:
  StateTransition(){}
  state _currentGameState;
};

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

class StartMenuState : public GameState{
public:
  StartMenuState();
  void update(float dt, Graphics *graphics);
  void draw(Graphics *graphics);
  void processInput(Input &input);
private:
  Menu startMenu;
};

#endif