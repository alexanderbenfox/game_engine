#include "hud.h"
#include "player.h"

void HealthBar::update(float dt, Player *player){
  graphic.update();
  max = player->_hpMax;
  current = player->_hp;
}

HUD::HUD(){
  
}

HUD::HUD(Graphics &graphics){
  HealthBar hp;
  hp.posX = 32;
  hp.posY = 0;
  hp.graphic = Sprite(graphics, "sprites/healthbar.png", 0,0,1,1,128,32);
  hp.cover = Sprite(graphics, "sprites/healthbarcover.png", 0,0,1,1,128,32);
  hp.graphic.setHUD();
  hp.cover.setHUD();
  
  hp.graphic._spriteScale = 2;
  hp.cover._spriteScale = 2;
  
  hp.max = 5;
  hp.current = 5;
  
  _healthBar = hp;
}

void HUD::update(float dt, Player* player){
  _healthBar.update(dt,player);
  
}

void HUD::draw(Graphics &graphics){
  _healthBar.draw(graphics);
}