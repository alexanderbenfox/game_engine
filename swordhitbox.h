#ifndef SWORDHITBOX_H
#define SWORDHITBOX_H

#include "rectangle.h"
#include "map.h"
#include <map>
#include <string>
#include "sfx.h"

class SFXManager;

enum HitboxType{
  STANDING,STANDING2, CROUCHING, JUMPING
};

struct Hitbox{
  Rectangle rect;
  bool hitRegistered = false;
  bool isActive = false;
  float lifetime = 0.01;
  float delay = 0.05;
  float direction;
  HitboxType type;
  
  Hitbox(){
    rect = Rectangle(0,0,0,0);
  }
  
  Hitbox(Rectangle box, float dir){
    direction = dir;
    rect = box;
    isActive = false;
    type = STANDING;
  }
  
  void moveHitbox(float dx, float dy){
    rect.moveAnchor(rect.getLeft() + dx, rect.getTop() + dy);
  }
  
  void setDelay(float d){
    delay = d;
  }
};

class SwordHitbox {
public:
  SwordHitbox();
  SwordHitbox(Graphics &graphics);
  
  void createHitBox(int x, int y, HitboxType type, float direction);
  
  void handleEnemyCollisions(Map &map);
  void update(float dt, float dx, float dy, float actionTimer);
  void draw(Graphics &graphics);
  
  bool hitRegistered();
  
protected:
  Hitbox _currentHitbox;
  std::map<std::string, Rectangle> _hitboxes;
  SFXManager _sfx;
  
};

#endif
