#ifndef ARROW_H
#define ARROW_H
#include "animatedsprite.h"
#include <vector>
#include <stdlib.h>
#include "sfx.h"
#include "map.h"

struct ghost{
  int x, y;
  float lifetime;
};

struct ArrowSprites{
  Sprite *stuckR;
  Sprite *ghostR;
  Sprite *stuckL;
  Sprite *ghostL;
  
  AnimatedSprite *arrowR;
  AnimatedSprite *arrowL;
  
  ArrowSprites(){}
  ArrowSprites(Graphics &graphics){
    stuckR = new Sprite(graphics,"sprites/arrowhalf.png",0,0,1,1,32,32);
    ghostR = new Sprite(graphics, "sprites/arrowtrail.png", 0,0,1,1,64,32);
    arrowR = new AnimatedSprite(graphics, "sprites/arrow.png",1 ,1, 0, 0, 32, 64);
    
    
    stuckL = new Sprite(graphics,"sprites/arrowhalf.png",0,0,1,1,32,32);
    stuckL->setFlipped(true);
    ghostL = new Sprite(graphics, "sprites/arrowtrail.png", 0,0,1,1,64,32);
    ghostL->setFlipped(true);
    arrowL =new AnimatedSprite(graphics, "sprites/arrow.png",1 ,1, 0, 0, 32, 64);
    arrowL ->setFlipped(true);
  }
  
};

class Arrow {
public:
  Arrow(Graphics &graphics, int startX, int startY, bool right, ArrowSprites a, float velY = 0);
  ~Arrow();
  
  void update(float elapsedTime);
  void draw(Graphics &graphics);
  void shoot(int x, int y);
  
  void handleCollisions(Map &map);
  void handleEnemyCollisions(Map &map);
  
  bool done;
  
  
protected:
  float _dx,_dy, _trailTime;
  float _x, _y;
  float _lifetime = 100.0;
  std::vector<ghost> trail;
  
  Sprite* _baseghost, *_stuck;
  AnimatedSprite* theArrow;
  
  float _ghostPeriod;
  
  bool _hitEnemy = false;
  bool _hitWall = false;
  
  SFXManager sfx;
  
  Vector2 stuckPos;
  Rectangle _collider;
  
};


#endif
