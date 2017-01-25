#include "enemyhitbox.h"
#include "player.h"

EnemyHitbox::EnemyHitbox(){}

EnemyHitbox::EnemyHitbox(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width, float dx, float dy, float lifetime, bool isAnimated, bool gravity, int animLength){
  
  _isAnimated = isAnimated;
  _gravity = gravity;
  
  length = animLength;
  
  
  if (!isAnimated){
    sprite = Sprite(graphics, filePath, startX, startY, posX, posY, width, height);
  }
  else{
    animSprite = AnimatedSprite(graphics, filePath, startX, startY, std::ceil(posX), std::ceil(posY), width, height);
    animSprite._spriteScale = 2;
    animSprite.addAnimation(.02, length, 0, 0, "Anim", width, height, Vector2(0,0), "main");
    animSprite.playAnimation("Anim");
  }
  
  hitbox = Rectangle(posX, posY, width, height);
  _dx = dx;
  _dy = dy;
  _lifetime = lifetime;
}

void EnemyHitbox::update(float dt, Player &player){
  if(_gravity)
    applyGravity(dt);
  hitbox.moveAnchor(hitbox.getLeft() + dt*_dx, hitbox.getTop() + dt*_dy);
  if(_isAnimated){
    animSprite.setXPlease(hitbox.getLeft());
    animSprite.setYPlease(hitbox.getTop());
    animSprite.update(dt);
  }
  else{
    sprite.setX(hitbox.getLeft());
    sprite.setY(hitbox.getTop());
    sprite.update();
  }
  
  _lifetime -= dt;
  if(_lifetime <= 0){
    finished = true;
  }
  
}

void EnemyHitbox::draw(Graphics &graphics){
  if(_isAnimated){
    animSprite.draw(graphics, hitbox.getLeft(), hitbox.getTop());
  }
  else{
    sprite.draw(graphics, hitbox.getLeft(), hitbox.getTop());
  }
}

void EnemyHitbox::collidePlayer(Player &player){
  player.takeDamage(1);
  finished = true;
}