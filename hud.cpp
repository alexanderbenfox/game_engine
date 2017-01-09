#include "hud.h"

HUD::HUD(){
  
}

HUD::HUD(Graphics &graphics){
  HealthBar hp;
  hp.posX = 0;
  hp.posY = 0;
  hp.graphic = Sprite(graphics, "sprites/healthbar.png", 0,0,1,1,128,32);
  hp.graphic.setHUD();
  hp.max = 5;
  hp.current = 5;
  
  _components.push_back(hp);
}

void HUD::update(float dt){
  for (Component comp : _components){
    comp.update(dt);
  }
  
}

void HUD::draw(Graphics &graphics){
  for (Component comp : _components){
    comp.draw(graphics);
  }
  
}