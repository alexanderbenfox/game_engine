#include "pickups.h"
#include "player.h"

Pickup::Pickup(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width) : AnimatedSprite(graphics, filePath, startX, startY, posX, posY, height, width), _stopped(true), _pickedUp(false) , _width(width*SPRITE_SCALE), _height(height*SPRITE_SCALE)
{
  
}

void Pickup::update(float dt, Player *player){
  AnimatedSprite::update(dt);
}

void Pickup::draw(Graphics &graphics){
  AnimatedSprite::draw(graphics, this->getX(), this->getY());
}

void Pickup::checkPickedUp(Player *player){
  if(!_pickedUp && this->getCollider().collidesWith(player->getCollider())){
    _pickedUp = true;
    onPickup(player);
  }
}

bool Pickup::isPickedUp(){
  return _pickedUp;
}

void Pickup::handleRightCollision(Rectangle tile){
  _stopped = true;
}

void Pickup::handleLeftCollision(Rectangle tile){
  _stopped = true;
}
void Pickup::handleUpCollision(Rectangle tile){
  _stopped = true;
}
void Pickup::handleDownCollision(Rectangle tile){
  _stopped = true;
}

void Pickup::updateMovement(float dt, Player *player){
  if(!_stopped){
    _dy += (GRAVITY/2*dt);
  }
  else{
    _dy = 0;
    _dx = 0;
  }
  
  checkPickedUp(player);
  x_ += _dx*dt;
  y_ += _dy*dt;
  this->setX(x_);
  this->setY(y_);
}

void Pickup::updateNoMovement(float dt, Player *player){
  checkPickedUp(player);
}

Currency::Currency(Graphics &graphics, float x, float y) : Pickup(graphics,"sprites/currency.png",1,1,x,y,8,8){
  x_ = x;
  y_ = y;
  _stopped = false;
  _grounded = false;
  _dy -= (rand()%2000);
  _dx = rand()%400 - 200;
}

void Currency::update(float dt, Player *player){
  Pickup::updateMovement(dt, player);
}

void Currency::draw(Graphics &graphics){
  Pickup::draw(graphics);
}

void Currency::onPickup(Player *player){
  player->changeCurrency(1);
}
