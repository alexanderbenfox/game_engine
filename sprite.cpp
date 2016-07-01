#include "sprite.h"
#include "graphics.h"
#include "conf.h"
#include <iostream>
#include "SDL2_image/SDL_image.h"

Sprite::Sprite(){}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int posX, int posY, int width, int height) : _x(posX), _y(posY)
{
  std::cerr<<"gets here";
  _sourceRect.x = sourceX;
  _sourceRect.y = sourceY;
  _sourceRect.w = width;
  _sourceRect.h = height;
  
  _collider = Rectangle(_x, _y, width * SPRITE_SCALE, height * SPRITE_SCALE);

  //do the file loading stuff
  //
Uint32 rmask, gmask, bmask, amask;

  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
   on the endianness (byte order) of the machine */
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

  SDL_Surface *surface = SDL_CreateRGBSurface(0, 640, 480, 32, rmask, gmask, bmask, amask);
  SDL_Surface *theSurface = IMG_Load(filePath.c_str());
  if(theSurface == NULL)
  {
    printf("Unable to load image %s SDL_Image error: %s\n", filePath.c_str(), SDL_GetError());
  }
  //SDL_Surface *optSurface = SDL_ConvertSurface(surface, SDL_GetWindowSurface(graphics.getWindow())->format, NULL);
  //graphics.loadImage(filePath);
  SDL_Surface *theWindow = SDL_GetWindowSurface(graphics.getWindow());
  _spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
  if (_spriteSheet == NULL) {
    printf("\nError: Unable to load image\n");
  } 
}

Sprite::~Sprite() {}

void Sprite::update(){
  _collider = Rectangle(_x,_y,_sourceRect.w * SPRITE_SCALE, _sourceRect.h * SPRITE_SCALE);
}

void Sprite::draw(Graphics &graphics, int x, int y) {
  SDL_Rect nextRect = {x,y,_sourceRect.w * SPRITE_SCALE, _sourceRect.h * SPRITE_SCALE};

  graphics.blitSurface(&this->_sourceRect, &nextRect, _spriteSheet);
}

Rectangle Sprite::getCollider(){
  return _collider;
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

//sides stuff goes here


