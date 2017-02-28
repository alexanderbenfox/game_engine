#include "enemy.h"
#include "sfx.h"

Enemy::Enemy(){}

Enemy::Enemy(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width) : AnimatedSprite(graphics,filePath,startX,startY,posX,posY,height,width), _direction(1), _maxHealth(0), _currentHealth(0)
{
  _dx = 0;
  _dy = 0;
  _damage = 1;
  
  _dead = false;
  _death = false;
  _knockBack = false;
  _actionTimer = 0;
  _knockBackSlow = 13;
  cannotBeDamaged = false;
  
  _scale = 2;
  _spriteScale = 2;
  
  _sfx = SFXManager(graphics);
  
  _graphics = &graphics;
  _shieldUp = false;
  
}
void Enemy::update(float dt, Player &player){
  AnimatedSprite::update(dt);
  _sfx.update(dt);
  bool flipped = _direction > 0 ? false : true;
  this->setFlipped(flipped);
  _collider = Rectangle(this->getX()+cur_collider.offset.x,this->getY()+cur_collider.offset.y,cur_collider.width, cur_collider.height);
  
  int i = 0;
  
  for(i = 0; i<hitboxes.size();i++){
    hitboxes.at(i).update(dt, player);
    if(hitboxes[i].finished){
      hitboxes.erase(hitboxes.begin() + i);
    }
  }
}
void Enemy::draw(Graphics &graphics){
  AnimatedSprite::draw(graphics, this->getX(), this->getY());
  _sfx.draw(graphics);
  
  for(EnemyHitbox hitbox : hitboxes){
    hitbox.draw(graphics);
  }
}

bool Enemy::getWithinStrikeZone(Player player, float radius){
  float centerX = this->getX() + cur_collider.width/2;
  bool withinStrikeZone =(player.getCollider().getCenterX() > (centerX-radius*SPRITE_SCALE)) && (player.getCollider().getCenterX() < (centerX+radius*SPRITE_SCALE));
  return withinStrikeZone;
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

void Enemy::collidePlayer(Player *player){
  player->takeDamage(_damage);
}

void Enemy::handleSlopeCollisions(std::vector<Slope> slopes, bool otherCollision){
  if(_tileCollides){
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
          _grounded = true;
        }
      }
    }
  }
}

bool Enemy::IsDamagable(float compareX){
  bool damageInFront = false;
  float centerX = this->getX() + cur_collider.width/2;
  if(compareX > centerX && _direction > 0)
    damageInFront = true;
  
  if(compareX < centerX && _direction < 0)
    damageInFront = true;
  
  if(!_shieldUp){
    return true;
  }
  else if(_shieldUp && damageInFront){
    float x = _direction > 0 ? this->getX() + cur_collider.width/2 : this->getX()-cur_collider.width/2;
    _sfx.addSFX(arrow_blast, x, this->getY());
    return false;
  }
  else{
    return true;
  }
}

void Enemy::changeHealth(int amount){
  _currentHealth += amount;
  if(amount < 0){
    _gotHit = true;
    if(_currentHealth <= 0){
      _death = true;
    }
  }
}

bool Enemy::getHitTrigger(){
  if(_gotHit){
    _gotHit = false;
    return true;
  }
  return false;
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

void Enemy::playDeathSFX(float dt){
  if(_currTime > _freq){
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
    _currTime = 0;
  }
  else{
    _currTime += dt;
  }
  
}

void Enemy::knockBack(float direction, bool strong){
  if(strong)
    _knockBackValue = 600.0;
  else
    _knockBackValue = 500.0;
  _dy = 0;
  _dy -= 1000;
  _knockBack = true;
  _knockDirection = direction;
  _actionTimer = 0.25;
}

bool Enemy::isInCameraRange(SDL_Rect *camera){
  int margin = 64*SPRITE_SCALE;
  int x1 = camera->x - margin;
  int x2 = camera->x + camera->w + margin;
  int y1 = camera->y - margin;
  int y2 = camera->y + camera->h + margin;
  
  bool inRange =
  _collider.getRight() >= x1 &&
  _collider.getLeft()<= (x2) &&
  _collider.getBottom() >= y1 &&
  _collider.getTop() <= (y2);
  
  return inRange;
}

void Enemy::dropGold(Map *map){
  for(int i = 0; i < goldDrop;i++)
  {
    Currency *c = new Currency(*_graphics, this->getCollider().getCenterX(), this->getCollider().getCenterY());
    map->addItemToMap(c);
  }
}


Walker::Walker(){}

Walker::Walker(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/walkingmonster-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,64, 64)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(28*_scale), .height = (int)(64*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 10;
  _currentHealth = _maxHealth;
  
}
void Walker::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
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
    
    _dx = (_direction) * 400.0;
  }
  else{
    if(_death){
      _knockBack = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    if(_knockBack){
      this->playAnimation("Hit");
      _dx = _knockDirection*_knockBackValue;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
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
}
void Walker::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Walker::playerCollision(Player* player){
  player->changeHealth(-_damage);
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
  _dy += (GRAVITY*dt);
}

void Walker::setupAnimations(){
  this->addAnimation(.01, 1, 0, 0, "Idle", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 1, 0, "Hit", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.04, 3, 2, 0, "Walk", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 5, 5, 0, "Attack", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.05, 7, 10, 0, "Death", 64, 64, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}


Spitter::Spitter(){}

Spitter::Spitter(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/spittingmonster-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,58, 32)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(28*_scale), .height = (int)(58*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 10;
  _currentHealth = _maxHealth;
  
}
void Spitter::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  bool playerToTheRight = (player.getCollider().getCenterX() > (this->getX()+16));
  bool playerToTheLeft = (player.getCollider().getCenterX() < (this->getX()+16));
  
  if (playerToTheRight && _direction < 0)
    _direction = 1;
  
  if (playerToTheLeft && _direction > 0)
    _direction = -1;
  
  if(_actionTimer <= 0){
    _dx = 0;
    _currentAnimationDone = false;
    this->playAnimation("Idle");
    
    _reloadTime -= dt;
    
    if(_reloadTime <= 0){
      _attacking = true;
      _actionTimer = this->getAnimationTime("Attack");
      _reloadTime = _reloadTimeMax;
    }
  }
  else{
    if(_death){
      _knockBack = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    
    if(_attacking){
      this->playAnimation("Attack",true);
      _projectileTime-=dt;
      if(_projectileTime <= 0){
        _projectileTime = _projectileTimeMax;
      }
    }
    
    if(_knockBack)
    {
      _dx = _knockDirection*_knockBackValue;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
    }
    
    _actionTimer -= dt;
    
    if(_currentAnimationDone)
      _actionTimer = 0;
    
    if(_actionTimer <= 0 && _knockBack)
    {
      _knockBack = false;
    }
    
    if(_actionTimer <= 0 && _attacking){
      float x_vect = player.getCollider().getCenterX() - _collider.getCenterX();
      float y_vect = player.getCollider().getCenterY() - this->getY();
      float mag = sqrtf(x_vect*x_vect + y_vect*y_vect);
      x_vect/=mag;
      y_vect/=mag;
      EnemyHitbox projectile = EnemyHitbox(*_graphics, "sprites/spittingmonsterproj.png", 0, 0, _collider.getCenterX(), this->getY(), 32, 32, x_vect*1700.0, y_vect*1700.0, .2, false, false);
      projectile.setDestroyable();
      hitboxes.push_back(projectile);
      _attacking = false;
    }
    
    if(_actionTimer <= 0 && _death){
      _dead = true;
    }
  }
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
}
void Spitter::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Spitter::playerCollision(Player* player){
  player->changeHealth(-1);
}

void Spitter::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void Spitter::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void Spitter::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void Spitter::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void Spitter::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void Spitter::setupAnimations(){
  this->addAnimation(.01, 1, 0, 0, "Idle", 32, 58, Vector2(0,0),"main");
  this->addAnimation(.02, 4, 1, 0, "Attack", 32, 58, Vector2(0,0),"main");
  this->addAnimation(.04, 5, 5, 0, "Death", 32, 58, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}




SmallSpitter::SmallSpitter(){}

SmallSpitter::SmallSpitter(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/smallspittingmonster-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,32, 32)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(28*_scale), .height = (int)(32*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 8;
  _currentHealth = _maxHealth;
  
}
void SmallSpitter::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  bool playerToTheRight = (player.getCollider().getCenterX() > (this->getX()+16));
  bool playerToTheLeft = (player.getCollider().getCenterX() < (this->getX()+16));
  
  bool withinShootZone =(player.getCollider().getCenterX() > (this->getCollider().getCenterX()-80*SPRITE_SCALE)) && (player.getCollider().getCenterX() < (this->getCollider().getCenterX()+80*SPRITE_SCALE));
  
  if (playerToTheRight && _direction < 0)
    _direction = 1;
  
  if (playerToTheLeft && _direction > 0)
    _direction = -1;
  
  if(!withinShootZone)
    _dx = 500*_direction;
  else _dx = 0;
  
  if(_grounded){
    _dy += -2400;
    _grounded = false;
  }
  
  if(_stationary)
    _dx = 0;
  
  if(_actionTimer <= 0){
    _currentAnimationDone = false;
    this->playAnimation("Idle");
    
    _reloadTime -= dt;
    
    if(_reloadTime <= 0){
      _attacking = true;
      _actionTimer = this->getAnimationTime("Attack");
      _reloadTime = _reloadTimeMax;
    }
  }
  else{
    if(_death){
      _attacking = false;
      _knockBack = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    
    if(_attacking){
      this->playAnimation("Attack",true);
      _projectileTime-=dt;
      if(_projectileTime <= 0){
        _projectileTime = _projectileTimeMax;
      }
    }
    
    if(_knockBack)
    {
      
      _dx = _knockDirection*_knockBackValue;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
    }
    
    _actionTimer -= dt;
    
    if(_currentAnimationDone)
      _actionTimer = 0;
    
    if(_actionTimer <= 0 && _knockBack)
    {
      _knockBack = false;
    }
    
    if(_actionTimer <= 0 && _attacking){
      EnemyHitbox projectile = EnemyHitbox(*_graphics, "sprites/spittingmonsterproj.png", 0, 0, this->getX(), this->getY(), 32, 32, _direction*1900.0, -1000, .15);
      projectile.setDestroyable();
      hitboxes.push_back(projectile);
      _attacking = false;
    }
    
    if(_actionTimer <= 0 && _death){
      _dead = true;
    }
  }
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
}
void SmallSpitter::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void SmallSpitter::playerCollision(Player* player){
  player->changeHealth(-1);
}

void SmallSpitter::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void SmallSpitter::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void SmallSpitter::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void SmallSpitter::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void SmallSpitter::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void SmallSpitter::setupAnimations(){
  this->addAnimation(.01, 1, 0, 0, "Idle", 32, 32, Vector2(0,0),"main");
  this->addAnimation(.02, 4, 1, 0, "Attack", 32, 32, Vector2(0,0),"main");
  this->addAnimation(.04, 4, 5, 0, "Death", 32, 32, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}





Floater::Floater(){}

Floater::Floater(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/floatingenemy-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,38, 38)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(38*_scale), .height = (int)(38*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 2;
  _currentHealth = _maxHealth;
  
  _dy = 0;
  
  _tileCollides = false;
  
}
void Floater::update(float dt, Player &player){
  Enemy::update(dt, player);
  Enemy::setDeathAnim();
  
  bool playerToTheRight = (player.getCollider().getCenterX() > _collider.getCenterX());
  bool playerToTheLeft = (player.getCollider().getCenterX() < (_collider.getCenterX()));
  bool playerBelow = (player.getCollider().getCenterY() > (_collider.getCenterY()));
  
  bool threshold =(player.getCollider().getCenterY() > (_collider.getCenterY()-64*SPRITE_SCALE)) && (player.getCollider().getCenterY() < (_collider.getCenterY()+64*SPRITE_SCALE));
  
  if (playerToTheRight && _direction < 0)
    _direction = 1;
  
  if (playerToTheLeft && _direction > 0)
    _direction = -1;
  
  int below = playerBelow ? 1 : -1;
  
  if(_dx > 6000)
    _direction = -1;
  if(_dx < -6000)
    _direction = 1;
  if(_dy > 6000)
    below = -1;
  if(_dy < -6000)
    below = 1;
  
  _dx += 5*_direction;
  _dy += 5*below;
  
  if(_actionTimer <= 0){
    _currentAnimationDone = false;
    this->playAnimation("Moving");
  }
  else{
    if(_death){
      _knockBack = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    
    if(_knockBack)
    {
      this->playAnimation("Idle");
      //_dx = _knockDirection*_knockBackValue;
      _dx = 0;
      _dy = 0;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
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
}
void Floater::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Floater::playerCollision(Player* player){
  player->changeHealth(-1);
}

void Floater::handleRightCollision(Rectangle tile){
  //Enemy::handleRightCollision(tile);
}

void Floater::handleLeftCollision(Rectangle tile){
  //Enemy::handleLeftCollision(tile);
}
void Floater::handleUpCollision(Rectangle tile){
  //Enemy::handleUpCollision(tile);
}
void Floater::handleDownCollision(Rectangle tile){
  //Enemy::handleDownCollision(tile);
}

void Floater::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void Floater::setupAnimations(){
  this->addAnimation(.01, 1, 0, 0, "Idle", 38, 38, Vector2(0,0),"main");
  this->addAnimation(.02, 3, 1, 0, "Moving", 38, 38, Vector2(0,0),"main");
  this->addAnimation(.04, 4, 4, 0, "Death", 38, 38, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}





SwordMan::SwordMan(){}

SwordMan::SwordMan(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/swordsman-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,96, 96)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(32*_scale), .height = (int)(64*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 15;
  _currentHealth = _maxHealth;
  
}
void SwordMan::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  bool playerToTheRight = (player.getCollider().getCenterX() > (this->getX()+16));
  bool playerToTheLeft = (player.getCollider().getCenterX() < (this->getX()+16));
  
  bool withinStrikeZone =(player.getCollider().getCenterX() > (this->getX()-132*SPRITE_SCALE)) && (player.getCollider().getCenterX() < (this->getX()+132*SPRITE_SCALE));
  
  if (playerToTheRight && _direction < 0 && !_attacking && !_wait && !_charging)
    _direction = 1;
  
  if (playerToTheLeft && _direction > 0 && !_attacking && !_wait && !_charging)
    _direction = -1;
  
  if(_stationary){
    if(withinStrikeZone){
      _triggered = true;
    }
  }
  else{
    _triggered = true;
  }
  
  if(_actionTimer <= 0 && !_attacking && !_charging && !_wait){
    _currentAnimationDone = false;
    
    if(_triggered)
    {
      this->playAnimation("Walk");
      
      _reloadTime -= dt;
      
      if(!withinStrikeZone)
        _dx = 500*_direction;
      else{
        _dx = 0;
        _charging = true;
        _actionTimer = 0.2;//this->getAnimationTime("Charge");
    }
    }
  }
  else{
    if(_charging){
      this->playAnimation("Charge");
      _dx = 0;
    }
    
    if(_attacking){
      this->playAnimation("Attack",true);
      _dx = 5000*_direction;
    }
    
    if(_wait){
      this->playAnimation("Wait",true);
      _dx = 0;
    }
    
    if(_knockBack)
    {
      _attacking = false;
      _charging = false;
      _wait = false;
      this->playAnimation("Idle");
      _dx = _knockDirection*_knockBackValue;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
    }
    if(_death){
      _attacking = false;
      _knockBack = false;
      _charging = false;
      _wait = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    
    _actionTimer -= dt;
    
    if(_currentAnimationDone && _death)
      _actionTimer = 0;
    
    if(_actionTimer <= 0 && _death){
      _dead = true;
    }
    
    if(_actionTimer <= 0 && _knockBack)
    {
      _knockBack = false;
    }
    
    if(_actionTimer <= 0 && _attacking){
      _attacking = false;
      _wait = true;
      _actionTimer = 0.05;//this->getAnimationTime("Attack");
    }
    
    if(_actionTimer <= 0 && _charging){
      _charging = false;
      _attacking = true;
      _actionTimer = 0.05;//this->getAnimationTime("Attack");
    }
    
    if(_actionTimer <= 0 && _wait){
      _wait = false;
      _actionTimer = 0;
    }
  }
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
}
void SwordMan::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void SwordMan::playerCollision(Player* player){
  player->changeHealth(-1);
}

void SwordMan::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void SwordMan::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void SwordMan::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void SwordMan::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void SwordMan::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void SwordMan::setupAnimations(){
  Vector2 offset = Vector2(0,-30);
  this->addAnimation(.01, 1, 0, 0, "Idle", 96, 96, offset,"main");
  this->addAnimation(.05, 3, 1, 0, "Walk", 96, 96, offset,"main");
  this->addAnimation(.01, 4, 4, 0, "Charge", 96, 96, offset,"main");
  this->addAnimation(.02, 3, 8, 0, "Attack", 96, 96, offset,"main");
  this->addAnimation(.02, 1, 10, 0 ,"Wait", 96,96, offset,"main");
  this->addAnimation(.04, 6, 10, 0, "Death", 96, 96, offset,"main");
  
  this->playAnimation("Idle");
}




SmallMage::SmallMage(){}

SmallMage::SmallMage(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/mage-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,32, 32)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(28*_scale), .height = (int)(32*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 8;
  _currentHealth = _maxHealth;
  
}
void SmallMage::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  bool playerToTheRight = (player.getCollider().getCenterX() > (this->getX()+16));
  bool playerToTheLeft = (player.getCollider().getCenterX() < (this->getX()+16));
  
  bool withinShootZone =(player.getCollider().getCenterX() > (this->getCollider().getCenterX()-160*SPRITE_SCALE)) && (player.getCollider().getCenterX() < (this->getCollider().getCenterX()+160*SPRITE_SCALE));
  
  if (playerToTheRight && _direction < 0)
    _direction = 1;
  
  if (playerToTheLeft && _direction > 0)
    _direction = -1;
  
  if(!withinShootZone)
    _dx = 300*_direction;
  else _dx = 0;
  
  if(_grounded){
    _dy += -3000;
    _grounded = false;
  }
  
  if(_stationary)
    _dx = 0;
  
  if(_actionTimer <= 0){
    _sfx.endSFX(CHARGE1);
    _currentAnimationDone = false;
    this->playAnimation("Idle");
    
    _reloadTime -= dt;
    
    if(_reloadTime <= 0){
      _sfx.addSFX(CHARGE1, this->getX(), this->getY());
      _attacking = true;
      _actionTimer = this->getAnimationTime("Attack");
      _reloadTime = _reloadTimeMax;
    }
  }
  else{
    if(_death){
      _sfx.endSFX(CHARGE1);
      _attacking = false;
      _knockBack = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    
    if(_attacking){
      this->playAnimation("Attack",true);
      _projectileTime-=dt;
      if(_projectileTime <= 0){
        _projectileTime = _projectileTimeMax;
      }
    }
    
    if(_knockBack)
    {
      _sfx.endSFX(CHARGE1);
      this->playAnimation("KnockBack",true);
      _dx = _knockDirection*_knockBackValue;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
    }
    
    _actionTimer -= dt;
    
    if(_currentAnimationDone)
      _actionTimer = 0;
    
    if(_actionTimer <= 0 && _knockBack)
    {
      _knockBack = false;
    }
    
    if(_actionTimer <= 0 && _attacking){
      _sfx.endSFX(CHARGE1);
      EnemyHitbox projectile = EnemyHitbox(*_graphics, "sprites/mag-proj.png", 0, 0, this->getX(), this->getY(), 16, 16, _direction*2000.0, 0, .15,true, false,3);
      projectile.setDestroyable();
      hitboxes.push_back(projectile);
      _attacking = false;
    }
    
    if(_actionTimer <= 0 && _death){
      _dead = true;
    }
  }
  
  _sfx.moveSFX(CHARGE1, _dx*dt, _dy*dt);
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
}
void SmallMage::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void SmallMage::playerCollision(Player* player){
  player->changeHealth(-1);
}

void SmallMage::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void SmallMage::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void SmallMage::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void SmallMage::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void SmallMage::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void SmallMage::setupAnimations(){
  this->addAnimation(.01, 1, 0, 0, "Idle", 32, 32, Vector2(0,0),"main");
  this->addAnimation(.02, 1, 1, 0, "KnockBack", 32, 32, Vector2(0,0),"main");
  this->addAnimation(.05, 3, 2, 0, "Attack", 32, 32, Vector2(0,0),"main");
  this->addAnimation(.04, 4, 5, 0, "Death", 32, 32, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}



Headless::Headless(){}

Headless::Headless(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/headless-monster.png", 1, 1,spawnPoint.x, spawnPoint.y ,64, 64)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(28*_scale), .height = (int)(64*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 10;
  _currentHealth = _maxHealth;
  
}
void Headless::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  if(_actionTimer <= 0){
    _shieldUp = true;
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
    
    bool withinZone = getWithinStrikeZone(player, 64);
    
    if(!withinZone){
      _dx = (_direction) * 300.0;
    }else{
      _dx = 0;
      _actionTimer = 0.2;
      _charging = true;
    }
  }
  else{
    _shieldUp = false;
    if(_death){
      _charging = false;
      _knockBack = false;
      _attacking = false;
      _wait = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    if(_knockBack){
      this->playAnimation("Hit");
      _attacking = false;
      _wait = false;
      _dx = _knockDirection*_knockBackValue;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
    }
    
    _actionTimer -= dt;
    
    if(_charging){
      _shieldUp = true;
      this->playAnimation("Charge");
      if(_actionTimer <= 0){
        _charging = false;
        _attacking = true;
        _actionTimer = getAnimationTime("Attack");
      }
    }
    
    if(_attacking){
      this->playAnimation("Attack");
      float box_pos_x = -60*SPRITE_SCALE;
      if(_direction > 0)
        box_pos_x = 23*SPRITE_SCALE;
        
      EnemyHitbox swordhitbox = EnemyHitbox(*_graphics, "", 0, 0, this->getX() + 16 *SPRITE_SCALE + box_pos_x, this->getY(), 50*SPRITE_SCALE, 32*SPRITE_SCALE, 0, 0, .1,false, false);
      hitboxes.push_back(swordhitbox);
      if(_actionTimer <= 0){
        _attacking = false;
        _wait = true;
        _actionTimer = 0.15;
      }
    }
    
    if(_wait){
      _shieldUp = false;
      this->playAnimation("Open");
      if(_actionTimer <= 0){
        _wait = false;
      }
      
    }
    
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
}
void Headless::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Headless::playerCollision(Player* player){
  player->changeHealth(-1);
}

void Headless::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void Headless::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void Headless::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void Headless::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void Headless::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void Headless::setupAnimations(){
  this->addAnimation(.01, 2, 0, 0, "Idle", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.05, 2, 2, 0, "Walk", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.04, 2, 4, 0, "Charge", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 3, 5, 0, "Attack", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.01, 1, 7, 0, "Open", 64, 64, Vector2(0,0), "main");
  this->addAnimation(.01, 1, 8, 0, "Hit", 64, 64, Vector2(0,0),"main");
  this->addAnimation(.05, 5, 9, 0, "Death", 64, 64, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}




FloatFish::FloatFish(){}

FloatFish::FloatFish(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/fishymonster.png", 1, 1,spawnPoint.x, spawnPoint.y ,32, 64)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(50*_scale), .height = (int)(28*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 1;
  _currentHealth = _maxHealth;
  
  _dy = 0;
  
  _tileCollides = false;
  
}
void FloatFish::update(float dt, Player &player){
  Enemy::update(dt, player);
  Enemy::setDeathAnim();
  
  lifetime += .05;
  
  bool playerToTheRight = (player.getCollider().getCenterX() > _collider.getCenterX());
  bool playerToTheLeft = (player.getCollider().getCenterX() < (_collider.getCenterX()));
  bool playerBelow = (player.getCollider().getCenterY() > (_collider.getCenterY()));
  
  bool threshold =(player.getCollider().getCenterY() > (_collider.getCenterY()-64*SPRITE_SCALE)) && (player.getCollider().getCenterY() < (_collider.getCenterY()+64*SPRITE_SCALE));
  
  if (playerToTheRight && _direction < 0)
    _direction = 1;
  
  if (playerToTheLeft && _direction > 0)
    _direction = -1;
  
  if(threshold)
    _triggered = true;
  
  if(_triggered){
    _dx = 700*_direction;
    _dy = (800)*sinf(lifetime);
  }
  
  if(_stationary)
    _dx = 0;
  
  if(_actionTimer <= 0){
    _currentAnimationDone = false;
    this->playAnimation("Idle");
  }
  else{
    if(_death){
      _knockBack = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    
    if(_knockBack)
    {
      this->playAnimation("Hit");
      //_dx = _knockDirection*_knockBackValue;
      _dx = 0;
      _dy = 0;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
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
}
void FloatFish::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void FloatFish::playerCollision(Player* player){
  player->changeHealth(-1);
}

void FloatFish::handleRightCollision(Rectangle tile){
  //Enemy::handleRightCollision(tile);
}

void FloatFish::handleLeftCollision(Rectangle tile){
  //Enemy::handleLeftCollision(tile);
}
void FloatFish::handleUpCollision(Rectangle tile){
  //Enemy::handleUpCollision(tile);
}
void FloatFish::handleDownCollision(Rectangle tile){
  //Enemy::handleDownCollision(tile);
}

void FloatFish::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void FloatFish::setupAnimations(){
  this->addAnimation(.03, 4, 0, 0, "Idle", 64, 32, Vector2(0,0),"main");
  this->addAnimation(.02, 1, 4, 0, "Hit", 64, 32, Vector2(0,0),"main");
  this->addAnimation(.04, 4, 5, 0, "Death", 64, 32, Vector2(0,0),"main");
  
  this->playAnimation("Idle");
}






Rat::Rat(){}

Rat::Rat(Graphics &graphics, Vector2 spawnPoint) : Enemy(graphics, "sprites/rat-sheet.png", 1, 1,spawnPoint.x, spawnPoint.y ,32, 64)
{
  _spawnPoint = Vector2(spawnPoint);
  setupAnimations();
  
  COLLIDER normal = {.width = (int)(48*_scale), .height = (int)(32*_scale), .offset = Vector2(0,0)};
  cur_collider = normal;
  
  _maxHealth = 5;
  _currentHealth = _maxHealth;
  
}
void Rat::update(float dt, Player &player){
  Enemy::update(dt, player);
  applyGravity(dt);
  Enemy::setDeathAnim();
  
  bool playerToTheRight = (player.getCollider().getCenterX() > (this->getX()+16));
  bool playerToTheLeft = (player.getCollider().getCenterX() < (this->getX()+16));
  
  bool withinStrikeZone =(player.getCollider().getCenterX() > (this->getX()-100*SPRITE_SCALE)) && (player.getCollider().getCenterX() < (this->getX()+100*SPRITE_SCALE));
  
  bool withinStrikeZone2 =(player.getCollider().getCenterX() > (this->getX()-100*SPRITE_SCALE)) && (player.getCollider().getCenterX() < (this->getX()+100*SPRITE_SCALE));
  
  if (playerToTheRight && _direction < 0 && !_attacking && !_wait && !_charging)
    _direction = 1;
  
  if (playerToTheLeft && _direction > 0 && !_attacking && !_wait && !_charging)
    _direction = -1;
  
  if(_stationary){
    if(withinStrikeZone){
      _triggered = true;
    }
  }
  else{
    _triggered = true;
  }
  
  
  if(_actionTimer <= 0 && !_attacking && !_charging && !_wait){
    _currentAnimationDone = false;
    this->playAnimation("Walk");
    
    if(_triggered){
    
      _reloadTime -= dt;
      
      if(!withinStrikeZone)
        _dx = 500*_direction;
      else{
        _dx = 0;
        _charging = true;
        _actionTimer = 0.05;//this->getAnimationTime("Charge");
      }
    }
  }
  else{
    if(_charging){
      this->playAnimation("Idle");
      _dx = 0;
    }
    
    if(_attacking){
      this->playAnimation("Leap",true);
      _dx = 1000*_direction;
    }
    
    if(_wait){
      this->playAnimation("Idle",true);
      _dx = 0;
    }
    
    if(_knockBack)
    {
      if(_attacking || _wait){
        _currentAnimationDone = false;
      }
      _attacking = false;
      _charging = false;
      _wait = false;
      _dx = _knockDirection*_knockBackValue;
      if(_knockBackValue > 0)
        _knockBackValue -= _knockBackSlow;
    }
    if(_death){
      _attacking = false;
      _knockBack = false;
      _charging = false;
      _wait = false;
      this->playAnimation("Death", true);
      this->playDeathSFX(dt);
      _dx = 0;
    }
    
    _actionTimer -= dt;
    
    if(_currentAnimationDone && _death)
      _actionTimer = 0;
    
    if(_actionTimer <= 0 && _death){
      _dead = true;
    }
    
    if(_actionTimer <= 0 && _knockBack)
    {
      _knockBack = false;
    }
    
    if(_actionTimer <= 0 && _attacking){
      _attacking = false;
      _wait = true;
      _actionTimer = 0.05;//this->getAnimationTime("Attack");
    }
    
    if(_actionTimer <= 0 && _charging){
      _charging = false;
      _attacking = true;
      _actionTimer = 0.1;//this->getAnimationTime("Attack");
      _dy -= 2000;
    }
    
    if(_actionTimer <= 0 && _wait){
      _wait = false;
      _actionTimer = 0;
    }
  }
  
  this->setY(this->getY() + _dy*dt);
  this->setX(this->getX() + _dx*dt);
}
void Rat::draw (Graphics &graphics){
  Enemy::draw(graphics);
}
void Rat::playerCollision(Player* player){
  player->changeHealth(-1);
}

void Rat::handleRightCollision(Rectangle tile){
  Enemy::handleRightCollision(tile);
}

void Rat::handleLeftCollision(Rectangle tile){
  Enemy::handleLeftCollision(tile);
}
void Rat::handleUpCollision(Rectangle tile){
  Enemy::handleUpCollision(tile);
}
void Rat::handleDownCollision(Rectangle tile){
  Enemy::handleDownCollision(tile);
}

void Rat::applyGravity(float dt){
  _dy += (GRAVITY*dt);
}

void Rat::setupAnimations(){
  Vector2 offset = Vector2(0,0);
  this->addAnimation(.01, 1, 3, 0, "Idle", 64, 32, offset,"main");
  this->addAnimation(.05, 4, 0, 0, "Walk", 64, 32, offset,"main");
  this->addAnimation(.01, 1, 2, 0, "Leap", 64, 32, offset,"main");
  this->addAnimation(.04, 4, 4, 0, "Death", 64, 32, offset,"main");
  
  this->playAnimation("Idle");
}







