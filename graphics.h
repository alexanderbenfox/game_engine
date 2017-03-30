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

enum BLEND_TYPE{
  COMBINE_ALPHA,// Blend and combine src and dest alpha
  DEST_ALPHA,  // Blend and keep dest alpha
  SRC_ALPHA,  // Blend and keep src alpha
  COPY_SRC_ALPHA, // Direct copy with src alpha
  COPY_DEST_ALPHA, // Direct copy with dest alpha
  COPY_COMBINE_ALPHA, // Direct copy with combined alpha
  COPY_NO_ALPHA,  // Direct copy, alpha opaque
  COPY_ALPHA_ONLY,  // Direct copy of just the alpha
  COMBINE_ALPHA_ONLY,  // Blend of just the alpha
  COMBINE_ALPHA_DEST //Blend of alphas with dest being main one
};



class Graphics{
public:
  Graphics();
  ~Graphics();

  /* Load images from sprite sheet into game */
  SDL_Surface* loadImage(const std::string &spriteSheetFilePath);
  
  void blendPixelOnSurface(SDL_Surface *surface, Sint16 x, Sint16 y, Uint32 color, Uint8 alpha, BLEND_TYPE btype);
  Uint32 getPixelInfo(SDL_Surface *s, Sint16 x, Sint16 y);
  void createRectInts(SDL_Rect* rect, SDL_Rect* drect, int *lowX, int *lowY, int *highX, int *highY);
  void blendTextures(SDL_Surface* overlay, SDL_Rect* overlayRect, SDL_Surface* dest, SDL_Rect* destRect, BLEND_TYPE blend);
  
  //srcRect is where on sprite sheet we are loading, dest rect is where on screen we're loading
  void blitSurface(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle = 0, const SDL_Point* = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
  //void blitSurfaceAlphaMask(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle = 0, const SDL_Point* = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
  void blitSurfaceIgnoreCamera(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle = 0, const SDL_Point* = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
  void blitSurfaceRenderText(std::string text, const char* fontLoc, int size, SDL_Color color, Vector2 position, SDL_Surface* preloadedSurface = NULL);
  void getTextSize(int* width, int* height, std::string message);

  void render();
  void clear();

  SDL_Renderer* getRenderer() const;
  SDL_Window* getWindow() const;
  
  void setCamera(SDL_Rect* camera);
  SDL_Rect getCamera();
  
  void fillScreen(SDL_Color color);
  
  SDL_Surface* dith = NULL;
  SDL_Surface* ghostColor = NULL;
  SDL_Surface* ghostColorInit = NULL;
  SDL_Surface* blackSurface = NULL;
  
  SDL_Surface* messageSurface = NULL;
  TTF_Font* font = NULL;

private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  SDL_Surface* _surface;

  std::map <std::string, SDL_Surface*> _spriteSheets;
  
  SDL_Rect _camera;
};
#endif
