#ifndef SAVEPOINT_H
#define SAVEPOINT_H

#include "animatedsprite.h"

class Player;

class SavePoint : public AnimatedSprite{
public:
  SavePoint(){}
  SavePoint(Graphics &graphics, float x, float y, std::string map);
  
  void update(float dt, Player &player);
  void draw(Graphics &graphics);
private:
  std::string _map;
  bool _activated = false;
  float _actionTimer;
  
  bool _turningOn, _on;
  
  float _x, _y;
};


#endif