#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include <string>

struct SDL_Window;
struct SDL_Renderer;



class Graphics{
public:
  Graphics();
  ~Graphics();

  /* Load images from sprite sheet into game */
  SDL_Surface* loadImage(const std::string &spriteSheetFilePath);

  //srcRect is where on sprite sheet we are loading, dest rect is where on screen we're loading
  void blitSurface(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle = 0, const SDL_Point* = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
  void blitSurfaceIgnoreCamera(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle = 0, const SDL_Point* = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);

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
};
#endif
