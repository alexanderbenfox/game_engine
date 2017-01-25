#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "conf.h"
#include <iostream>
#include <SDL2/SDL.h>

class Graphics;

class Rectangle{

public:
  Rectangle() {}
  Rectangle(int x, int y, int width, int height) :
    _x(x),
    _y(y),
    _width(width),
    _height(height)
  {}

  const int getCenterX() const {
    return _x+(_width/2);}
  const int getCenterY() const {
    return _y+(_height/2);}

  const int getLeft() const {
    return _x;}
  const int getRight() const {
    return (_x+_width);}
  const int getTop() const {
    return _y;}
  const int getBottom() const {
    return (_y+_height);}

  const int getWidth() const {return _width;}
  const int getHeight() const {return _height;}
  
  void moveAnchor(int x, int y){
    _x = x;
    _y = y;
  }

  const int getSide(const collision_sides::Side side) const{
    switch (side) {
      case collision_sides::LEFT :
        return this->getLeft();
        break;
      case collision_sides::RIGHT :
        return this->getRight();
        break;
      case collision_sides::TOP :
        return this->getTop();
        break;
      case collision_sides::BOTTOM :
        return this->getBottom();
      default:
        return collision_sides::NONE;
        break;
    }
  }

  const bool collidesWith(const Rectangle &other) const {
    return
      getRight() >= other.getLeft() &&
      getLeft() <= other.getRight() &&
      getBottom() >= other.getTop() &&
      getTop() <= other.getBottom();
  }
  
  const collision_sides::Side getCollisionSide(Rectangle &other) const{
    int overlapRight, overlapLeft, overlapBottom, overlapTop;
    overlapRight = this->getRight() - other.getLeft();
    overlapLeft = this->getLeft() - other.getRight();
    overlapBottom = this->getBottom() - other.getTop();
    overlapTop = this->getTop() - other.getBottom();
    
    //get smallest element
    int overlaps[4] = {abs(overlapRight), abs(overlapLeft), abs(overlapBottom), abs(overlapTop)};
    int minValue = abs(overlapRight);
    for(int i = 0; i < 4; i++){
      if (overlaps[i] < minValue)
        minValue = overlaps[i];
    }
    
    if (minValue == abs(overlapRight)) return collision_sides::RIGHT;
    if (minValue == abs(overlapLeft)) return collision_sides::LEFT;
    if (minValue == abs(overlapBottom)) return collision_sides::BOTTOM;
    if (minValue == abs(overlapTop)) return collision_sides::TOP;
    
    return collision_sides::NONE;
  }

  const inline Rectangle getRect() const {return *this;}

protected:

private:
  int _x, _y, _width, _height;

};

#endif
