#include "arrow.h"
#include "enemy.h"

Arrow::Arrow(Graphics &graphics, int startX, int startY, bool right, ArrowSprites a, float velY){
  
  _dx = right ? 9000.0 : -9000.0;
  _dy = velY;
  done = false;
  _x = startX;
  _y = startY;
  
  _collider = Rectangle(_x,_y, 64*1.5, 32*1.5);
  
  if(right){
    theArrow = a.arrowR;
    _stuck = a.stuckR;
    _baseghost = a.ghostR;
    
  }
  else{
    theArrow = a.arrowL;
    _stuck = a.stuckL;
    _baseghost = a.ghostL;
  }
  
  _baseghost->_spriteScale = 1.8;
  _stuck->_spriteScale = 1.5;
  theArrow->_spriteScale = 1.5;
  
  _ghostPeriod = 0.02;
  //int i = rand()%25 + 15;
  int i = 15;
  _ghostPeriod = ((float)i)/1000.0;
  sfx = SFXManager(graphics, true);
  
  
}

Arrow::~Arrow(){
  
}

void Arrow::update(float deltaTime){
  sfx.update(deltaTime);
  _collider = Rectangle(_x,_y,_collider.getWidth(), _collider.getHeight());
  
  for(int i = 0; i < trail.size(); i++){
    trail[i].lifetime += deltaTime;
    if(trail[i].lifetime > 0.012 || _lifetime < 100.0){
      trail.erase(trail.begin() + i);
    }
  }
  
  if(_lifetime < 100.0){
    theArrow->setVisible(false);
    _lifetime -= deltaTime;
    if(_lifetime <= 0)
    {
      done = true;
    }
  }
  else{
    _x = (_x + _dx*deltaTime);
    _y = (_y + _dy*deltaTime);
  }
  
  _trailTime += deltaTime;
  
  if (_trailTime > _ghostPeriod && _lifetime == 100.0){
    ghost newg;
    newg.x = (_x);
    newg.y = (_y);
    newg.lifetime = 0;
    
    _trailTime = 0;
    
    trail.push_back(newg);
  }
  
}

void Arrow::draw(Graphics &graphics){
  sfx.draw(graphics);
  if(_lifetime == 100.0)
    theArrow->draw(graphics, _x, _y);
  //SDL_RenderDrawLine(graphics.getRenderer(), this->_collider.getLeft(),this->_collider.getTop(),this->_collider.getLeft()+_collider.getWidth(),this->_collider.getTop()+_collider.getHeight());
  for(ghost arrow : trail){
    _baseghost->draw(graphics, arrow.x, arrow.y);
  }
  
  if(_lifetime < 100.0){
    _stuck->draw(graphics,stuckPos.x,stuckPos.y);
  }
}

void Arrow::handleCollisions(Map &map){
  if(!_hitWall){
    std::vector<Rectangle> tiles = map.checkTileCollisions(_collider);
    for (Rectangle tile : tiles){
      collision_sides::Side side = _collider.getCollisionSide(tile);
      if (side != collision_sides::NONE){
        switch(side){
          case(collision_sides::RIGHT) :
            _hitWall = true;
            _dx = (_dx > 0) ? 0 : _dx;
            _x = (tile.getLeft() - _collider.getWidth()-2);
            //done = true;
            _lifetime = 2.0;
            stuckPos = Vector2(_x+50, _y);
            sfx.addSFX(arrow_blast, _x+50, _collider.getCenterY()-32);
            break;
          case(collision_sides::LEFT) :
            _hitWall = true;
            _dx = (_dx < 0) ? 0 : _dx;
            _x = (tile.getRight()-1 + 2);
            //done = true;
            _lifetime = 2.0;
            stuckPos = Vector2(_x,_y);
            sfx.addSFX(arrow_blast, _x -32, _collider.getCenterY()-32);
            break;
        }
      }
    }
  }
  handleEnemyCollisions(map);
}

void Arrow::handleEnemyCollisions(Map &map){
  if(!_hitEnemy){
    std::vector<Enemy*> enemies = map.checkEnemyCollisions(_collider);
    std::vector<Rectangle> tiles;
    for(Enemy* enemy : enemies){
      if (!enemy->isPlayingDeathAnimation()){
        tiles.push_back(enemy->getCollider());
        enemy->changeHealth(-2);
        _hitEnemy = true;
      }
      
    }
    for (Rectangle tile : tiles){
      collision_sides::Side side = _collider.getCollisionSide(tile);
      if (side != collision_sides::NONE){
        switch(side){
          case(collision_sides::RIGHT) :
            _hitEnemy = true;
            _x = (tile.getLeft() - _collider.getWidth()-2);
            _lifetime = 2.0;
            _hitEnemy = true;
            if(_dx > 0)
              sfx.addSFX(splatter, tile.getCenterX()-64 + 10, _collider.getCenterY()-64, false);
            else
              sfx.addSFX(splatter, tile.getCenterX()-64 - 10, _collider.getCenterY()-64, true);
            _dx = (_dx > 0) ? 0 : _dx;
            _dy = 0;
            break;
          case(collision_sides::LEFT) :
            _hitEnemy = true;
            _x = (tile.getRight()-1 + 2);
            _lifetime = 2.0;
            _hitEnemy = true;
            if(_dx > 0)
              sfx.addSFX(splatter, tile.getCenterX()-64 + 10, _collider.getCenterY()-64, false);
            else
              sfx.addSFX(splatter, tile.getCenterX()-64 - 10, _collider.getCenterY()-64, true);
            
            _dx = (_dx < 0) ? 0 : _dx;
            _dy = 0;
            break;
        }
      }
    }
  }
}
