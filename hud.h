#ifndef HUD_H
#define HUD_H

#include "conf.h"
#include <vector>
#include "sprite.h"

class Player;
class HealthBar;

struct Component{
  int posX;
  int posY;
  Sprite graphic;
  
  void update(float dt, Player* player);
  void draw(Graphics &graphics);
};

struct HealthBar : Component{
  int max;
  int current;
  Sprite cover;
  
  void update(float dt, Player* player);
  
  void draw(Graphics &graphics){
    graphic.drawBar(graphics, posX, posY, max, current);
    cover.draw(graphics,posX,posY);
  }
};

struct BossHealthBar : Component{
  int max;
  int current;
  Sprite cover;
  bool isTrackingBoss;
  
  void update(float dt, Player* player);
  
  void draw(Graphics &graphics){
    if(isTrackingBoss){
      graphic.drawBar(graphics, posX, posY, max, current);
      cover.draw(graphics,posX,posY);
    }
  }
};

class HUD{
public:
  HUD();
  HUD(Graphics &graphics);
  
  void update(float dt, Player* player);
  void draw(Graphics &graphics);
  
private:
  std::vector<Component> _components;
  HealthBar _healthBar;
  BossHealthBar _bossHealthBar;
};

#endif