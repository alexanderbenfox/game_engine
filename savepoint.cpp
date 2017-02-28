#include "savepoint.h"
#include "player.h"

SavePoint::SavePoint(Graphics &graphics, float x, float y, std::string map) : AnimatedSprite(graphics,"sprites/altar.png",1,1,x,y,64,64), _map(map), _activated(false), _x(x), _y(y){
  _spriteScale = 2;
  this->addAnimation(.05, 1, 0, 0, "Idle", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.03, 9, 0, 0, "TurnOn", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.05, 3, 6, 0, "On", 64, 64, Vector2(0,0),"main");
  this->playAnimation("Idle");
  
  _turningOn = false;
  _on = false;
}

void SavePoint::update(float dt, Player &player){
  AnimatedSprite::update(dt);
  if(!_activated && collidesWith(player.getCollider()))
    _activated = true;
  
  if(_activated && !_on){
    if(!_turningOn && !_on){
      _turningOn = true;
      this->playAnimation("TurnOn");
      _actionTimer = this->getAnimationTime("TurnOn");
    }
    
    _actionTimer -= dt;
    
    if(_actionTimer <= 0){
      if(_turningOn){
        _turningOn = false;
        this->playAnimation("On");
        _on = true;
      }
    }
  }
  
  if(_on){
    this->playAnimation("On");
  }
}

void SavePoint::draw(Graphics &graphics){
  AnimatedSprite::draw(graphics, _x, _y);
}