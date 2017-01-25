#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include "conf.h"
#include <iostream>

#include <string>
#include <map>

//collision stuff later
#include "rectangle.h"

class Graphics;

struct SHEET{
  int width, height;
  SDL_Texture* sheet;
};

class SpriteLoader{
public:
  static SpriteLoader* getInstance(){
    static SpriteLoader instance;
    return &instance;
  }
  
  static SDL_Texture* getTexture(std::string filePath){
    if(getInstance()->spriteLoaded(filePath)){
      return getInstance()->_textures[filePath];
    }
    else return NULL;
  }
  
  static std::map<std::string, SDL_Texture*> Textures(){
    return getInstance()->_textures;
  }
  
  static bool spriteLoaded(std::string filePath){
    return (getInstance()->_textures.count(filePath) != 0);
  }
  
  static void addTexture(std::string filePath, SDL_Texture* texture){
    getInstance()->_textures[filePath] = texture;
  }
  
  SpriteLoader(SpriteLoader const&) = delete;
  void operator=(SpriteLoader const&) = delete;
private:
  SpriteLoader(){}
  std::map<std::string,SDL_Texture*> _textures;
};

class Sprite{

public:
  Sprite();
  Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int posX, int posY, int width, int height);
  virtual ~Sprite();

  virtual void update();
  void draw(Graphics &graphics, int x, int y);
  void drawBar(Graphics &graphics, int x, int y, int max, int current);

  //const Rectangle getBoundingBox() const;
  //const sides::Side getCollisionSide(Rectangle &other) const;

  const inline float getX() const
    {return this->_x;}
  const inline float getY() const
    {return this->_y;}
  
  void setX(float x);
  void setY(float y);

  Rectangle getCollider();

  void setColliderX(int value);
  void setColliderY(int value);
  void setColliderWidth(int value);
  void setColliderHeight(int value);
  
  void addSpriteSheet(Graphics &graphics, const std::string &filePath, std::string name, int width, int height);
  
  void moveSprite(float x, float y);
  const collision_sides::Side getCollisionSide(Rectangle &other) const;
  
  float _spriteScale = 1;
  Uint8 transparency = 255;
  
  void setFlipped(bool flipped);
  void setHUD();

protected:
  
  float _x, _y;
  bool _flipped;
  
  Rectangle _collider;

  SDL_Rect _sourceRect;
  SDL_Texture* _spriteSheet;
  float _scale = 1;
  
  std::map<std::string, SHEET> _spriteSheets;
  std::string _currentSheet;
  
  bool _HUD = false;

private:
};

#endif
