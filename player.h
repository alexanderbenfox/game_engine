#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "rectangle.h"
#include "arrow.h"
#include "sfx.h"
#include "hud.h"
#include "map.h"
#include "swordhitbox.h"

struct COLLIDER {
  int width;
  int height;
  Vector2 offset;
};

class Player : public AnimatedSprite{
public:
  Player();
  Player(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width);
  
  void update(float elapsedTime);
  void setPlayerActions(float elapsedTime);
  void updateAttachments(float dt);
  void draw(Graphics &graphics);
  
  void moveSprite(float x, float y);
  
  void applyGravity(float dt);
  
  bool handleDoorCollisions(Map &map);
  void handleCollisions(Map &map);
  void handleOneWayCollisions(Map &map);
  void handleSlopeCollisions(Map &map, bool otherCollision);
  void handleEnemyCollisions(Map &map);
  
  void jump();
  void chargeShot(){
    if (!_charging){
      _charging = true;
      _chargeDelay = true;
    }
  }
  void stopCharging(){
    if(_charging){
      _charging = false;
      sfx.endSFX(CHARGE2);
    }
  }
  void shoot();
  void dodge();
  void check_crouch(bool crouch);
  void dropDown();
  void attack();
  
  void setActionBools();
  
  float _dx,_dy;
  
  std::vector<Arrow> arrows;
  
  void setCamera(SDL_Rect* screen, Map map);
  void changeHealth(int x);
  void takeDamage(int x);
  bool getInvulnerable();
  bool getDead();
  bool hitRegistered();
  
  void reset();
  
  int _hp = 1;
  int _hpMax = 10;
  
  bool touchedMovingPlatform = false;
  void movingPlatformStuff(Rectangle tile, float dx){
    if(!touchedMovingPlatform){
      _dy = (_dy > 0) ? 0 : _dy;
      _jumps = 1;
      _thisFrameGrounded = !_grounded;
      _grounded = true;
      this->setY(tile.getTop() - _collider.getHeight() - cur_collider.offset.y);
      this->setX(this->getX() + dx);
    }
  }
  
  Door getNewRoom(){
    return transitionDoor;
  }
  
private:
  std::vector<COLLIDER> colliders;
  COLLIDER cur_collider;
  
  bool _grounded, _crouched, _shoot, _melee, _dodge, _wait, _jumpattack;
  int _combo = 0;
  bool _charging;
  float _chargeTime;
  bool _chargeDelay;
  void createArrow();
  bool arrowDelay = false;
  
  bool _bufferDodge, _bufferAttack, _bufferShoot, _bufferJump;
  void resetBuffer();
  bool _storedFlipped;
  
  bool _thisFrameGrounded;
  bool _dropOneWay;
  bool _screenPaused;
  
  float _actionTimer;
  
  int _jumps = 1;
  
  //damage booleans
  float _invulnerableTimer, _knockBackTimer, _knockBackValue;
  bool _knockBack, _invulnerable;
  bool _dead = false;
  
  SFXManager sfx;
  HUD hud;
  SwordHitbox hitbox;
  Graphics *_graphics;
  
  Door transitionDoor;
  
  float _oldX, _oldY;
  
};

#endif
