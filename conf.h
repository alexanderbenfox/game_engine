#ifndef CONF_H
#define CONF_H

namespace globals{
}

namespace collision_sides{
  enum Side{
    TOP, BOTTOM, LEFT, RIGHT, NONE
  };
  
  inline Side getOppositeSide(Side side)
  {
    return side == TOP ? BOTTOM : side == BOTTOM ? TOP : side == RIGHT ? LEFT : side == LEFT ? RIGHT : NONE;
  }
};

enum Direction {
  LEFT, RIGHT, UP, DOWN
};

struct Vector2{
  int x, y;
  Vector2(): x(0), y(0) {}
  Vector2(int x, int y): x(x), y(y) {}
  Vector2(Vector2 const &v): x(v.x), y(v.y) {}
  Vector2 zero() { return Vector2(0,0); }
};


// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
#define TIMESTEP 16.0 //in milliseconds
#define LAGTIME 0.0  //in nanos

//window parameters
#define WINDOW_TITLE "Game"
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720

//some globals
#define SPRITE_SCALE 2

#endif

