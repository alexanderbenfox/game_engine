#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "rectangle.h"
#include "arrow.h"
#include "sfx.h"
#include "hud.h"
#include "map.h"
#include "swordhitbox.h"
#include "inventory.h"

class Boss;

struct COLLIDER {
  int width;
  int height;
  Vector2 offset;
};

struct RevivalPoint{
  Vector2 point;
  std::string map;
};

struct CameraOffset{
  Vector2 offset;
  float radius;
  float randomAngle;
  bool active = false;
  
  CameraOffset(){
    //active = false;
  }
  
  CameraOffset(float r){
    radius = r;
    randomAngle = rand()%360;
    offset = Vector2( sin(randomAngle) * radius , cos(randomAngle) * radius);
    active = true;
  }
  
  void shake(){
    if(active){
      radius *= .9;
      randomAngle += (150 + rand()%60);
      offset = Vector2(sin(randomAngle)*radius , cos(randomAngle)*radius);
      if(radius < 1){
        active = false;
      }
    }
  }
};

class UpgradeManager{
private:
  bool _doubleJump, _glide, _airDodge;
public:
  UpgradeManager();
  
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
      _fullCharge = true;
    }
  }
  void stopCharging(){
    if(_charging){
      _charging = false;
      _chargeDelay = false;
      _fullCharge = false;
      sfx.endSFX(CHARGE2);
      sfx.endSFX(CHARGED);
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
  
  void setCamera(SDL_Rect* screen, Vector2 size, bool start = false);
  void shakeCamera(float r){
    offset = CameraOffset(r);
  }
  void changeHealth(int x);
  void takeDamage(int x);
  bool gotHit(){
    if (_gotHit){
      _gotHit = false;
      return true;
    }
    return false;
  }
  bool getInvulnerable();
  bool getDead();
  bool hitRegistered();
  
  int _hp = 10;
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
  
  void setTrackedBoss(Boss *boss){
    _trackedBoss = boss;
    _isTrackingBoss = true;
  }
  
  bool isTrackingBoss(){
    return _isTrackingBoss;
  }
  
  void bossDied(){
    _isTrackingBoss = false;
  }
  
  Boss* getTrackedBoss(){
    return _trackedBoss;
  }
  
  //revival stuff
  void changeRevivalPoint(std::string mapName, Vector2 location);
  void revive(Map &map);
  void reset(Map &map);
  
  //Inventory management stuff
  Inventory* getInventory();
  void useItem();
  void addToInventory(ItemType type);
  void changeCurrency(int num);
  
private:
  std::vector<COLLIDER> colliders;
  COLLIDER cur_collider;
  
  bool _grounded, _crouched, _shoot, _melee, _dodge, _wait, _jumpattack;
  int _combo = 0;
  bool _charging;
  float _chargeTime;
  bool _chargeDelay, _fullCharge;
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
  bool _gotHit = false;
  float _invulnerableTimer, _knockBackTimer, _knockBackValue;
  bool _knockBack, _invulnerable;
  bool _dead = false;
  
  SFXManager sfx;
  HUD hud;
  SwordHitbox hitbox;
  Graphics *_graphics;
  
  Door transitionDoor;
  
  float _oldX, _oldY;
  
  CameraOffset offset;
  
  Boss *_trackedBoss;
  bool _isTrackingBoss;
  
  RevivalPoint revivalPoint;
  Inventory* inventory;
  
};

#endif
