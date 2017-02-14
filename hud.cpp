#include "hud.h"
#include "player.h"
#include "boss.h"

void HealthBar::update(float dt, Player *player){
  graphic.update();
  max = player->_hpMax;
  current = player->_hp;
}

void BossHealthBar::update(float dt, Player *player){
  if(player->isTrackingBoss()){
    isTrackingBoss = true;
    graphic.update();
    max = player->getTrackedBoss()->_maxHealth;
    current =player->getTrackedBoss()->_currentHealth;
  }
  else{
    isTrackingBoss = false;
  }
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
  
  BossHealthBar bosshp;
  bosshp.posX = 0;
  bosshp.posY = WINDOW_HEIGHT - 96;
  bosshp.graphic = Sprite(graphics, "sprites/healthbar.png", 0,0,1,1,WINDOW_WIDTH/SPRITE_SCALE,20*SPRITE_SCALE);
  bosshp.cover = Sprite(graphics, "sprites/healthbarcover.png", 0,0,1,1,WINDOW_WIDTH/SPRITE_SCALE,20*SPRITE_SCALE);
  bosshp.graphic.setHUD();
  bosshp.cover.setHUD();
  
  bosshp.graphic._spriteScale = 2;
  bosshp.cover._spriteScale = 2;
  
  bosshp.max = 5;
  bosshp.current = 5;
  
  _bossHealthBar = bosshp;
}

void HUD::update(float dt, Player* player){
  _healthBar.update(dt,player);
  _bossHealthBar.update(dt,player);
  
}

void HUD::draw(Graphics &graphics){
  _healthBar.draw(graphics);
  _bossHealthBar.draw(graphics);
}