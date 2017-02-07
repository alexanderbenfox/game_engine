#include "boss.h"

Boss::Boss(){}

Boss::Boss(Graphics &graphics, Vector2 spawnPoint, std::string filePath, Vector2 size) : Enemy(graphics, filePath, 1, 1,spawnPoint.x, spawnPoint.y ,size.y, size.x)
{
  
}

void Boss::update(float dt, Player &player){
  Enemy::update(dt, player);
}
void Boss::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Boss::playerCollision(Player* player){
  player->takeDamage(0);
}

void Boss::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void Boss::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void Boss::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void Boss::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void Boss::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void Boss::setupAnimations() {}




Snake::Snake(){}
Snake::Snake(Graphics &graphics, Vector2 spawnPoint, std::string filePath, Vector2 size) : Boss(graphics, spawnPoint, filePath, size)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(70*_scale), .height = (int)(128*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 100;
  _currentHealth = _maxHealth;
  _direction = -1;
  
}

void Snake::normalCollider(){
  COLLIDER normal = {.width = (int)(70*_scale), .height = (int)(128*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
}

void Snake::changeCollider(int x, int y, int width, int height){
  COLLIDER bew = {.width = (int)(width*_scale), .height = (int)(height*_scale), .offset = Vector2(x,y)};
  cur_collider = bew;
}

void Snake::setupAnimations(){
  Vector2 offset = Vector2(-196,-173);
  
  //Animations for the leaping attack
  this->addAnimation(.05, 3, 18, 0, "Jump", 500, 350, offset,"main");
  this->addAnimation(.05, 1, 21, 0, "LeapAscend", 500, 350, offset,"main");
  this->addAnimation(.01, 1, 22, 0, "LeapPeak", 500, 350, offset,"main");
  this->addAnimation(.01, 1, 23, 0, "Fall", 500, 350, offset,"main");
  this->addAnimation(.1, 1, 25, 0, "Layout", 500, 350, offset,"main");
  this->addAnimation(.05, 5, 26, 0, "GetUp", 500, 350, offset,"main");
  
  //Animations for VenomSpitAttack
  this->addAnimation(.05, 4, 0, 0, "Ready", 500, 350, offset,"main");
  this->addAnimation(.05, 2, 3, 0, "Charge", 500, 350, offset,"main");
  this->addAnimation(.05, 4, 5, 0, "Spit", 500, 350, offset,"main");
  
  //Animations for TailStrike
  this->addAnimation(.05, 4, 8, 0, "RaiseTail", 500, 350, offset,"main");
  this->addAnimation(.05, 4, 12, 0, "WaitForStrike", 500, 350, offset,"main");
  this->addAnimation(.05, 1, 16, 0, "Striking", 500, 350, offset,"main");
  
  //Other animations
  this->addAnimation(.01, 1, 18, 0, "Idle", 500, 350, offset,"main");
  this->addAnimation(.04, 1, 18, 0, "Death", 500, 350, offset,"main");
  
  this->playAnimation("Idle");
  
}

void Snake::LeapingAttack(){
  if(startLeapingAttack()){
    resetAttackBools();
    _jump = true;
    _actionTimer = this->getAnimationTime("Jump");
    playAnimation("Jump");
  }
  else{
    if(_actionTimer >= 0){
      //switch to leap peak here
      if(_leapAscend){
        if(_dy > -500){
          _actionTimer = 0;
        }
      }
      if(_leapPeak){
        if(_dy > 0){
          _actionTimer = 0;
        }
      }
      if(_fall){
        if(_grounded){
          _actionTimer = 0;
        }
      }
    }
    else{
      if(_jump){
        _dy -= 4000;
        _dx = 1000*_direction;
        _jump = false;
        _leapAscend = true;
        this->playAnimation("LeapAscend");
        _actionTimer = 100;
        //change collider here?
      }
      else if(_leapAscend){
        changeCollider(-64, 0, 250, 64);
        _leapAscend = false;
        _leapPeak = true;
        this->playAnimation("LeapPeak");
        _actionTimer = 100;
      }
      else if(_leapPeak){
        _grounded = false;
        _leapPeak = false;
        _fall = true;
        this->playAnimation("Fall");
        _actionTimer = 100;
      }
      else if(_fall){
        _dx = 0;
        _fall = false;
        _layout = true;
        this->playAnimation("Layout");
        //change collider here
        _actionTimer = this->getAnimationTime("Layout");
      }
      else if(_layout){
        normalCollider();
        _dx = 0;
        _layout = false;
        _getup = true;
        this->playAnimation("GetUp");
        //change collider again here
        _actionTimer = this->getAnimationTime("GetUp");
      }
      else if(_getup){
        _getup = false;
        _inLeapingAttack = false;
      }
    }
  }
}

void Snake::VenomSpitAttack(){
  if(startVenomSpitAttack()){
    _dx = 0;
    resetAttackBools();
    _ready = true;
    _actionTimer = this->getAnimationTime("Ready");
    playAnimation("Ready");
  }
  else{
    if(_actionTimer >= 0){
    }
    else{
      if(_ready){
        _ready = false;
        _charge = true;
        this->playAnimation("Charge");
        _actionTimer = 0.1;
        //change collider here?
      }
      else if(_charge){
        _charge = false;
        _spit = true;
        this->playAnimation("Spit");
        _actionTimer = this->getAnimationTime("Spit");
        
        //create the spit balls here
      }
      else if(_spit){
        _spit = false;
        _inVenomSpitAttack = false;
      }
    }
  }
}

void Snake::TailStrike(){
  if(startTailStrikeAttack()){
    _dx = 0;
    resetAttackBools();
    _raiseTail = true;
    _actionTimer = this->getAnimationTime("RaiseTail");
    playAnimation("RaiseTail");
  }
  else{
    if(_actionTimer >= 0){
    }
    else{
      if(_raiseTail){
        _raiseTail = false;
        _waitForStrike = true;
        this->playAnimation("WaitForStrike");
        _actionTimer = this->getAnimationTime("WaitForStrike");
        //change collider here?
      }
      else if(_waitForStrike){
        _waitForStrike = false;
        _striking = true;
        this->playAnimation("Striking");
        _actionTimer = .2;
        
        //create the attacking tail here
      }
      else if(_striking){
        _striking = false;
        _inTailStrike = false;
      }
    }
  }
}

void Snake::chooseCurrentAttack(){
  if(!(_inLeapingAttack || _inTailStrike || _inVenomSpitAttack)){
    int rand = random()%3;
    std::cout<<rand<<std::endl;
    if (rand == 0)
      _inLeapingAttack = true;
    if (rand == 1)
      _inTailStrike = true;
    if (rand == 2)
      _inVenomSpitAttack = true;
  }
  else{
    if(_inLeapingAttack)
      this->LeapingAttack();
    if(_inTailStrike)
      this->TailStrike();
    if(_inVenomSpitAttack)
      this->VenomSpitAttack();
  }
}

void Snake::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  bool playerToTheRight = (player.getCollider().getCenterX() > (this->getX()+16));
  bool playerToTheLeft = (player.getCollider().getCenterX() < (this->getX()+16));
  
  if(!_inLeapingAttack){
    if(playerToTheRight)
      _direction = 1;
    if(playerToTheLeft)
      _direction = -1;
  }
  
  bool withinZone = getWithinStrikeZone(player, 64);
  
  this->chooseCurrentAttack();
  
  _actionTimer -= dt;
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
}

void Snake::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Snake::playerCollision(Player* player){
  player->changeHealth(-1);
}

void Snake::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void Snake::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void Snake::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void Snake::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void Snake::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}