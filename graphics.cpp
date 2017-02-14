#include <SDL2/SDL.h>
#include "SDL2_image/SDL_image.h"
#include "graphics.h"
#include "conf.h"
#include <iostream>

//constructor
Graphics::Graphics()
{
  std::cout<<"Graphics created\n";
  //int SDL_CreateWindowAndRenderer(int            width,
  //                        int            height,
  //                        Uint32         window_flags,
  //pointer to window pointer          SDL_Window**   window,
  //pointer to renderer pointer        SDL_Renderer** renderer)
  //void SDL_SetWindowTitle(SDL_Window* window, const char* title)
  //create window
  _window = SDL_CreateWindow( "Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 720, SDL_WINDOW_SHOWN );
  //std::cerr<<SDL_GetWindowSurface(_window);
  
  //_surface = SDL_GetWindowSurface(_window);

  //Create renderer for window
  _renderer = SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED );
  if(_renderer == NULL )
  {
    printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
  }
  else
  {
    //Initialize renderer color
    SDL_SetRenderDrawColor( _renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
      printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
  }
  SDL_SetWindowTitle(this->_window, "Game");
  }

//destructor
Graphics::~Graphics()
{
  std::cout<<"Graphics is destroyed";
  //Destroy window    
  SDL_DestroyRenderer( _renderer );
  SDL_DestroyWindow( _window );
  _window = NULL;
  _renderer = NULL;

  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

SDL_Surface* Graphics::loadImage(const std::string &spriteSheetFilePath){
  //this is going to require downloading SDL_Image
  if (_spriteSheets.count(spriteSheetFilePath) == 0){
    this->_spriteSheets[spriteSheetFilePath] = IMG_Load(spriteSheetFilePath.c_str());
  }

  if(!_spriteSheets[spriteSheetFilePath]) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }
  return this-> _spriteSheets[spriteSheetFilePath];
}



void Graphics::blitSurface(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle, const SDL_Point* point, const SDL_RendererFlip flip){
  int destRight = (destRect->x + destRect->w);
  int destLeft = destRect->x;
  int destTop = destRect->y;
  int destBottom = destRect->y + destRect->h;
  
  destRect->x -= _camera.x;
  destRect->y -= _camera.y;
  
  bool isInCameraRange =
  destRight >= _camera.x &&
  destLeft<= (_camera.x + _camera.w) &&
  destBottom >= _camera.y &&
  destTop <= (_camera.y + _camera.h);
  
  //if(destRect->x >= _camera.x && destRect->x <=)
  if(isInCameraRange)
    SDL_RenderCopyEx(_renderer, srcTexture, srcRect, destRect, angle, point, flip);
}

void Graphics::blitSurfaceIgnoreCamera(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle, const SDL_Point* point, const SDL_RendererFlip flip){
  SDL_RenderCopyEx(_renderer, srcTexture, srcRect, destRect, angle, point, flip);
}

void Graphics::fillScreen(SDL_Color color){
  SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(_renderer);
  SDL_RenderPresent(_renderer);
}

void Graphics::setCamera(SDL_Rect *camera){
  _camera.x = camera->x;
  _camera.y = camera->y;
  _camera.w = camera->w;
  _camera.h = camera->h;
}

SDL_Rect Graphics::getCamera(){
  return _camera;
}

void Graphics::render(){
  SDL_RenderPresent(_renderer);
}

void Graphics::clear(){
  SDL_RenderClear(_renderer);
}

SDL_Renderer* Graphics::getRenderer() const{
  return this->_renderer;
}

SDL_Window* Graphics::getWindow() const{
  return this->_window;
}
