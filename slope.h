#ifndef SLOPE_H
#define SLOPE_H

#include "conf.h"
#include <cmath>

class Slope{
public:
  Slope()
  {}
  Slope(Vector2 p1, Vector2 p2)
  {
    this->_p1 = p1;
    this->_p2 = p2;
    this->_slope = calcSlope(p1,p2);
  }

  const inline float calcSlope(Vector2 p1, Vector2 p2) const
  {
    float y = std::fabs(p2.y) - std::fabs(p1.y);
    float x = std::fabs(p2.x) - std::fabs(p1.x);
    if( (p2.x - p1.x) != 0)
      return y/x;
    else 
      return 0;
  }

  const inline float getSlope() const
  {
    return this->_slope;
  }

  const inline bool collidesWith(const Rectangle &other) const
  {
    return
        (other.getRight() >= this->_p2.x &&
        other.getLeft() <= this->_p1.x &&
        other.getTop() <= this->_p2.y &&
        other.getBottom() >= this->_p1.y) ||
        (other.getRight() >= this->_p1.x &&
        other.getLeft() <= this->_p2.x &&
        other.getTop() <= this->_p1.y &&
        other.getBottom() >= this->_p2.y) ||
        (other.getLeft() <= this->_p1.x &&
        other.getRight() >= this->_p2.x &&
        other.getTop() <= this->_p1.y &&
        other.getBottom() >= this->_p2.y) ||
        (other.getLeft() <= this->_p2.x &&
        other.getRight() >= this->_p1.x &&
        other.getTop() <= this->_p2.y &&
        other.getBottom() >= this->_p1.y);
  }

  const inline Vector2 getP1() const
  {
    return this->_p1;
  }

  const inline Vector2 getP2() const
  {
    return this->_p2;
  }

private:
  Vector2 _p1;
  Vector2 _p2;
  float _slope;
};

#endif
