#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Rect;
struct SDL_Point;
struct Vector2;



class Graphics{
public:
  Graphics();
  ~Graphics();

  /* Load images from sprite sheet into game */
  SDL_Surface* loadImage(const std::string &spriteSheetFilePath);

  //srcRect is where on sprite sheet we are loading, dest rect is where on screen we're loading
  void blitSurface(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle = 0, const SDL_Point* = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
  void blitSurfaceIgnoreCamera(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle = 0, const SDL_Point* = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
  void blitSurfaceRenderText(std::string text, const char* fontLoc, int size, SDL_Color color, Vector2 position);

  void render();
  void clear();

  SDL_Renderer* getRenderer() const;
  SDL_Window* getWindow() const;
  
  void setCamera(SDL_Rect* camera);
  SDL_Rect getCamera();
  
  void fillScreen(SDL_Color color);

private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  SDL_Surface* _surface;

  std::map <std::string, SDL_Surface*> _spriteSheets;
  
  SDL_Rect _camera;
  TTF_Font* font = NULL;
};
#endif
