#ifndef SFX_H
#define SFX_H

#include <vector>
#include "animatedsprite.h"
#include <map>

enum SFX_TYPES{
  explosion, blood1, blood2, slash, drop, blast, button_prompt, DUST, arrow_blast, splatter, white_splatter, splatter2, CHARGE1, CHARGE2, CHARGED
  
};

struct SFX_SPRITES{
  SFX_SPRITES(){}
  SFX_SPRITES(Graphics *graphics, float _frameTime, int _numberOfFrames, int _startFrame, float life, SFX_TYPES type = explosion)
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
    _type = type;
  }
  
  Graphics *_graphics;
  std::string filePath;
  int startX, startY;
  int height,width;
  float lifetime;
  
  float frameTime;
  int numberOfFrames;
  int startFrame;
  SFX_TYPES _type;
};

struct SFX{
  float _x,_y;
  float _lifetime;
  AnimatedSprite _sprite;
  bool _done;
  SFX_TYPES type;
  
  SFX(int x, int y, SFX_SPRITES setupSprite){
    _x = x;
    _y = y;
    _lifetime = setupSprite.lifetime;
    _sprite = AnimatedSprite(*setupSprite._graphics, setupSprite.filePath, setupSprite.startX, setupSprite.startY, x, y, setupSprite.height, setupSprite.width);
    _sprite.addAnimation(setupSprite.frameTime, setupSprite.numberOfFrames, setupSprite.startFrame, 0, "Animation", setupSprite.width, setupSprite.height, Vector2(0,0),"main");
    _sprite.playAnimation("Animation");
    _sprite._spriteScale = 2;
    type = setupSprite._type;
    _done = false;
  }
  
  void update(float dt){
    _lifetime -= dt;
    _sprite.update(dt);
    
    if(_lifetime <= 0){
      _done = true;
    }
  }
  
  void move(float dx, float dy){
    _x += dx;
    _y += dy;
  }
  
  void finished(){
    _done = true;
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

/*class SFXSpriteLoader{
public:
  static SFXSpriteLoader* getInstance(Graphics &graphics){
    static SFXSpriteLoader instance(graphics);
    return &instance;
  }
  
  static std::vector<SFX> getSFX(Graphics &graphics){
    return getInstance(graphics)->_sfx;
  }
  
  static std::map<SFX_TYPES, SFX_SPRITES> getSprites(Graphics &graphics){
    return getInstance(graphics)->_baseSprites;
  }
  
  SFXSpriteLoader(SFXSpriteLoader const&) = delete;
  void operator=(SFXSpriteLoader const&) = delete;
private:
  SFXSpriteLoader(Graphics &graphics){
    SFX_SPRITES dust(&graphics, 0.03, 3, 24, 0.05);
    SFX_SPRITES arrow_exp(&graphics, 0.01, 5, 27, 0.05);
    SFX_SPRITES splat(&graphics, 0.01, 6, 32, 0.06);
    SFX_SPRITES splat2(&graphics, 0.01, 5, 44,0.05);
    
    SFX_SPRITES explosion_(&graphics, 0.01, 3, 0, 0.03);
    SFX_SPRITES blood1_(&graphics, 0.01, 3, 3, 0.03);
    SFX_SPRITES blood2_(&graphics, 0.01, 3, 6, 0.03);
    
    SFX_SPRITES charge1(&graphics, 0.01,4,49,100, CHARGE1);
    SFX_SPRITES charge2(&graphics, 0.02,5,53,100, CHARGE2);
    
    _baseSprites[arrow_blast] = arrow_exp;
    _baseSprites[DUST] = dust;
    _baseSprites[splatter] = splat;
    _baseSprites[splatter2] = splat2;
    
    _baseSprites[explosion] = explosion_;
    _baseSprites[blood1] = blood1_;
    _baseSprites[blood2] = blood2_;
    _baseSprites[CHARGE1] = charge1;
    _baseSprites[CHARGE2] = charge2;
    
  }
  //SFXSpriteLoader(SFXSpriteLoader const&);
  
  //void operator=(SFXSpriteLoader const&);
  
  std::vector<SFX> _sfx;
  std::map<SFX_TYPES , SFX_SPRITES> _baseSprites;
};*/



class SFXManager{
public:
  SFXManager();
  SFXManager(Graphics &graphics, bool arrow = false);
  
  void addSFX(SFX_TYPES type, int x, int y, bool flipped = false, int scale = 2);
  void endSFX(SFX_TYPES type);
  void moveSFX(SFX_TYPES type, float dx, float dy);
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
