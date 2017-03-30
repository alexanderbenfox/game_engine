#include "sprite.h"
#include "graphics.h"
#include "conf.h"
#include "SDL2_image/SDL_image.h"
#include <iostream>

Sprite::Sprite(){}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int posX, int posY, int width, int height) : _x(posX), _y(posY)
{
  _sourceRect.x = sourceX;
  _sourceRect.y = sourceY;
  _sourceRect.w = width;
  _sourceRect.h = height;
  
  _collider = Rectangle(_x, _y, width * SPRITE_SCALE, height * SPRITE_SCALE);

  //do the file loading stuff
  //
Uint32 rmask, gmask, bmask, amask;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
  #else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
  #endif
  if(SpriteLoader::spriteLoaded(filePath)){
    _spriteSheet = SpriteLoader::getTexture(filePath);
  }else{
    SpriteLoader::addTexture(filePath, SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath)));
    _spriteSheet = SpriteLoader::getTexture(filePath);
  }
  if (_spriteSheet == NULL) {
    printf("\nError: Unable to load image\n");
  }
  
  this->addSpriteSheet(graphics, filePath, "main", width, height);
  _spriteSheet = _spriteSheets["main"].sheet;
  _spriteSheetSurface = _spriteSheets["main"].sheet_surface;
}

Sprite::~Sprite() {}

void Sprite::addSpriteSheet(Graphics &graphics, const std::string &filePath, std::string name, int width, int height){
  if(SpriteLoader::spriteLoaded(filePath)){
    _spriteSheets[name].sheet = SpriteLoader::getTexture(filePath);
    _spriteSheets[name].sheet_surface = SpriteLoader::getSurface(filePath);
  }
  else{
    SpriteLoader::addSurface(filePath, graphics.loadImage(filePath));
    SpriteLoader::addTexture(filePath, SDL_CreateTextureFromSurface(graphics.getRenderer(), SpriteLoader::getSurface(filePath)));
    _spriteSheets[name].sheet = SpriteLoader::getTexture(filePath);
    _spriteSheets[name].sheet_surface = SpriteLoader::getSurface(filePath);
    
  }
  _spriteSheets[name].width = width;
  _spriteSheets[name].height = height;
}

void Sprite::update(){
  //_collider = Rectangle(_x,_y,_sourceRect.w * SPRITE_SCALE, _sourceRect.h * SPRITE_SCALE);
  _collider = Rectangle(_x,_y,_sourceRect.w, _sourceRect.h);
}

void Sprite::draw(Graphics &graphics, int x, int y) {
  SDL_Rect nextRect = {x,y,_sourceRect.w * SPRITE_SCALE, _sourceRect.h * SPRITE_SCALE};
  nextRect.w = _sourceRect.w * _spriteScale;
  nextRect.h = _sourceRect.h * _spriteScale;
  
  SDL_RendererFlip flip = _flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
  
  if(_HUD){
    graphics.blitSurfaceIgnoreCamera(&this->_sourceRect, &nextRect, _spriteSheet, 0,NULL,flip);
  }
  else{
    graphics.blitSurface(&this->_sourceRect, &nextRect, _spriteSheet, 0,NULL,flip);
  }
}

void Sprite::drawBar(Graphics &graphics, int x, int y, int max, int current){
  SDL_Rect nextRect = {x,y,_sourceRect.w * SPRITE_SCALE, _sourceRect.h * SPRITE_SCALE};
  nextRect.w = (float)(_sourceRect.w * _spriteScale)*((float)current/(float)max);
  nextRect.h = (float)(_sourceRect.h * _spriteScale);
  
  SDL_RendererFlip flip = _flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
  
  graphics.blitSurfaceIgnoreCamera(&this->_sourceRect, &nextRect, _spriteSheet, 0,NULL,flip);
}

Rectangle Sprite::getCollider(){
  return _collider;
}

const collision_sides::Side Sprite::getCollisionSide(Rectangle &other) const{
  int overlapRight, overlapLeft, overlapBottom, overlapTop;
  overlapRight = _collider.getRight() - other.getLeft();
  overlapLeft = _collider.getLeft() - other.getRight();
  overlapBottom = _collider.getBottom() - other.getTop();
  overlapTop = _collider.getTop() - other.getBottom();
  
  //get smallest element
  int overlaps[4] = {abs(overlapRight), abs(overlapLeft), abs(overlapBottom), abs(overlapTop)};
  int minValue = abs(overlapRight);
  for(int i = 0; i < 4; i++){
    if (overlaps[i] < minValue)
      minValue = overlaps[i];
  }
  
  if (minValue == abs(overlapRight)) return collision_sides::RIGHT;
  if (minValue == abs(overlapLeft)) return collision_sides::LEFT;
  if (minValue == abs(overlapBottom)) return collision_sides::BOTTOM;
  if (minValue == abs(overlapTop)) return collision_sides::TOP;
  
  return collision_sides::NONE;
}

void Sprite::setFlipped(bool flipped){
  _flipped = flipped;
}

void Sprite::setHUD(){
  _HUD = true;
}

void Sprite::setColliderX(int value){
  _sourceRect.x = value;
}
void Sprite::setColliderY(int value){
  _sourceRect.y = value;
}
void Sprite::setColliderWidth(int value){
  _sourceRect.w = value;
}
void Sprite::setColliderHeight(int value){
  _sourceRect.h = value;
}
void Sprite::moveSprite(float x,float y)
{
  this->_x += x;
  this->_y += y;
}

void Sprite::setX(float x){
  _x = x;
}
void Sprite::setY(float y){
  _y = y;
}

const bool Sprite::collidesWith(const Rectangle &other) const{
  return
  (_collider.getRight()) >= other.getLeft() &&
  (_collider.getLeft()) <= other.getRight() &&
  (_collider.getBottom()) >= other.getTop() &&
  (_collider.getTop()) <= other.getBottom();
}

//sides stuff goes here


