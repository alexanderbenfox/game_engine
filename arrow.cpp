#include "arrow.h"
#include "enemy.h"

Arrow::Arrow(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width, bool right, float velY) : AnimatedSprite(graphics,filePath,startX,startY,posX,posY,height,width){
  _spriteScale = 1.5;
  _dx = right ? 9000.0 : -9000.0;
  _dy = velY;
  _flipped = !right;
  done = false;
  
  _collider = Rectangle(this->getX(),this->getY(), width*_spriteScale, height*_spriteScale);
  
  _baseghost = Sprite(graphics, "sprites/arrowtrail.png", 0,0,1,1,64,32);
  _baseghost._spriteScale = 1.8;
  
  _stuck = Sprite(graphics,"sprites/arrowhalf.png",0,0,1,1,32,32);
  _stuck._spriteScale = 1.5;
  _stuck.setFlipped(!right);
  
  _ghostPeriod = 0.02;
  srand(time(NULL));
  int i = rand()%25 + 15;
  _ghostPeriod = ((float)i)/1000.0;
  std::cout<<"HOST"<<_ghostPeriod<<std::endl;
  
  sfx = SFXManager(graphics);
  
  
}

Arrow::~Arrow(){
  
}

void Arrow::update(float deltaTime){
  sfx.update(deltaTime);
  this->setX(this->getX() + _dx*deltaTime);
  this->setY(this->getY() + _dy*deltaTime);
  _collider = Rectangle(this->getX(),this->getY(),_collider.getWidth(), _collider.getHeight());
  
  for(int i = 0; i < trail.size(); i++){
    trail[i].lifetime += deltaTime;
    if(trail[i].lifetime > 0.012 || _lifetime < 100.0){
      trail.erase(trail.begin() + i);
    }
  }
  
  if(_lifetime < 100.0){
    this->setVisible(false);
    _lifetime -= deltaTime;
    if(_lifetime <= 0)
    {
      done = true;
    }
  }
  
  _trailTime += deltaTime;
  
  if (_trailTime > _ghostPeriod && _lifetime == 100.0){
    ghost newg;
    newg.x = (this->getX());
    newg.y = (this->getY());
    newg.lifetime = 0;
    
    _trailTime = 0;
    
    trail.push_back(newg);
  }
  
}

void Arrow::draw(Graphics &graphics){
  sfx.draw(graphics);
  if(_lifetime == 100.0)
    Sprite::draw(graphics, this->getX(), this->getY());
  //SDL_RenderDrawLine(graphics.getRenderer(), this->_collider.getLeft(),this->_collider.getTop(),this->_collider.getLeft()+_collider.getWidth(),this->_collider.getTop()+_collider.getHeight());
  for(ghost arrow : trail){
    _baseghost.draw(graphics, arrow.x, arrow.y);
  }
  
  if(_lifetime < 100.0){
    _stuck.draw(graphics,_stuck.getX(),_stuck.getY());
  }
}

void Arrow::handleCollisions(Map &map){
  std::vector<Rectangle> tiles = map.checkTileCollisions(_collider);
  for (Rectangle tile : tiles){
    collision_sides::Side side = Sprite::getCollisionSide(tile);
    if (side != collision_sides::NONE){
      switch(side){
        case(collision_sides::RIGHT) :
          _dx = (_dx > 0) ? 0 : _dx;
          this->setX(tile.getLeft() - _collider.getWidth()-2);
          //done = true;
          _lifetime = 2.0;
          _stuck.setX(this->getX()+50);
          _stuck.setY(this->getY());
          sfx.addSFX(arrow_blast, this->getX()+50, this->getCollider().getCenterY()-32);
          break;
        case(collision_sides::LEFT) :
          _dx = (_dx < 0) ? 0 : _dx;
          this->setX(tile.getRight()-1 + 2);
          //done = true;
          _lifetime = 2.0;
          _stuck.setX(this->getX());
          _stuck.setY(this->getY());
          sfx.addSFX(arrow_blast, this->getX() -32, this->getCollider().getCenterY()-32);
          break;
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
        enemy->changeHealth(-1);
      }
      
    }
    for (Rectangle tile : tiles){
      collision_sides::Side side = Sprite::getCollisionSide(tile);
      if (side != collision_sides::NONE){
        switch(side){
          case(collision_sides::RIGHT) :
            this->setX(tile.getLeft() - _collider.getWidth()-2);
            _lifetime = 2.0;
            _hitEnemy = true;
            if(_dx > 0)
              sfx.addSFX(splatter, tile.getCenterX()-64 + 10, this->getCollider().getCenterY()-64, false);
            else
              sfx.addSFX(splatter, tile.getCenterX()-64 - 10, this->getCollider().getCenterY()-64, true);
            _dx = (_dx > 0) ? 0 : _dx;
            break;
          case(collision_sides::LEFT) :
            this->setX(tile.getRight()-1 + 2);
            _lifetime = 2.0;
            _hitEnemy = true;
            if(_dx > 0)
              sfx.addSFX(splatter, tile.getCenterX()-64 + 10, this->getCollider().getCenterY()-64, false);
            else
              sfx.addSFX(splatter, tile.getCenterX()-64 - 10, this->getCollider().getCenterY()-64, true);
            
            _dx = (_dx < 0) ? 0 : _dx;
            break;
        }
      }
    }
  }
}
