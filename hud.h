#ifndef HUD_H
#define HUD_H

#include "conf.h"
#include <vector>
#include "sprite.h"
#include "textdraw.h"

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

struct ItemUI : Component{
  int num;
  Sprite itemPicture;
  void update(float dt, Player* player);
  void draw(Graphics &graphics){
    graphic.draw(graphics, posX, posY);
    itemPicture.draw(graphics, posX, posY);
    std::string message = std::to_string(num);
    TextRender::RenderTextOnScreen(graphics, message, 12*SPRITE_SCALE, white, Vector2(posX + 20*SPRITE_SCALE, posY + 15*SPRITE_SCALE));
  }
};

struct CurrencyUI : Component{
  int num;
  void update(float dt, Player* player);
  void draw(Graphics &graphics){
    graphic.draw(graphics, posX, posY);
    std::string message = std::to_string(num);
    TextRender::RenderTextOnScreen(graphics, message, 12*SPRITE_SCALE, white, Vector2(posX + 20*SPRITE_SCALE, posY + 2*SPRITE_SCALE));
    
  }
};

struct PopupBox : Component{
  std::string message;
  int width, height;
  bool enabled;
  void update(float dt, Player* player);
  void draw(Graphics &graphics){
    posX = WINDOW_WIDTH/3;
    posY = WINDOW_HEIGHT/3;
    int w,h;
    TextRender::GetTextSize(graphics, &w, &h, message);
    graphic.draw(graphics, posX, posY);
    float x = posX + width/2 - w/2;
    float y = posY+height/2- h/2;
    TextRender::RenderTextOnScreen(graphics, message, 30*SPRITE_SCALE, white, Vector2(x,y));
  }
  
  void Enable(){
    enabled = true;
  }
  
  void Disable(){
    enabled = false;
  }
  
  void setMessage(std::string m){
    message = m;
  }
};

struct BossHealthBar : Component{
  int max;
  int current;
  Sprite cover;
  bool isTrackingBoss;
  std::string name;
  
  void update(float dt, Player* player);
  
  void draw(Graphics &graphics){
    if(isTrackingBoss){
      graphic.drawBar(graphics, posX, posY, max, current);
      cover.draw(graphics,posX,posY);
      TextRender::RenderTextOnScreen(graphics, name, 12*SPRITE_SCALE, white, Vector2(posX+15*SPRITE_SCALE, posY-12*SPRITE_SCALE));
    }
  }
};

class HUD{
public:
  HUD();
  HUD(Graphics &graphics);
  
  void update(float dt, Player* player);
  void draw(Graphics &graphics);
  
  void message(std::string text);
  void disablePopup();
  
private:
  std::vector<Component> _components;
  HealthBar _healthBar;
  BossHealthBar _bossHealthBar;
  ItemUI _itemUI;
  CurrencyUI _currencyUI;
  PopupBox _popup;
};

#endif