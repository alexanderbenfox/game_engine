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

void Pickup::stop(){
  _stopped = true;
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


ChargeSpeedUpgrade::ChargeSpeedUpgrade(Graphics &graphics, float x, float y) : Pickup(graphics, "sprites/chargespeedupgrade.png", 1,1,x,y,32*SPRITE_SCALE,32*SPRITE_SCALE)
{
  x_ = x;
  y_ = y;
  _stopped = false;
  _grounded = false;
}

void ChargeSpeedUpgrade::update(float dt, Player *player){
  Pickup::updateNoMovement(dt, player);
}
void ChargeSpeedUpgrade::draw(Graphics &graphics){
  Pickup::draw(graphics);
}
void ChargeSpeedUpgrade::onPickup(Player *player){
  player->showPopup("Charge speed doubled!");
  PersistentInfo::getUpgrades()->acquireChargeSpeed();
}

HealthUpgrade::HealthUpgrade(Graphics &graphics, float x, float y) : Pickup(graphics, "sprites/healthupgrade.png", 1,1,x,y,32*SPRITE_SCALE,32*SPRITE_SCALE)
{
  x_ = x;
  y_ = y;
  _stopped = false;
  _grounded = false;
}

void HealthUpgrade::update(float dt, Player *player){
  Pickup::updateNoMovement(dt, player);
}
void HealthUpgrade::draw(Graphics &graphics){
  Pickup::draw(graphics);
}
void HealthUpgrade::onPickup(Player *player){
  player->showPopup("You've grown in strength!");
  player->changeMaxHP(2);
  PersistentInfo::getUpgrades()->addToHealthPickups(_map);
}

void HealthUpgrade::setMap(std::string map){
  _map = map;
}
std::string HealthUpgrade::getMap(){
  return _map;
}

HealthFlaskUpgrade::HealthFlaskUpgrade(Graphics &graphics, float x, float y) : Pickup(graphics, "sprites/healingflask.png", 1,1,x,y,32*SPRITE_SCALE,32*SPRITE_SCALE)
{
  x_ = x;
  y_ = y;
  _stopped = false;
  _grounded = false;
}

void HealthFlaskUpgrade::update(float dt, Player *player){
  Pickup::updateNoMovement(dt, player);
}
void HealthFlaskUpgrade::draw(Graphics &graphics){
  Pickup::draw(graphics);
}
void HealthFlaskUpgrade::onPickup(Player *player){
  player->showPopup("You gained another health flask!");
  player->getInventory()->addToFlaskCapacity(1);
  player->getInventory()->refillItem(flask);
  PersistentInfo::getUpgrades()->addToHealthFlaskPickups(_map);
}

void HealthFlaskUpgrade::setMap(std::string map){
  _map = map;
}

std::string HealthFlaskUpgrade::getMap(){
  return _map;
}


MessagePopup::MessagePopup(Graphics &graphics, float x, float y) : Pickup(graphics, "sprites/chargespeedupgrade.png", 1,1,x,y,64*SPRITE_SCALE,64*SPRITE_SCALE)
{
  x_ = x;
  y_ = y;
  _stopped = false;
  _grounded = false;
}

void MessagePopup::update(float dt, Player *player){
  Pickup::updateNoMovement(dt, player);
}
void MessagePopup::draw(Graphics &graphics){}
void MessagePopup::onPickup(Player *player){
  player->showPopup(message);
}

void MessagePopup::setMessage(std::string m){
  message = m;
}

