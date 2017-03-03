#include "menu.h"
#include "graphics.h"
#include "gamestate.h"

void MenuItem::handleInput(Input &input){
  if(input.keyIsHeld(SDL_SCANCODE_RETURN)){
    if(this->_type == New){
      StateTransition::getInstance()->setCurrentGameState(game_play);
    }
  }
}

void drawRect(Graphics &graphics, float x1, float x2, float y1, float y2){
  //set color white
  SDL_SetRenderDrawColor(graphics.getRenderer(), 255,255,255, 255);
  
  SDL_RenderDrawLine(graphics.getRenderer(), x1,y1,x2,y1);
  SDL_RenderDrawLine(graphics.getRenderer(), x1,y1,x1,y2);
  SDL_RenderDrawLine(graphics.getRenderer(), x2,y1,x2,y2);
  SDL_RenderDrawLine(graphics.getRenderer(), x1,y2,x2,y2);
  
  //set color black
  SDL_SetRenderDrawColor(graphics.getRenderer(), 0,0,0, 255);
}

Menu::Menu(int columns, int rows, float x, float y, float width, float height): _columns(columns), _rows(rows), _posX(x), _posY(y), _width(width), _height(height){
  //create double array of items
  _menuItems = (MenuItem**) malloc(_columns * sizeof(MenuItem*));
  for(int i = 0; i<_columns; i++){
    _menuItems[i] = (MenuItem*)malloc(_rows * sizeof(MenuItem));
  }
  
  _fillX = 0;
  _fillY = 0;
  _cursorX = 0;
  _cursorY = 0;
}

Menu::~Menu(){
  free(_menuItems);
}

void Menu::handleInput(Input &input){
  if(input.keyIsHeld(SDL_SCANCODE_UP)){
    if(_cursorY > 0)
      _cursorY -= 1;
  }
  if(input.keyIsHeld(SDL_SCANCODE_DOWN)){
    if(_cursorY < _rows-1)
      _cursorY += 1;
  }
  if(input.keyIsHeld(SDL_SCANCODE_LEFT)){
    if(_cursorX > 0)
      _cursorX -= 1;
  }
  if(input.keyIsHeld(SDL_SCANCODE_RIGHT)){
    if(_cursorX < _columns-1)
      _cursorX += 1;
  }
  
  _menuItems[_cursorX][_cursorY].handleInput(input);
}

void Menu::draw(Graphics &graphics){
  float item_w = _width/_columns;
  float item_h = _height/_rows;
  
  for(int i = 0; i<_columns; i++){
    for(int j = 0; j <_rows; j++){
      _menuItems[i][j].draw(graphics, _posX + i*item_w, _posY+ j*item_h);
    }
  }
  
  float left = _cursorX*(item_w);
  float right = left + (item_w);
  float top = _cursorY*(item_h);
  float bottom = top + (item_h);
  
  left+=_posX;
  right +=_posX;
  top+=_posY;
  bottom+=_posY;
  
  drawRect(graphics, left, right, top, bottom);
}

void Menu::addMenuItem(std::string text, MenuType type){
  _menuItems[_fillX][_fillY] = MenuItem(text,type);
  
  if(_fillX < _columns-1){
    _fillX++;
  }
  else{
    if(_fillY < _rows-1){
      _fillY++;
      _fillX = 0;
    }
  }
  
}