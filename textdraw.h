#ifndef TEXTDRAW_H
#define TEXTDRAW_H

#include "conf.h"
#include <string>

class Graphics;
struct SDL_Surface;

enum DrawColor{
  white, black
};

class TextRender{
public:
  static void RenderTextOnScreen(Graphics &graphics, std::string message, int size, DrawColor color, Vector2 position, SDL_Surface* preloadedSurface = NULL);
  static void GetTextSize(Graphics &graphics, int* width, int* height, std::string message);
  
  static SDL_Surface* createNewTextSurface(Graphics &graphics, std::string message,DrawColor color);
};

#endif