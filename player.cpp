#include "player.h"
#include "graphics.h"
#include <iostream>

Player::Player() {}

Player::Player(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width) : AnimatedSprite(graphics,filePath,startX,startY,posX,posY,height,width)
{
  _dy = 0;
  
  hud = HUD(graphics);
  sfx = SFXManager(graphics);
  hitbox = SwordHitbox(graphics);
  _graphics = &graphics;
  
  this->addAnimation(.01, 1, 0, 0, "Idle", 32, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 1, 0, "StandingJump", 32, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 2, 0, "ForwardJump", 32, 64, Vector2(0,0),"main");
  this->addAnimation(.05, 3, 3, 0, "Fall", 32, 64, Vector2(0,0),"main");
  this->addAnimation(.015, 8, 6, 0, "Walk", 32, 64, Vector2(0,0),"main");
  
  this->addSpriteSheet(graphics, "sprites/shooting-sheet.png", "shooting",64,70);
  this->addAnimation(.01,7,9,0,"StandingShot",64,70,Vector2(-1,-5),"shooting");
  this->addAnimation(.006,3,13,0,"StandingWait",64,70,Vector2(-1,-5),"shooting");
  this->addAnimation(.0095,7,0,0,"JumpingShot",64,70,Vector2(-1,-8),"shooting");
  
  this->addSpriteSheet(graphics, "sprites/crouching-sheet.png", "crouch",60,60);
  this->addAnimation(1,1,0,0,"Crouch",60,60,Vector2(0,4),"crouch");
  this->addAnimation(.012,6,1,0,"CrouchShot",60,60,Vector2(0,4),"crouch");
  
  this->addSpriteSheet(graphics, "sprites/attacking-sheet.png", "attacking",160,128);
  this->addAnimation(.015,6,0,0,"Attack",160,128,Vector2(-40,-48),"attacking");
  this->addAnimation(.015,7,6,0,"Attack2",160,128,Vector2(-40,-48),"attacking");
  this->addAnimation(.02,6,13,0,"JumpAttack",160,128, Vector2(0,-75),"attacking");
  this->addAnimation(.015,5,19,0,"CrouchAttack",160,128,Vector2(0,28),"attacking");
  
  this->addSpriteSheet(graphics, "sprites/rollingsheet.png", "roll", 64, 64);
  this->addAnimation(.01,11,1,0,"Dodge",64,64,Vector2(0,0),"roll");
    
  _scale = 2;
  _spriteScale = 2;
  
  COLLIDER normal = {.width = (int)(30*_scale), .height = (int)(60*_scale), .offset = Vector2(1*_scale,4*_scale)};
  colliders.push_back(normal);
  cur_collider = normal;
  
  _jumpattack = false;
  _actionTimer = 0;
  
}

void Player::update(float elapsedTime)
{
  AnimatedSprite::update(elapsedTime);
  float dir = _flipped ? -1 : 1;
  
  bool buffered = _bufferAttack || _bufferDodge || _bufferJump || _bufferShoot;
  
  if(_actionTimer <= 0 && buffered)
  {
    resetCurrentAnimation();
    if(_bufferJump)
      jump();
    if(_bufferAttack)
      attack();
    if(_bufferDodge)
      dodge();
    if(_bufferShoot)
      shoot();
  }

  
  if(_thisFrameGrounded && _melee && !_shoot){
    //_jumpattack = true;
    _actionTimer = 0;
    _thisFrameGrounded = false;
  }
  
  if(_thisFrameGrounded && _shoot){
    _wait = true;
    _thisFrameGrounded = false;
  }
  
  if(_actionTimer <= 0){
    _jumpattack = false;
    if(_dy < 0 && _dx == 0){
      this->playAnimation("StandingJump");
    }
    if(_dy < 0 && _dx != 0){
      this->playAnimation("ForwardJump");
    }
    if(_dy > 0){
      this->playAnimation("Fall");
    }
    if(_dy == 0 && _dx == 0){
      if (!_crouched)
        this->playAnimation("Idle");
      else
        this->playAnimation("Crouch");
    }
    if(_dy == 0 && _dx != 0){
      this->playAnimation("Walk");
    }
    if (_dx > 0){
      _flipped = false;
    }
    if (_dx < 0){
      _flipped = true;
    }
  }
  else{
    if(_currentAnimationDone)
      _actionTimer = 0;
    if(_shoot){
      if(arrowDelay && _actionTimer < .025){
        createArrow();
      }
      if(_grounded && !_crouched){
        _dx = 0;
        if(!_wait){
          this->playAnimation("StandingShot", true);
        }else
        {
          this->playAnimation("StandingWait",true);
        }
      }else if(_grounded && _crouched){
        _dx = 0;
        this->playAnimation("CrouchShot", true);
      }else{
        this->playAnimation("JumpingShot", true);
      }
    }
    if(_melee){
      if(_grounded && !_crouched && !_jumpattack){
        _dx = 0;
        if (_actionTimer > .025)
          _dx = 500*dir;
        if (_combo == 0)
          this->playAnimation("Attack", true);
        else
          this->playAnimation("Attack2", true);
      }
      if(!_grounded || _jumpattack){
        if(_grounded){
          _dx = 0;
        }
        this->playAnimation("JumpAttack", true);
      }
      if(_grounded && _crouched){
        _dx = 0;
        this->playAnimation("CrouchAttack", true);
      }
    }
    if(_dodge){
      _storedFlipped = (_dx > 0) ? false : true;
      _storedFlipped = (_dx == 0) ? _flipped : _storedFlipped;
      this->playAnimation("Dodge", true);
      _dx = 1800*dir;
    }
    _actionTimer -= elapsedTime;
    if(_actionTimer <= 0)
    {
      _jumpattack = false;
      if(_dodge)
        _flipped = _storedFlipped;
      _wait = false;
    }
  }
  
  
  this->setY(this->getY() + _dy*elapsedTime);
  this->setX(this->getX() + _dx*elapsedTime);
    
  for(int i = 0; i < arrows.size(); i++){
    arrows[i].update(elapsedTime);
    if(arrows[i].done){
      arrows.erase(arrows.begin() + i);
    }
  }
  
  
  
  _collider = Rectangle(this->getX()+cur_collider.offset.x,this->getY()+cur_collider.offset.y,cur_collider.width, cur_collider.height);
  
}

void Player::updateAttachments(float dt){
  sfx.update(dt);
  hud.update(dt);
  if (_actionTimer > 0)
    hitbox.update(dt, _dx, _dy);
}

void Player::draw(Graphics &graphics)
{
  AnimatedSprite::draw(graphics,this->getX(),this->getY());
  //SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 255);
  //SDL_RenderDrawLine(graphics.getRenderer(), this->_collider.getLeft(),this->_collider.getTop(),this->_collider.getLeft()+_sourceRect.w,this->_collider.getTop()+_sourceRect.h);
  
  for(int i = 0; i < arrows.size(); i++){
    arrows[i].draw(graphics);
  }
  
  sfx.draw(graphics);
  hud.draw(graphics);
  hitbox.draw(graphics);
}

void Player::moveSprite(float x, float y){
  _dx = x;
}

void Player::applyGravity(float dt)
{
  _dy += (240);
}

void Player::dropDown(){
  _dropOneWay = true;
}

void Player::jump(){
  if(_actionTimer > 0){
    resetBuffer();
    _bufferJump = true;
  }
  if(_grounded || _jumps > 0){
    _bufferJump = false;
    //if(!_grounded)
    sfx.addSFX(DUST, this->getX(), this->getY()+32*_spriteScale);
    if(!_grounded){
      _jumps--;
      _dy = -2000;
    }
    else{
      _grounded = false;
      _dy = -2600;
    }
  }
}

void Player::dodge(){
  if(_actionTimer > 0){
    resetBuffer();
    _bufferDodge = true;
    //sfx.addSFX(DUST, this->getX(), this->getY()+32*_spriteScale);
  }
  else if(_actionTimer <= 0 && _grounded)
  {
    sfx.addSFX(DUST, this->getX(), this->getY()+32*_spriteScale);
    _currentAnimationDone = false;
    _bufferDodge = false;
    _actionTimer = 1.0;
    //_actionTimer = this->getAnimationTime("Dodge");
    setActionBools();
    _dodge = true;
  }
}

void Player::setActionBools(){
  _melee = false;
  _shoot = false;
  _dodge = false;
}

void Player::shoot(){
  _currentAnimationDone = false;
  if(_actionTimer > 0){
    resetBuffer();
    _bufferShoot = true;
  }
  else{
    _bufferShoot = false;
    arrowDelay = true;
    //_currentAnimationDone = false;
    
    if(_grounded && _crouched){
      _actionTimer = this->getAnimationTime("CrouchShot");
    }
    else if (_grounded && !_crouched){
      _actionTimer = this->getAnimationTime("StandingShot");
    }
    else{
      _actionTimer = this->getAnimationTime("JumpingShot");
    }
    //_actionTimer = 1.0;
    setActionBools();
    _shoot = true;
  }
}

void Player::attack(){
  float dir = _flipped ? -1 : 1;
  if(_actionTimer > 0){
    resetBuffer();
    _bufferAttack = true;
  }
  else{
    _screenPaused = false;
    _bufferAttack = false;
    _currentAnimationDone = false;
    if(_grounded && _crouched){
      _actionTimer = this->getAnimationTime("CrouchAttack");
      hitbox.createHitBox(getX(), getY(), CROUCHING, dir);
    }
    else if (_grounded && !_crouched){
      if (_combo == 0)
      {
        _actionTimer = this->getAnimationTime("Attack");
        hitbox.createHitBox(getX(), getY(), STANDING, dir);
      }
      else
      {
        _actionTimer = this->getAnimationTime("Attack2");
        hitbox.createHitBox(getX(), getY(), STANDING, dir);
      }
    }
    else{
      _actionTimer = this->getAnimationTime("JumpAttack");
      hitbox.createHitBox(getX(), getY(), JUMPING, dir);
    }
    
    //_actionTimer = 100.0;
    setActionBools();
    _melee = true;
    _combo = _combo == 0 ? 1 : 0;
  }
}

void Player::check_crouch(bool crouch){
  if(_grounded)
    _crouched = crouch;
  else
    _crouched = false;
}

void Player::setCamera(SDL_Rect* screen, Map map){
  int x = this->getX()+ _collider.getWidth()/2 - WINDOW_WIDTH/2;
  int y = this->_collider.getCenterY() +_collider.getHeight()/2 - WINDOW_HEIGHT/2;
  
  if(x < 0){
    x = 0;
  }
  if (x+WINDOW_WIDTH > (map.getSize().x*SPRITE_SCALE)){
    x = map.getSize().x*SPRITE_SCALE - WINDOW_WIDTH;
  }
  
  if(y < 0){
    y = 0;
  }
  
  if(y + WINDOW_HEIGHT > (map.getSize().y*SPRITE_SCALE)){
    y = map.getSize().y*SPRITE_SCALE - WINDOW_HEIGHT;
  }
  
  screen->x = x;
  screen->y = y;
  
}

void Player::handleCollisions(Map &map){
  for(int i = 0; i < arrows.size(); i++){
    arrows[i].handleCollisions(map);
  }
  hitbox.handleEnemyCollisions(map);
  
  std::vector<Rectangle> tiles = map.checkTileCollisions(_collider);
  for (Rectangle tile : tiles){
    collision_sides::Side side = Sprite::getCollisionSide(tile);
    if (side != collision_sides::NONE){
      switch(side){
        case(collision_sides::RIGHT) :
          _dx = (_dx > 0) ? 0 : _dx;
          this->setX(tile.getLeft() - _collider.getWidth()-2);
          std::cout<<"RIGHT"<<std::endl;
          break;
        case(collision_sides::LEFT) :
          _dx = (_dx < 0) ? 0 : _dx;
          this->setX(tile.getRight()-1 + 2);
          std::cout<<"LEFT"<<std::endl;
          break;
        case(collision_sides::TOP) :
          if(_dy < 0)
            _dy = 0;
          this->setY(tile.getBottom() + 0 -cur_collider.offset.y+1);
          std::cout<<"TOP"<<std::endl;
          break;
        case(collision_sides::BOTTOM) :
          _dy = (_dy > 0) ? 0 : _dy;
          _jumps = 1;
          //std::cout<<"BOTTOM"<<std::endl;
          _thisFrameGrounded = !_grounded;
          _grounded = true;
          this->setY(tile.getTop() - _collider.getHeight() - cur_collider.offset.y);
          break;
      }
    }
  }
  handleOneWayCollisions(map);
  handleSlopeCollisions(map);
}

void Player::handleOneWayCollisions(Map &map){
  std::vector<Rectangle> tiles = map.checkOneWayCollisions(_collider);
  bool touchingOneWay = false;
  for (Rectangle tile : tiles){
    collision_sides::Side side = Sprite::getCollisionSide(tile);
    if (side != collision_sides::NONE){
      switch(side){
        case(collision_sides::RIGHT) :
          break;
        case(collision_sides::LEFT) :
          break;
        case(collision_sides::TOP) :
          break;
        case(collision_sides::BOTTOM) :
          touchingOneWay = true;
          if(_dy >= 0 && !_dropOneWay){
            _dy = (_dy > 0) ? 0 : _dy;
            _jumps = 1;
            _thisFrameGrounded = !_grounded;
            _grounded = true;
            this->setY(tile.getTop() - _collider.getHeight() - cur_collider.offset.y);
          }
          break;
      }
    }
  }
  if (_dropOneWay){
    _dropOneWay = touchingOneWay;
  }
}

//void handleSlopeCollisions
//Handles collisions with ALL slopes the player is colliding with
void Player::handleSlopeCollisions(Map &map){
  std::vector<Slope> slopes = map.checkSlopeCollisions(_collider);
  for (int i = 0; i < slopes.size(); i++) {
    std::cout<<slopes.at(i).getSlope()<<std::endl;
    //Calculate where on the slope the player's bottom center is touching
    //and use y=mx+b to figure out the y position to place him at
    //First calculate "b" (slope intercept) using one of the points (b = y - mx)
    int b = (slopes.at(i).getP1().y - (slopes.at(i).getSlope() * std::abs(slopes.at(i).getP1().x)));
    
    //Now get player's center x
    int centerX = this->_collider.getCenterX();
    
    //Now pass that X into the equation y = mx + b (using our newly found b and x) to get the new y position
    int newY = (slopes.at(i).getSlope() * centerX) + b-32;
    
    //Re-position the player to the correct "y"
    this->setY(newY - this->_collider.getHeight());
    this->_grounded = true;
  }
}

void Player::resetBuffer(){
  _bufferShoot = false;
  _bufferJump = false;
  _bufferDodge = false;
  _bufferAttack = false;
}

void Player::createArrow(){
  int y = getY()+6;
  if (_crouched)
    y+=54;
  
  int x = getX();
  if(_flipped)
    x = getX() - 32;
  Arrow arrow(*_graphics, "sprites/arrow.png",1 ,1, x, y, 32, 64, !_flipped);
  arrows.push_back(arrow);
  arrowDelay = false;
}

void Player::changeHealth(int x){
  _hp += x;
}

bool Player::hitRegistered(){
  if (hitbox.hitRegistered() && !_screenPaused){
    _screenPaused = true;
    return true;
  }
  else return false;
  
  //return hitbox.hitRegistered();
};