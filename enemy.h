#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"
#include "map.h"
#include "player.h"
#include "enemyhitbox.h"

#include <vector>

class SFXManager;

class Enemy : public AnimatedSprite{
public:
  Enemy();
  Enemy(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width);
  virtual void update(float dt, Player &player);
  virtual void draw(Graphics &graphics);
  virtual void playerCollision(Player* player) = 0;
  
  virtual void handleRightCollision(Rectangle tile);
  virtual void handleLeftCollision(Rectangle tile);
  virtual void handleUpCollision(Rectangle tile);
  virtual void handleDownCollision(Rectangle tile);
  
  void handleSlopeCollisions(std::vector<Slope> slopes, bool otherCollision);
  
  void changeHealth(int amount);
  void setDeathAnim();
  bool isPlayingDeathAnimation();
  bool isDead();
  
  void playDeathSFX(float dt);
  
  void knockBack(float direction, bool strong = false);
  
  const bool collidesWith(const Rectangle &other) const {
    return
    (getX()+cur_collider.width) >= other.getLeft() &&
    (getX()) <= other.getRight() &&
    (getY()+cur_collider.height) >= other.getTop() &&
    (getY()) <= other.getBottom();
  }
  
  const Rectangle getCollider() const{
    return Rectangle(this->getX(), this->getY(), this->getColliderWidth(), this->getColliderHeight());
    
  }
  
  const int getColliderWidth() const {return cur_collider.width;}
  const int getColliderHeight() const {return cur_collider.height;}
  
  void collidePlayer(Player* player);
  bool isInCameraRange(SDL_Rect* camera);
  bool getWithinStrikeZone(Player player, float radius);
  bool IsDamagable(float centerX);
  
  
  COLLIDER cur_collider;
  bool _colLeft, _colRight, _colDown, _colUp;
  
  std::vector<EnemyHitbox> hitboxes;
  
  void setStationary(){
    _stationary = true;
  }
  
  bool getHitTrigger();
  
  
protected:
  Graphics* _graphics;
  SFXManager _sfx;
  float _freq = 0.01;
  float _currTime = 0;
  
  int _direction;
  int _maxHealth;
  int _currentHealth;
  
  float _actionTimer;
  
  Vector2 _spawnPoint;
  
  int _dx, _dy;
  bool _grounded, _thisFrameGrounded, _dead;
  
  //animation booleans
  bool _death, _knockBack;
  
  float _knockDirection, _knockBackValue, _knockBackSlow;
  
  bool _attacking;
  
  bool _tileCollides = true;
  
  bool _stationary = false;
  bool _gotHit = false;
  
  bool _charging, _wait;
  bool _shieldUp = false;
};

class Walker : public Enemy{
public:
  Walker();
  Walker(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw (Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
  
protected:
  void setupAnimations();
};

class Spitter : public Enemy{
public:
  Spitter();
  Spitter(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw(Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
protected:
  void setupAnimations();
  
  float _reloadTime = .5;
  float _reloadTimeMax = .5;
  
  float _projectileTime = .01;
  float _projectileTimeMax = .01;
};

class SmallSpitter : public Enemy{
public:
  SmallSpitter();
  SmallSpitter(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw(Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
protected:
  void setupAnimations();
  
  float _reloadTime = .5;
  float _reloadTimeMax = .5;
  
  float _projectileTime = .01;
  float _projectileTimeMax = .01;
};

class Floater : public Enemy{
public:
  Floater();
  Floater(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw(Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
protected:
  void setupAnimations();
  
  float _reloadTime = .5;
  float _reloadTimeMax = .5;
  
  float _projectileTime = .01;
  float _projectileTimeMax = .01;
};

class SwordMan : public Enemy{
public:
  SwordMan();
  SwordMan(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw(Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
protected:
  void setupAnimations();
  
  float _reloadTime = .5;
  float _reloadTimeMax = .5;
  
  float _projectileTime = .01;
  float _projectileTimeMax = .01;
};

class SmallMage : public Enemy{
public:
  SmallMage();
  SmallMage(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw(Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
protected:
  void setupAnimations();
  
  float _reloadTime = .4;
  float _reloadTimeMax = .4;
  
  float _projectileTime = .02;
  float _projectileTimeMax = .02;
};

class Headless : public Enemy{
public:
  Headless();
  Headless(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw (Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
  
protected:
  void setupAnimations();
};


#endif