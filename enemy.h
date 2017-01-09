#ifndef ENEMY_H
#define ENEMY_H
#include "animatedsprite.h"
#include "map.h"
#include "player.h"

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
  
  void changeHealth(int amount);
  void setDeathAnim();
  bool isPlayingDeathAnimation();
  bool isDead();
  
  void playDeathSFX();
  
  void knockBack(float direction);
  
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
  
  
  COLLIDER cur_collider;
  bool _colLeft, _colRight, _colDown, _colUp;
  
protected:
  SFXManager _sfx;
  int _direction;
  int _maxHealth;
  int _currentHealth;
  
  float _actionTimer;
  
  Vector2 _spawnPoint;
  
  int _dx, _dy;
  bool _grounded, _thisFrameGrounded, _dead;
  
  //animation booleans
  bool _death, _knockBack;
  
  float _knockDirection;
  
  
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


#endif