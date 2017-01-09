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
  void updateAttachments(float dt);
  void draw(Graphics &graphics);
  
  void moveSprite(float x, float y);
  
  void applyGravity(float dt);
  
  void handleCollisions(Map &map);
  void handleOneWayCollisions(Map &map);
  void handleSlopeCollisions(Map &map);
  
  void jump();
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
  bool hitRegistered();
  
private:
  int _hp = 10;
  
  std::vector<COLLIDER> colliders;
  COLLIDER cur_collider;
  
  bool _grounded, _crouched, _shoot, _melee, _dodge, _wait, _jumpattack;
  int _combo = 0;
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
  
  SFXManager sfx;
  HUD hud;
  SwordHitbox hitbox;
  Graphics *_graphics;
  
};

#endif
