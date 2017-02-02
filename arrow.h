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

class Arrow : public AnimatedSprite{
public:
  Arrow(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width, bool right, float velY = 0);
  ~Arrow();
  
  void update(float elapsedTime);
  void draw(Graphics &graphics);
  void shoot(int x, int y);
  
  void handleCollisions(Map &map);
  void handleEnemyCollisions(Map &map);
  
  bool done;
  
  
protected:
  float _dx, _dy,_trailTime;
  float _lifetime = 100.0;
  std::vector<ghost> trail;
  
  Sprite _baseghost, _stuck;
  
  float _ghostPeriod;
  
  bool _hitEnemy = false;
  
  SFXManager sfx;
  
};


#endif
