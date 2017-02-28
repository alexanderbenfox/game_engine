#ifndef PICKUPS_H
#define PICKUPS_H

#include "animatedsprite.h"

class Player;

class Pickup : public AnimatedSprite{
public:
  Pickup() : _width(0), _height(0){}
  Pickup(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width);
  virtual void update(float dt, Player *player);
  virtual void draw(Graphics &graphics);
  
  virtual void onPickup(Player *player) = 0;
  void checkPickedUp(Player *player);
  
  Rectangle getCollider(){
    return Rectangle(x_, y_, _width, _height);
  }
  
  virtual void handleRightCollision(Rectangle tile);
  virtual void handleLeftCollision(Rectangle tile);
  virtual void handleUpCollision(Rectangle tile);
  virtual void handleDownCollision(Rectangle tile);
  
  bool isPickedUp();
  
  void updateMovement(float dt, Player *player);
  void updateNoMovement(float dt, Player *player);
protected:
  float _dx, _dy, x_, y_;
  int _width, _height;
  bool _grounded, _thisFrameGrounded, _dead;
  bool _colUp, _colDown, _colRight, _colLeft;
  bool _stopped, _pickedUp;
};

class Currency : public Pickup{
public:
  Currency(){}
  Currency(Graphics &graphics, float x, float y);
  
  void update(float dt, Player *player);
  void draw(Graphics &graphics);
  
  void onPickup(Player *player);
};

#endif