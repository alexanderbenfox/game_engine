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

void ItemUI::update(float dt, Player *player){
  Item *item = player->getInventory()->getCurrentItem();
  itemPicture = *item->getSprite();
  num = item->getNum();
}

void CurrencyUI::update(float dt, Player *player){
  num = player->getInventory()->getCurrency();
}

void PopupBox::update(float dt, Player *player){
  
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
  bosshp.name = "Snake King Geedorah";
  bosshp.graphic = Sprite(graphics, "sprites/healthbar.png", 0,0,1,1,WINDOW_WIDTH/SPRITE_SCALE,20*SPRITE_SCALE);
  bosshp.cover = Sprite(graphics, "sprites/healthbarcover.png", 0,0,1,1,WINDOW_WIDTH/SPRITE_SCALE,20*SPRITE_SCALE);
  bosshp.graphic.setHUD();
  bosshp.cover.setHUD();
  
  bosshp.graphic._spriteScale = 2;
  bosshp.cover._spriteScale = 2;
  
  bosshp.max = 5;
  bosshp.current = 5;
  
  _bossHealthBar = bosshp;
  
  ItemUI itemUI;
  itemUI.posX = 16*SPRITE_SCALE;
  itemUI.posY = 32*SPRITE_SCALE;
  itemUI.graphic = Sprite(graphics, "sprites/item-ui.png",0,0,1,1,32,32);
  itemUI.graphic.setHUD();
  itemUI.graphic._spriteScale = 2;
  _itemUI = itemUI;
  
  CurrencyUI currency;
  currency.posX = 64 * SPRITE_SCALE;
  currency.posY = 32 *SPRITE_SCALE;
  currency.graphic = Sprite(graphics, "sprites/currency-ui.png",0,0,1,1,64,25);
  currency.graphic.setHUD();
  currency.graphic._spriteScale = 2;
  _currencyUI = currency;
  
  PopupBox popup;
  popup.posX = WINDOW_WIDTH*(1/3);
  popup.posY = WINDOW_HEIGHT*(1/3);
  popup.width = WINDOW_WIDTH/3;
  popup.height = WINDOW_HEIGHT/3;
  popup.graphic = Sprite(graphics, "sprites/popupbox.png", 0,0,1,1,WINDOW_WIDTH/3,WINDOW_HEIGHT/3);
  popup.graphic.setHUD();
  popup.Disable();
  _popup = popup;
  
}

void HUD::update(float dt, Player* player){
  _healthBar.update(dt,player);
  _bossHealthBar.update(dt,player);
  _itemUI.update(dt, player);
  _currencyUI.update(dt,player);
  
}

void HUD::draw(Graphics &graphics){
  _healthBar.draw(graphics);
  _bossHealthBar.draw(graphics);
  _itemUI.draw(graphics);
  _currencyUI.draw(graphics);
  if(_popup.enabled)
    _popup.draw(graphics);
}

void HUD::message(std::string text){
  _popup.setMessage(text);
  _popup.Enable();
}

void HUD::disablePopup(){
  _popup.Disable();
}