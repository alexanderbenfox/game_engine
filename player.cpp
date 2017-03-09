#include "player.h"
#include "graphics.h"
#include "enemy.h"
#include <iostream>

Player::Player() {}

Player::Player(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width) : AnimatedSprite(graphics,filePath,startX,startY,posX,posY,height,width)
{
  srand(time(NULL));
  _dy = 0;
  _isTrackingBoss = false;
  
  hud = HUD(graphics);
  sfx = SFXManager(graphics);
  hitbox = SwordHitbox(graphics);
  inventory = new Inventory(graphics);
  _graphics = &graphics;
  
  this->setX(posX);
  this->setY(posY);
  
  this->addAnimation(.01, 1, 0, 0, "Idle", 32, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 1, 0, "StandingJump", 32, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 2, 0, "ForwardJump", 32, 64, Vector2(0,0),"main");
  this->addAnimation(.05, 3, 3, 0, "Fall", 32, 64, Vector2(0,0),"main");
  this->addAnimation(.015, 8, 6, 0, "Walk", 32, 64, Vector2(0,0),"main");
  
  this->addSpriteSheet(graphics, "sprites/shooting-sheet.png", "shooting",64,70);
  this->addAnimation(.007,7,9,0,"StandingShot",64,70,Vector2(-1,-5),"shooting");
  this->addAnimation(.006,3,13,0,"StandingWait",64,70,Vector2(-1,-5),"shooting");
  this->addAnimation(.007,7,0,0,"JumpingShot",64,70,Vector2(-1,-8),"shooting");
  
  this->addSpriteSheet(graphics, "sprites/crouching-sheet.png", "crouch",60,60);
  this->addAnimation(1,1,0,0,"Crouch",60,60,Vector2(0,4),"crouch");
  this->addAnimation(.01,6,1,0,"CrouchShot",60,60,Vector2(0,4),"crouch");
  
  this->addSpriteSheet(graphics, "sprites/attacking-sheet.png", "attacking",160,128);
  this->addAnimation(.012,6,0,0,"Attack",160,128,Vector2(-40,-48),"attacking");
  this->addAnimation(.012,7,6,0,"Attack2",160,128,Vector2(-40,-48),"attacking");
  this->addAnimation(.017,8,24,0,"Attack3",160,128,Vector2(-40,-48),"attacking");
  this->addAnimation(.02,6,13,0,"JumpAttack",160,128, Vector2(0,-75),"attacking");
  this->addAnimation(.015,5,19,0,"CrouchAttack",160,128,Vector2(0,28),"attacking");
  
  this->addAnimation(.015,1,34,0,"KnockBack",160,128,Vector2(-40,-48),"attacking");
  this->addAnimation(.02,6,35,0,"Death",160,128,Vector2(-40,-48),"attacking");
  
  this->addSpriteSheet(graphics, "sprites/rollingsheet.png", "roll", 64, 64);
  this->addAnimation(.01,11,1,0,"Dodge",64,64,Vector2(0,0),"roll");
    
  _scale = 2;
  _spriteScale = 2;
  
  COLLIDER normal = {.width = (int)(20*_scale), .height = (int)(60*_scale), .offset = Vector2(1*_scale,4*_scale)};
  colliders.push_back(normal);
  cur_collider = normal;
  
  _jumpattack = false;
  _actionTimer = 0;
  
  _knockBack = false;
  _invulnerable = false;
  _dodge = false;
  _charging = false;
  _chargeDelay = false;
  _fullCharge = false;
  _shoot = false;
  _bufferShoot = false;
  _popup = false;
  
}

void Player::update(float elapsedTime)
{
  if(PersistentInfo::getUpgrades()->getChargeSpeed()){
    _timeToCharge = .2;
  }
  else{
    _timeToCharge = .4;
  }
  
  AnimatedSprite::update(elapsedTime);  
  if(_dead){
    _chargeTime = 0;
    transparency = 255;
    this->playAnimation("Death",true);
    return;
  }
  
  float dir = _flipped ? -1 : 1;
  
  bool buffered = _bufferAttack || _bufferDodge || _bufferJump || _bufferShoot;
  
  if(_actionTimer <= 0 && buffered)
  {
    resetCurrentAnimation();
    if (_dx > 0){
      _flipped = false;
    }
    if (_dx < 0){
      _flipped = true;
    }
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
    _jumpattack = true;
    _actionTimer = .04;
    _thisFrameGrounded = false;
  }
  
  if(_thisFrameGrounded && _shoot){
    _wait = true;
    _thisFrameGrounded = false;
  }
  
  if(!_knockBack)
    setPlayerActions(elapsedTime);
  else{
    _dx = _knockBackValue * -dir;
    _knockBackValue -= 100*elapsedTime;
    this->playAnimation("KnockBack");
    _knockBackTimer-=elapsedTime;
    if(_knockBackTimer <= 0){
    //if(_grounded){
      _knockBack = false;
    }
  }
  
  if(_invulnerable){
    transparency = 150;
    _invulnerableTimer -= elapsedTime;
    if(_invulnerableTimer <= 0){
      _invulnerable = false;
    }
  }
  else{
    transparency = 255;
  }
  
  if(_charging){
    _chargeTime += elapsedTime;
    if(_chargeTime > .1 && _chargeDelay){
      sfx.addSFX(CHARGE2, this->getX()-22*_spriteScale, this->getY()-10*_spriteScale);
      _chargeDelay = false;
    }
    
    if(_chargeTime > _timeToCharge && _fullCharge){
      sfx.endSFX(CHARGE2);
      sfx.addSFX(CHARGED, this->getX()-22*_spriteScale, this->getY()-10*_spriteScale);
      sfx.addSFX(explosion, this->getX()-10*_spriteScale, this->getY()-10*_spriteScale);
      sfx.addSFX(explosion, this->getX()+10*_spriteScale, this->getY()+10*_spriteScale);
      sfx.addSFX(explosion, this->getX()-15*_spriteScale, this->getY()+10*_spriteScale);
      _fullCharge = false;
    }
  }
  
  this->setY(this->getY() + _dy*elapsedTime);
  this->setX(this->getX() + _dx*elapsedTime);
  sfx.moveSFX(CHARGE2, getX()-_oldX, getY()-_oldY);
  sfx.moveSFX(CHARGED, getX()-_oldX, getY()-_oldY);
    
  for(int i = 0; i < arrows.size(); i++){
    arrows[i].update(elapsedTime);
    if(arrows[i].done){
      arrows.erase(arrows.begin() + i);
    }
  }
  
  _oldX = this->getX();
  _oldY = this->getY();
  
  
  
  _collider = Rectangle(this->getX()+cur_collider.offset.x,this->getY()+cur_collider.offset.y,cur_collider.width, cur_collider.height);
  
}

void Player::setPlayerActions(float elapsedTime){
  float dir = _flipped ? -1 : 1;
  if(_actionTimer <= 0){
    _combo = 2;
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
    if(_currentAnimationDone && !_shoot)
      _actionTimer = 0;
    if(_shoot){
      if(arrowDelay && _actionTimer < .046){
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
          _dx = 600*dir;
        if (_combo == 0)
          this->playAnimation("Attack", true);
        else if (_combo == 1)
          this->playAnimation("Attack2", true);
        else{
          if(_actionTimer < .10)
            _dx = 800*dir;
          if (_actionTimer < .05 || _actionTimer > .1)
            _dx = 0;
          this->playAnimation("Attack3",true);
        }
      }
      if(!_grounded || _jumpattack){
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
      _dx = 2000*dir;
    }
    _actionTimer -= elapsedTime;
    if(_actionTimer <= 0)
    {
      setActionBools();
      _jumpattack = false;
      if(_dodge)
        _flipped = _storedFlipped;
      _wait = false;
    }
  }

}

void Player::updateAttachments(float dt){
  sfx.update(dt);
  hud.update(dt, this);
  hitbox.update(dt, _dx, _dy, _actionTimer);
}

void Player::draw(Graphics &graphics)
{
  if(_dead)
    SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);
  AnimatedSprite::draw(graphics,this->getX(),this->getY());
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
  _dy += (GRAVITY*dt);
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
      _actionTimer = this->getAnimationTime("CrouchShot")+.02;
    }
    else if (_grounded && !_crouched){
      _actionTimer = this->getAnimationTime("StandingShot")+.02;
    }
    else{
      _actionTimer = this->getAnimationTime("JumpingShot")+.02;
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
    if(_grounded){
      _combo += 1;
      if(_combo > 2)
        _combo = 0;
    }
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
      else if(_combo == 1)
      {
        _actionTimer = this->getAnimationTime("Attack2");
        hitbox.createHitBox(getX(), getY(), STANDING, dir);
      }
      else if (_combo == 2){
        _actionTimer = this->getAnimationTime("Attack3");
        hitbox.createHitBox(getX(), getY(),STANDING2, dir);
      }
    }
    else{
      _actionTimer = this->getAnimationTime("JumpAttack");
      hitbox.createHitBox(getX(), getY(), JUMPING, dir);
    }
    
    //_actionTimer = 100.0;
    setActionBools();
    _melee = true;
  }
}

void Player::check_crouch(bool crouch){
  if(_grounded)
    _crouched = crouch;
  else
    _crouched = false;
}

void Player::setCamera(SDL_Rect* screen, Vector2 size, bool start){
  float x = this->getX()+ _collider.getWidth()/2 - WINDOW_WIDTH/2;
  float y = this->_collider.getCenterY() +_collider.getHeight()/2 - WINDOW_HEIGHT/2;
  
  float shake_x = 0;
  float shake_y = 0;
  
  if(x < 0){
    x = 0;
  }
  if (x+WINDOW_WIDTH > (size.x*SPRITE_SCALE)){
    x = size.x*SPRITE_SCALE - WINDOW_WIDTH;
  }
  
  if(y < 0){
    y = 0;
  }
  
  if(y + WINDOW_HEIGHT > (size.y*SPRITE_SCALE)){
    y = size.y*SPRITE_SCALE - WINDOW_HEIGHT;
  }
  
  float cameraSpeed = .1;
  
  float distanceX = -screen->x + x;
  float distanceY = -screen->y + y;
  
  float moveX = screen->x + distanceX*cameraSpeed;
  float moveY = screen->y + distanceY*cameraSpeed;
  
  if(offset.active){
    shake_x = offset.offset.x;
    shake_y = offset.offset.y;
    offset.shake();
  }
  
  if(!start){
    screen->x = moveX + shake_x;
    screen->y = moveY + shake_y;
  }
  else{
    screen->x = x;
    screen->y = y;
  }
  
}

/*void Player::screenShake(){
  //intial shake
  float radius = 30.0;
  float randomAngle = rand()%360;
  Vector2 offset = Vector2( sin(randomAngle) * radius , cos(randomAngle) * radius);
  
  if(radius < 30.0){
    radius *= .9;
    randomAngle += (150 + rand()%60);
    offset = Vector2(sin(randomAngle)*radius , cos(randomAngle)*radius);
  }
}*/

bool Player::handleDoorCollisions(Map &map){
  bool atDoor = false;
  std::vector<Door> doors = map.checkDoorCollisions(_collider);
  for(int i = 0; i<doors.size(); i++){
    atDoor = true;
    transitionDoor = doors.at(i);
  }
  return atDoor;
}

void Player::handleCollisions(Map &map){
  for(int i = 0; i < arrows.size(); i++){
    arrows[i].handleCollisions(map);
  }
  hitbox.handleEnemyCollisions(map);
  
  bool gotCollision = false;
  
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
          gotCollision = true;
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
  
  std::vector<Rectangle> checkpoints = map.checkCheckPointCollisions(_collider);
  for(int i = 0; i < checkpoints.size(); i++){
    changeRevivalPoint(map.getMapName(), Vector2(checkpoints[i].getLeft(),checkpoints[i].getTop()));
  }
  handleOneWayCollisions(map);
  handleSlopeCollisions(map, gotCollision);
  handleEnemyCollisions(map);
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

void Player::handleEnemyCollisions(Map &map){
  std::vector<Enemy*> enemies = map.checkEnemyCollisions(_collider);
  for(Enemy* enemy : enemies){
    if(!_invulnerable && !enemy->isDead() && !enemy->isPlayingDeathAnimation() && !enemy->nonDamaging)
      enemy->collidePlayer(this);
  }
  
  std::vector<EnemyHitbox*> hitters = map.checkEnemyHitboxCollisions(_collider);
  for(int i = 0; i < hitters.size(); i++){
    if(!_invulnerable)
      hitters.at(i)->collidePlayer(*this);
  }
}

//void handleSlopeCollisions
//Handles collisions with ALL slopes the player is colliding with
void Player::handleSlopeCollisions(Map &map, bool otherCollision){
  std::vector<Slope> slopes = map.checkSlopeCollisions(_collider);
  for (int i = 0; i < slopes.size(); i++) {
    //Calculate where on the slope the player's bottom center is touching
    //and use y=mx+b to figure out the y position to place him at
    //First calculate "b" (slope intercept) using one of the points (b = y - mx)
    int b = (slopes.at(i).getP1().y - (slopes.at(i).getSlope() * std::abs(slopes.at(i).getP1().x)));
    
    //Now get player's center x
    int centerX = this->getX() + this->cur_collider.width/2;
    
    int bottom = slopes.at(i).getP1().y > slopes.at(i).getP2().y ? slopes.at(i).getP1().y : slopes.at(i).getP2().y;
    
    bool isAbove = (this->getY() + cur_collider.height) < (bottom-8);
    int ya = (this->getY() + cur_collider.height);
    
    //Now pass that X into the equation y = mx + b (using our newly found b and x) to get the new y position
    int newY = (slopes.at(i).getSlope() * centerX) + b-32;
    
    float slope = slopes.at(i).getSlope();
    
    bool set = false;
      
    
    int bump = 1;
    int sloper = 3;
    if(std::abs(slope) < 1){
      bump = 7;
      sloper = 1;
    }
    
    newY = newY - this->_collider.getHeight()+bump*SPRITE_SCALE;
    bool below = (newY)<=(getY()+sloper*SPRITE_SCALE);
    
    if(below)
    {
      //Re-position the player to the correct "y"
      if(isAbove && !otherCollision){
        this->setY(newY);
        set = true;
      }
      if(!isAbove && otherCollision){
        this->setY(newY);
        set = true;
      }
      if(set){
        _thisFrameGrounded = !_grounded;
        this->_grounded = true;
        _dy = (_dy > 0) ? 0 : _dy;
        _jumps = 1;
        _grounded = true;
      }
    }
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
  
  int x = getX()+10;
  if(_flipped)
    x = getX() - 32-10;
  if(_chargeTime <= _timeToCharge){
    Arrow arrow(*_graphics, "sprites/arrow.png",1 ,1, x, y, 32, 64, !_flipped);
    arrows.push_back(arrow);
  }
  else{
    Arrow arrow(*_graphics, "sprites/arrow.png",1 ,1, x, y, 32, 64, !_flipped);
    Arrow arrow2(*_graphics, "sprites/arrow.png",1 ,1, x, y, 32, 64, !_flipped, 1500.0);
    Arrow arrow3(*_graphics, "sprites/arrow.png",1 ,1, x, y, 32, 64, !_flipped,-1500.0);
    arrows.push_back(arrow);
    arrows.push_back(arrow2);
    arrows.push_back(arrow3);
    
  }
  _chargeTime = 0;
  arrowDelay = false;
}

void Player::changeHealth(int x){
  if((_hp + x) <= _hpMax){
    _hp += x;
    if(_hp <= 0){
      _dead = true;
    }
  }
  else{
    _hp = _hpMax;
  }
}

void Player::takeDamage(int x){
  if(!_dodge){
    changeHealth(-x);
    
    _gotHit = true;
    
    //set knock back
    _dy = -2000.0;
    
    _knockBackValue = 1500.0;
    
    _knockBack = true;
    _invulnerable = true;
    _invulnerableTimer = 0.2;
    _knockBackTimer = 0.07;
    
    setActionBools();
    _actionTimer = 0;
    _grounded = false;
    
    SDL_Color red = {255,0,0};
    _graphics->fillScreen(red);
  }
}

bool Player::getInvulnerable(){
  return _invulnerable;
}

bool Player::getDead(){
  return _dead;
}

bool Player::hitRegistered(){
  if (hitbox.hitRegistered() && !_screenPaused){
    _screenPaused = true;
    return true;
  }
  else return false;
  
  //return hitbox.hitRegistered();
}

void Player::reset(Map &map){
  if(_dead){
    _dead = false;
    _hp = _hpMax;
    _currentAnimationDone = false;
    _knockBack = false;
    _isTrackingBoss = false;
    _dx = 0;
    _dy = 0;
    shakeCamera(0);
    this->revive(map);
  }
}

void Player::changeRevivalPoint(std::string mapName, Vector2 location){
  this->revivalPoint.map = mapName;
  this->revivalPoint.point = location;
  inventory->refillItem(flask);
}

void Player::revive(Map &map){
  map.deleteMap();
  map = Map(revivalPoint.map, *_graphics);
  inventory->getCurrentItem()->addToInventory(5);
  this->setX(revivalPoint.point.x);
  this->setY(revivalPoint.point.y-32);
}

Inventory* Player::getInventory(){
  return inventory;
}

void Player::useItem(){
  inventory->getCurrentItem()->usePlayer(this);
}

void Player::addToInventory(ItemType type){
  inventory->addToInventory(type);
}
void Player::changeCurrency(int num){
  inventory->changeCurrency(num);
  if(num == 1){
    sfx.addSFX(drop, this->getX(), this->getY()+32*SPRITE_SCALE);
  }
}

void Player::showPopup(std::string message){
  hud.message(message);
  _popup = true;
}

void Player::disablePopup(){
  hud.disablePopup();
  _popup = false;
}

bool Player::getPopup(){
  return _popup;
}
