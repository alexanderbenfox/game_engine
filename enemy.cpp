#include "enemy.h"
#include "sfx.h"

Enemy::Enemy(){}

Enemy::Enemy(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width) : AnimatedSprite(graphics,filePath,startX,startY,posX,posY,height,width), _direction(1), _maxHealth(0), _currentHealth(0)
{
  _dx = 0;
  _dy = 0;
  
  _dead = false;
  _death = false;
  _knockBack = false;
  _actionTimer = 0;
  
  _scale = 2;
  _spriteScale = 2;
  
  _sfx = SFXManager(graphics);
  
}
void Enemy::update(float dt, Player &player){
  AnimatedSprite::update(dt);
  _sfx.update(dt);
  bool flipped = _direction > 0 ? false : true;
  this->setFlipped(flipped);
  _collider = Rectangle(this->getX()+cur_collider.offset.x,this->getY()+cur_collider.offset.y,cur_collider.width, cur_collider.height);
}
void Enemy::draw(Graphics &graphics){
  AnimatedSprite::draw(graphics, this->getX(), this->getY());
  _sfx.draw(graphics);
}

void Enemy::handleRightCollision(Rectangle tile){
  _dx = (_dx > 0) ? 0 : _dx;
  this->setX(tile.getLeft() - _collider.getWidth()-2);
  _colRight = true;
}

void Enemy::handleLeftCollision(Rectangle tile){
  _dx = (_dx < 0) ? 0 : _dx;
  this->setX(tile.getRight()-1 + 2);
  _colLeft = true;
}
void Enemy::handleUpCollision(Rectangle tile){
  if(_dy < 0)
    _dy = 0;
  this->setY(tile.getBottom() + 0 - cur_collider.offset.y+1);
  _colUp = true;
}
void Enemy::handleDownCollision(Rectangle tile){
  _dy = (_dy > 0) ? 0 : _dy;
  _thisFrameGrounded = !_grounded;
  _grounded = true;
  this->setY(tile.getTop() - _collider.getHeight() - cur_collider.offset.y);
  _colDown = true;
}

void Enemy::changeHealth(int amount){
  _currentHealth += amount;
}

void Enemy::setDeathAnim(){
  _death = (_currentHealth <= 0) ? true : false;
  if(_death){
    _actionTimer = 100.0;
  }
}

bool Enemy::isPlayingDeathAnimation(){
  return _death;
}

bool Enemy::isDead(){
  return _dead;
}

void Enemy::playDeathSFX(){
  int x = std::ceil(rand()%(this->getColliderWidth()) + (int)(this->getX()));
  int y = std::ceil(rand()%(this->getColliderHeight()) + (int)(this->getY()));
  
  //int x = (int)(this->getX());
  //int y = (int)(this->getY());
  int sfx_no = rand()%3;
  
  if(sfx_no == 1){
    _sfx.addSFX(blood1, x-32, y-32);
  }
  else if (sfx_no == 2){
    _sfx.addSFX(blood2, x-32, y-32);
  }
  else{
    _sfx.addSFX(explosion, x-32, y-32);
  }
  
  std::cout<<x<<"!!"<<std::endl;
  
}

void Enemy::knockBack(float direction){
  _dy -= 1000;
  _knockBack = true;
  _knockDirection = direction;
  _actionTimer = 0.1;
}


Walker::Walker(){}

Walker::Walker(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/walkingmonster-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,64, 64)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(30*_scale), .height = (int)(64*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 200;
  _currentHealth = _maxHealth;
  
}
void Walker::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  std::cout<<_currentHealth<<std::endl;
  
  if(_actionTimer <= 0){
    _currentAnimationDone = false;
    this->playAnimation("Walk");
    
    if(_direction > 0 && _colRight){
      _direction = -1;
      _colRight = false;
    }
    if (_direction < 0 && _colLeft){
      _direction = 1;
      _colLeft = false;
    }
    
    bool playerToTheRight = (player.getCollider().getCenterX() > (this->getX()+16 + 100));
    bool playerToTheLeft = (player.getCollider().getCenterX() < (this->getX()+16 - 100));
    
    if (playerToTheRight && _direction < 0)
      _direction = 1;
    
    if (playerToTheLeft && _direction > 0)
      _direction = -1;
    
    _dx = (_direction) * 500.0;
  }
  else{
    if(_death){
      _knockBack = false;
      this->playAnimation("Death", true);
      this->playDeathSFX();
      _dx = 0;
    }
    if(_knockBack){
      this->playAnimation("Hit");
      _dx = _knockDirection*400.0;
    }
    _actionTimer -= dt;
    
    if(_currentAnimationDone)
      _actionTimer = 0;
    
    if(_actionTimer <= 0 && _knockBack)
    {
      _knockBack = false;
    }
    
    if(_actionTimer <= 0 && _death){
      _dead = true;
    }
  }
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
  
  std::cout<<getX()<<std::endl;
}
void Walker::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Walker::playerCollision(Player* player){
  player->changeHealth(-1);
}

void Walker::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void Walker::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void Walker::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void Walker::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void Walker::applyGravity(float dt){
  _dy += (240);
}

void Walker::setupAnimations(){
  this->addAnimation(.01, 1, 0, 0, "Idle", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 1, 0, "Hit", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.04, 3, 2, 0, "Walk", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 5, 5, 0, "Attack", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.05, 7, 10, 0, "Death", 64, 64, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}

