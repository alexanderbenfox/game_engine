#include <SDL2/SDL.h>
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "graphics.h"
#include "conf.h"
#include <iostream>

//constructor
Graphics::Graphics()
{
  TTF_Init();
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

void Graphics::blitSurfaceRenderText(std::string text, const char* fontLoc, int size, SDL_Color color, Vector2 position){
  if (font == NULL){
    printf("Error: %s", TTF_GetError());
    font = TTF_OpenFont(fontLoc, size*SPRITE_SCALE);
  }
  SDL_Surface* messageSurface = TTF_RenderText_Solid(font, text.c_str(), color);
  SDL_Texture* message = SDL_CreateTextureFromSurface(_renderer, messageSurface);
  
  
  //get the text size
  int width;
  int height;
  
  if(TTF_SizeText(font, text.c_str(), &width, &height) != 0){
    //failure
    printf("TTF_Error: %s\n",TTF_GetError());
    width = 100;
    height = 100;
  }
  
  SDL_Rect rect = {position.x, position.y, width, height};
  SDL_RenderCopy(_renderer, message, NULL, &rect);
  SDL_FreeSurface(messageSurface);
  SDL_DestroyTexture(message);
  //SDL_free(message);
  
}

void Graphics::getTextSize(int *width, int *height, std::string message){
  TTF_SizeText(font, message.c_str(), width, height);
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
