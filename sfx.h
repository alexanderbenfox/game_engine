#ifndef SFX_H
#define SFX_H

#include <vector>
#include "animatedsprite.h"
#include <map>

enum SFX_TYPES{
  explosion, blood1, blood2, slash, drop, blast, button_prompt, DUST, arrow_blast, splatter, white_splatter, splatter2
  
};

struct SFX_SPRITES{
  SFX_SPRITES(){}
  SFX_SPRITES(Graphics *graphics, float _frameTime, int _numberOfFrames, int _startFrame, float life)
  {
    _graphics = graphics;
    filePath = "sprites/sfx-sheet.png";
    startX = 1;
    startY = 1;
    height = 64;
    width = 64;
    
    frameTime = _frameTime;
    numberOfFrames = _numberOfFrames;
    startFrame = _startFrame;
    lifetime = life;
  }
  
  Graphics *_graphics;
  std::string filePath;
  int startX, startY;
  int height,width;
  float lifetime;
  
  float frameTime;
  int numberOfFrames;
  int startFrame;
};

struct SFX{
  int _x,_y;
  float _lifetime;
  AnimatedSprite _sprite;
  bool _done;
  
  SFX(int x, int y, SFX_SPRITES setupSprite){
    _x = x;
    _y = y;
    _sprite = AnimatedSprite(*setupSprite._graphics, setupSprite.filePath, setupSprite.startX, setupSprite.startY, (float)x, (float)y, setupSprite.height, setupSprite.width);
    _sprite.addAnimation(setupSprite.frameTime, setupSprite.numberOfFrames, setupSprite.startFrame, 0, "Animation", setupSprite.width, setupSprite.height, Vector2(0,0),"main");
    _sprite.playAnimation("Animation");
    _lifetime = setupSprite.lifetime;
    _sprite._spriteScale = 2;
  }
  
  void update(float dt){
    _lifetime -= dt;
    _sprite.update(dt);
    
    if(_lifetime <= 0){
      _done = true;
    }
  }
  
  void setFlipped(bool flipped){
    _sprite.setFlipped(flipped);
  }
  
  void setScale(int scale){
    _sprite._spriteScale = scale;
  }
  
  void draw(Graphics &graphics){
    if(!_done)
      _sprite.draw(graphics, _x, _y);
  }
};



class SFXManager{
public:
  SFXManager();
  SFXManager(Graphics &graphics);
  
  void addSFX(SFX_TYPES type, int x, int y, bool flipped = false, int scale = 2);
  void update(float dt);
  void draw(Graphics &graphics);
  
  const long getSize() const{
    return _sfx.size();
  }
  
private:
  std::vector<SFX> _sfx;
  std::map<SFX_TYPES , SFX_SPRITES> _baseSprites;
};

#endif
