#ifndef HUD_H
#define HUD_H

#include "conf.h"
#include <vector>
#include "sprite.h"

struct Component{
  int posX;
  int posY;
  Sprite graphic;
  
  void update(float dt){
    graphic.update();
  }
  
  void draw(Graphics &graphics){
    graphic.draw(graphics, posX, posY);
  }
};

struct HealthBar : Component{
  int max;
  int current;
};

class HUD{
public:
  HUD();
  HUD(Graphics &graphics);
  
  void update(float dt);
  void draw(Graphics &graphics);
  
private:
  std::vector<Component> _components;
};

#endif