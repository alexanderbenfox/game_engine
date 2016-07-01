#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include "conf.h"

#include <string>

//collision stuff later
#include "rectangle.h"

class Graphics;

class Sprite{

public:
  Sprite();
  Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int posX, int posY, int width, int height);
  virtual ~Sprite();

  virtual void update();
  void draw(Graphics &graphics, int x, int y);

  //const Rectangle getBoundingBox() const;
  //const sides::Side getCollisionSide(Rectangle &other) const;

  const inline float getX() const
    {return this->_x;}
  const inline float getY() const
    {return this->_y;}

  Rectangle getCollider();

  void setColliderX(int value);
  void setColliderY(int value);
  void setColliderWidth(int value);
  void setColliderHeight(int value);

protected:

  float _x, _y;

  Rectangle _collider;

  SDL_Rect _sourceRect;
  SDL_Texture* _spriteSheet;

private:
};

#endif
