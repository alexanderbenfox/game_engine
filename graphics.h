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
  SDL_Surface *loadImage(const std::string &spriteSheetFilePath);

  void blitSurface(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture *srcTexture);

private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;

  std::map <std::string, SDL_Surface*> _spriteSheets;
};
#endif
