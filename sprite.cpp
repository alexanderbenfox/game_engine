#include "sprite.h"
#include "graphics.h"
#include "conf.h"

Sprite::Sprite(){}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int posX, int posY, int width, int height) : _x(posX), _y(posY)
{
  _sourceCollider.x = sourceX;
  _sourceCollider.y = sourceY;
  _sourceCollider.w = width;
  _sourceCollider.h = height;
  
  _collider = Rectangle(_x, _y, width * SPRITE_SCALE, height * SPRITE_SCALE);

  //do the file loading stuff
  //
  _spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
  if (_spriteSheet == NULL) {
    printf("\nError: Unable to load image\n");
  } 
}

Sprite::~Sprite() {}

Sprite::update(){
  _collider = Rectangle(_x,_y,_sourceRect.w * SPRITE_SCALE, _sourceRect.h * SPRITE_SCALE);
}

Sprite::draw(Graphics &graphics, int x, int y) {
  SDL_Rect nextRect = SDL_Rect(x,y,_sourceRect.w * SPRITE_SCALE, _sourceRect.h * SPRITE_SCALE);

  graphics.blitSurface(spriteSheet, &this->_sourceRect, &nextRect);
}

Sprite::getCollider(){
  return _collider;
}

Sprite::setColliderX(int value){
  _sourceRect.x = value;
}
Sprite::setColliderY(int value){
  _sourceRect.y = value;
}
Sprite::setColliderWidth(int value){
  _sourceRect.w = value;
}
Sprite::setColliderHeight(int value){
  _sourceRect.h = value;
}

//sides stuff goes here


