#include "enemy.h"

Fodder::Fodder(){}

Fodder::Fodder(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/fodderenemy-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y, 32, 40)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(32*_scale), .height = (int)(32*_scale), .offset = Vector2(4,0)};
  cur_collider = normal;
  
  _maxHealth = 1;
  _currentHealth = _maxHealth;
  
}
void Fodder::update(float dt, Player &player){
  Enemy::update(dt, player);
  
  if(_actionTimer <= 0){
    Enemy::setDeathAnim();
  }
  else{
    if(_death){
      _knockBack = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    _actionTimer -= dt;
    if(_actionTimer <= 0 && _death){
      _dead = true;
    }
  }
  
  //this->setY(this->getY() + _dy*dt);
  //this->setX(this->getX() + _dx*dt);
}
void Fodder::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Fodder::playerCollision(Player* player){
  player->changeHealth(-1);
}

void Fodder::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void Fodder::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void Fodder::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void Fodder::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void Fodder::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void Fodder::setupAnimations(){
  this->addAnimation(.1, 2, 0, 0, "Idle", 40, 32, Vector2(0,0),"main");
  this->addAnimation(.05, 4, 2, 0, "Death", 40, 32, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}


SpikeHazard::SpikeHazard(){}

SpikeHazard::SpikeHazard(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/spike-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,32, 32)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(32*_scale), .height = (int)(1*_scale), .offset = Vector2(0,31)};
  cur_collider = normal;
  
  _maxHealth = 10;
  _currentHealth = _maxHealth;
  
  facingUp = true;
  facingRight = false;
  facingDown = false;
  facingLeft = false;
  
  cannotBeDamaged = true;
  
}
void SpikeHazard::update(float dt, Player &player){
  Enemy::update(dt, player);
  
  if(facingRight)
    playAnimation("Right");
  if(facingLeft)
    playAnimation("Left");
  if(facingDown)
    playAnimation("Down");
  if(facingUp)
    playAnimation("Up");
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
}
void SpikeHazard::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void SpikeHazard::playerCollision(Player* player){
  player->changeHealth(-1);
}

void SpikeHazard::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void SpikeHazard::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void SpikeHazard::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void SpikeHazard::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void SpikeHazard::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void SpikeHazard::setupAnimations(){
  this->addAnimation(.01, 1, 0, 0, "Up", 32, 32, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 1, 0, "Down", 32, 32, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 2, 0, "Right", 32, 32, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 3, 0, "Left", 32, 32, Vector2(0,0),"main");
  
  this->playAnimation("Up");
}


FireballHazard::FireballHazard(){}

FireballHazard::FireballHazard(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/fireballhazard-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,64, 32)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(32*_scale), .height = (int)(1*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 10;
  _currentHealth = _maxHealth;
  
  cannotBeDamaged = true;
  
}
void FireballHazard::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  
  if(_grounded){
    _dy -= 5000;
    _grounded = false;
  }
  
  if(_dy < 0){
    _vflipped = false;
  }
  else{
    _vflipped = true;
  }
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
}
void FireballHazard::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void FireballHazard::playerCollision(Player* player){
  player->changeHealth(-1);
}

void FireballHazard::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void FireballHazard::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void FireballHazard::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void FireballHazard::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void FireballHazard::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void FireballHazard::setupAnimations(){
  this->addAnimation(.02, 3, 0, 0, "Up", 32, 64, Vector2(0,-32),"main");
  
  this->playAnimation("Up");
}

