#include "swordhitbox.h"
#include "enemy.h"

SwordHitbox::SwordHitbox(){}

SwordHitbox::SwordHitbox(Graphics &graphics){
  _currentHitbox = Hitbox();
  _sfx = SFXManager(graphics);
}

void SwordHitbox::createHitBox(int x, int y, HitboxType type, float direction){
  switch(type){
    case STANDING:{
      if(direction >= 0){
        Rectangle temp = Rectangle(x+64,y,80+32,52);
        _currentHitbox = Hitbox(temp, direction);
      }
      else{
        Rectangle temp = Rectangle(x-(80+32),y,80+32,52);
        _currentHitbox = Hitbox(temp, direction);
      }
      _currentHitbox.setDelay(0.05);
      break;
    }
    case STANDING2:{
      if(direction >= 0){
        Rectangle temp = Rectangle(x+64,y,80+32,80);
        _currentHitbox = Hitbox(temp, direction);
      }
      else{
        Rectangle temp = Rectangle(x-(80+32),y,80+32,80);
        _currentHitbox = Hitbox(temp, direction);
      }
      _currentHitbox.setDelay(0.08);
      _currentHitbox.type = STANDING2;
      break;
    }
    case CROUCHING:{
      if(direction >= 0){
        Rectangle temp = Rectangle(x+64,y+64,80,32);
        _currentHitbox = Hitbox(temp, direction);
      }
      else{
        Rectangle temp = Rectangle(x-80,y+64,80,32);
        _currentHitbox = Hitbox(temp, direction);
      }
      _currentHitbox.setDelay(0.02);
      break;
    }
    case JUMPING:{
      if(direction >= 0){
        Rectangle temp = Rectangle(x+32,y-40,80+32,96+32);
        _currentHitbox = Hitbox(temp, direction);
      }
      else{
        Rectangle temp = Rectangle(x-80,y-40,80+32,96+32);
        _currentHitbox = Hitbox(temp, direction);
      }
      _currentHitbox.setDelay(0.04);
    }
    default:
      break;
  }
}
void SwordHitbox::update(float dt, float dx, float dy, float actionTimer){
  _sfx.update(dt);
  _currentHitbox.moveHitbox(dx*dt,dy*dt);
  if(_currentHitbox.isActive){
    _currentHitbox.lifetime -= dt;
    if(_currentHitbox.lifetime <= 0){
      _currentHitbox.isActive = false;
    }
  }
  //delay
  else if (_currentHitbox.delay > 0)
  {
    _currentHitbox.delay -= dt;
    if(_currentHitbox.delay <= 0){
      _currentHitbox.isActive = true;
      _currentHitbox.delay = 0;
    }
  }
  
  if(actionTimer <= 0)
    _currentHitbox.isActive = false;
}

void SwordHitbox::draw(Graphics &graphics){
  _sfx.draw(graphics);
  /*if(_currentHitbox.isActive){
    SDL_Rect camera = graphics.getCamera();
    Rectangle c = Rectangle(_currentHitbox.rect.getLeft()-camera.x, _currentHitbox.rect.getTop()-camera.y,_currentHitbox.rect.getWidth(), _currentHitbox.rect.getHeight());
    SDL_RenderDrawLine(graphics.getRenderer(), c.getLeft(),c.getTop(),c.getLeft()+c.getWidth(),c.getTop());
    SDL_RenderDrawLine(graphics.getRenderer(), c.getLeft(),c.getTop(),c.getLeft(),c.getBottom());
    SDL_RenderDrawLine(graphics.getRenderer(), c.getRight(),c.getTop(),c.getRight(),c.getBottom());
    SDL_RenderDrawLine(graphics.getRenderer(), c.getLeft(),c.getBottom(),c.getLeft()+c.getWidth(),c.getBottom());
  }*/
}

void SwordHitbox::handleEnemyCollisions(Map &map){
  float hitBoxStart = _currentHitbox.direction == 1 ? _currentHitbox.rect.getLeft() : _currentHitbox.rect.getRight();
  if(_currentHitbox.isActive){
    std::vector<Enemy*> enemies = map.checkEnemyCollisions(_currentHitbox.rect);
    for(Enemy* enemy : enemies){
      bool isDamagable = enemy->IsDamagable(hitBoxStart);
      if (!enemy->isPlayingDeathAnimation() && isDamagable && !enemy->cannotBeDamaged){
        bool strong = (_currentHitbox.type == STANDING2);
        enemy->changeHealth(-4);
        if(strong)
          enemy->changeHealth(-3);
        
        enemy->knockBack(_currentHitbox.direction, strong);
        _currentHitbox.hitRegistered = true;
        
        
        SFX_TYPES type = splatter;
        int i = rand()%10;
        type = (i > 2) ? splatter : splatter2;
        if(_currentHitbox.direction < 0)
          _sfx.addSFX(type, enemy->getCollider().getCenterX()-64 - 10, _currentHitbox.rect.getCenterY()-64, true);
        else
          _sfx.addSFX(type, enemy->getCollider().getCenterX()-64 + 10, _currentHitbox.rect.getCenterY()-64, false);
      }
      if(!isDamagable){
      }
    }
    std::vector<EnemyHitbox*> hbs = map.checkEnemyHitboxForDestruction(_currentHitbox.rect);
    for(int i = 0; i < hbs.size(); i++){
      hbs.at(i)->Destroy();
    }
  }
}

bool SwordHitbox::hitRegistered(){
  return (_currentHitbox.isActive && _currentHitbox.hitRegistered);
}