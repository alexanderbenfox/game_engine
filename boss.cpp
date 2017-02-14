#include "boss.h"

Boss::Boss(){}

Boss::Boss(Graphics &graphics, Vector2 spawnPoint, std::string filePath, Vector2 size) : Enemy(graphics, filePath, 1, 1,spawnPoint.x, spawnPoint.y ,size.y, size.x)
{
  
}

void Boss::update(float dt, Player &player){
  Enemy::update(dt, player);
  if(_grounded)
    _triggered = true;
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
  
  //_maxHealth = 100;
  _maxHealth = 100;
  _currentHealth = _maxHealth;
  _direction = -1;
  
  _triggered = false;
  _wait = false;
  
  this->addSpriteSheet(graphics, "sprites/snakedeath-sheet.png", "death", 500, 350);
  
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
  this->addAnimation(.02, 5, 26, 0, "GetUp", 500, 350, offset,"main");
  
  //Animations for VenomSpitAttack
  this->addAnimation(.05, 4, 0, 0, "Ready", 500, 350, offset,"main");
  this->addAnimation(.05, 2, 3, 0, "Charge", 500, 350, offset,"main");
  this->addAnimation(.05, 4, 5, 0, "Spit", 500, 350, offset,"main");
  
  //Animations for TailStrike
  this->addAnimation(.05, 4, 8, 0, "RaiseTail", 500, 350, offset,"main");
  this->addAnimation(.02, 4, 12, 0, "WaitForStrike", 500, 350, offset,"main");
  this->addAnimation(.05, 1, 16, 0, "Striking", 500, 350, offset,"main");
  
  //Other animations
  this->addAnimation(.01, 1, 18, 0, "Idle", 500, 350, offset,"main");
  this->addAnimation(.03, 18, 0, 0, "Death", 500, 350, offset,"death");
  
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
        _dx = 700*_direction;
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
        _wait = true;
        _actionTimer = _maxWaitTime*((float)_currentHealth/(float)_maxHealth);
        playAnimation("Idle");
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
        float x = this->getX();
        if(_direction > 0)
          x += cur_collider.width;
        EnemyHitbox projectile1 = EnemyHitbox(*_graphics, "sprites/spittingmonsterproj.png", 0, 0, x, this->getY(), 20, 20, _direction*2000.0, -1800, .2);
        EnemyHitbox projectile2 = EnemyHitbox(*_graphics, "sprites/spittingmonsterproj.png", 0, 0, x, this->getY(), 20, 20, _direction*1000.0, -1600, .2);
        EnemyHitbox projectile3 = EnemyHitbox(*_graphics, "sprites/spittingmonsterproj.png", 0, 0, x, this->getY(), 20, 20, _direction*3000.0, -2200, .2);
        projectile1.setDestroyable();
        projectile2.setDestroyable();
        projectile3.setDestroyable();
        hitboxes.push_back(projectile1);
        hitboxes.push_back(projectile2);
        hitboxes.push_back(projectile3);
        _spit = false;
        _inVenomSpitAttack = false;
        _wait = true;
        _actionTimer = _maxWaitTime*((float)_currentHealth/(float)_maxHealth);
        playAnimation("Idle");
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
        float x = this->getX();
        if(_direction > 0)
          x -= 128;
        EnemyHitbox projectile = EnemyHitbox(*_graphics, "sprites/snaketail.png", 0, 0, x, this->getY()+cur_collider.height-28*SPRITE_SCALE, 34, 128, _direction*3000.0, 0, .2, true, false, 3);
        if(_direction < 0)
          projectile.flipSprite();
        hitboxes.push_back(projectile);
        
        //create the attacking tail here
      }
      else if(_striking){
        _striking = false;
        _inTailStrike = false;
        _wait = true;
        _actionTimer = _maxWaitTime*((float)_currentHealth/(float)_maxHealth);
        playAnimation("Idle");
      }
    }
  }
}

void Snake::chooseCurrentAttack(){
  if(!(_inLeapingAttack || _inTailStrike || _inVenomSpitAttack)){
    int rand = random()%3;
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
  Boss::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  bool playerToTheRight = (player.getCollider().getCenterX() > (this->getX()+16));
  bool playerToTheLeft = (player.getCollider().getCenterX() < (this->getX()+16));
  
  
  if(_triggered){
    if(!_inLeapingAttack){
      if(playerToTheRight)
        _direction = 1;
      if(playerToTheLeft)
        _direction = -1;
    }
    
    bool withinZone = getWithinStrikeZone(player, 64);
    
    if(_death){
      normalCollider();
      resetAttackBools();
      _inLeapingAttack = false;
      _inTailStrike = false;
      _inVenomSpitAttack = false;
      //this->playAnimation("Death");
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
      _dy = 0;
      if(_currentAnimationDone)
        _actionTimer = 0;
      if(_actionTimer <= 0 && _death){
        _dead = true;
      }
    }
    else{
      _currentAnimationDone = false;
      if(!_wait)
        this->chooseCurrentAttack();
    }
    
    if(_wait && _actionTimer <= 0)
      _wait = false;
    
    _actionTimer -= dt;
  }
  
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
  if(!_inLeapingAttack)
    Enemy::handleRightCollision(tile);
}

void Snake::handleLeftCollision(Rectangle tile){
  if(!_inLeapingAttack)
    Enemy::handleLeftCollision(tile);
}
void Snake::handleUpCollision(Rectangle tile){
  if(!_inLeapingAttack)
    Enemy::handleUpCollision(tile);
}
void Snake::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void Snake::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}