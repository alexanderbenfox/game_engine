#include <SDL2/SDL.h>
#include "graphics.h"
#include "conf.h"

//constructor
Graphics::Graphics()
{
  //int SDL_CreateWindowAndRenderer(int            width,
  //                        int            height,
  //                        Uint32         window_flags,
  //pointer to window pointer          SDL_Window**   window,
  //pointer to renderer pointer        SDL_Renderer** renderer)
  SDL_CreateWindowAndRenderer(640, 480, 0, &this->_window, &this->_renderer);
  //void SDL_SetWindowTitle(SDL_Window* window, const char* title)
  SDL_SetWindowTitle(this->_window, "Game");
}

//destructor
Graphics::~Graphics()
{
  SDL_DestroyWindow(this->_window);
}

SDL_Surface* Graphics::loadImage(const std::string &spriteSheetFilePath){
  //this is going to require downloading SDL_Image
  if (_spriteSheets.count(spriteSheetFilePath) == 0){
    _spriteSheets[spriteSheetFilePath] = IMG_Load(spriteSheetFilePath.c_str());
  }

  return _spriteSheets[spriteSheetFilePath];
}

void Graphics::blitSurface(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture){
  SDL_RenderCopy(_renderer, srcTexture, srcRect, destRect);
}

void Graphics::render(){
  SDL_RenderPresent(_renderer);
}

void Graphics::clear(){
  SDL_RenderClear(_renderer);
}

SDL_Renderer* Graphics::getRenderer() const{
  return _renderer;
}
