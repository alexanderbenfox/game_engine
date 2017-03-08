#ifndef MENU_H
#define MENU_H

#include "textdraw.h"
#include "input.h"
#include <vector>
#include <string>

class Graphics;

enum MenuType{
  New, NewNoTut, Load
};

class MenuItem{
public:
  MenuItem(std::string text, MenuType type){
    _value = text;
    _type = type;
  }
  //virtual ~MenuItem();
  void handleInput(Input &input);
  void draw(Graphics &graphics,float x, float y){
    TextRender::RenderTextOnScreen(graphics, _value, 20, white, Vector2(x,y));
  }
private:
  std::string _value;
  MenuType _type;
};

class Menu{
public:
  Menu(int columns, int rows, float x, float y, float width, float height);
  virtual ~Menu();
  virtual void handleInput(Input &input);
  virtual void draw(Graphics &graphics);
  void addMenuItem(std::string text, MenuType type);
private:
  MenuItem** _menuItems;
  int _cursorX, _cursorY, _columns, _rows;
  int _fillX, _fillY;
  float _width, _height, _posX, _posY;
};




#endif