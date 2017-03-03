#include "gamestate.h"
#include "conf.h"
#include "menu.h"

StartMenuState::StartMenuState() : startMenu(Menu(1, 2, WINDOW_WIDTH/3, 0, WINDOW_WIDTH/3, WINDOW_HEIGHT)){
  startMenu.addMenuItem("New", New);
  startMenu.addMenuItem("Load", Load);
};

void StartMenuState::update(float dt, Graphics *graphics){}

void StartMenuState::draw(Graphics *graphics){
  startMenu.draw(*graphics);
}

void StartMenuState::processInput(Input &input){
  startMenu.handleInput(input);
}