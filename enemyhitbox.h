#ifndef ENEMYHITBOX_H
#define ENEMYHITBOX_H

#include "animatedsprite.h"

class Player;

class EnemyHitbox{
public:
  Rectangle hitbox;
  Sprite sprite;
  AnimatedSprite animSprite;
  
  EnemyHitbox();
  EnemyHitbox(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width, float dx, float dy, float lifetime, bool isAnimated = false, bool gravity = true, int animLength = 0, int damage = 1);
  
  const bool collidesWith(const Rectangle &other) const {
    return
    (hitbox.getRight()) >= other.getLeft() &&
    (hitbox.getLeft()) <= other.getRight() &&
    (hitbox.getBottom()) >= other.getTop() &&
    (hitbox.getTop()) <= other.getBottom();
  }
  
  void flipSprite(){
    if(_isAnimated)
      animSprite.setFlipped(true);
    else
      sprite.setFlipped(true);
  }
  
  void applyGravity(float dt){
    _dy += (GRAVITY*dt);
  }
  
  void update(float dt, Player &player);
  void draw(Graphics &graphics);
  
  void collidePlayer(Player &player);
  
  bool finished = false;
  void setDestroyable();
  bool isDestroyable();
  void Destroy();
  
private:
  int length;
  int _damage;
  
  float _dx,_dy;
  bool _isAnimated = false;
  bool _noSprite = false;
  bool _gravity = true;
  
  float _lifetime;
  bool _destroyable;
  
};

#endif