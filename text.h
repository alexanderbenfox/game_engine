#ifndef TEXTDRAW_H
#define TEXTDRAW_H

#include "conf.h"
#include "graphics.h"

namespace TextRender{
  enum DrawColor{
    white, black
  };
  void RenderTextOnScreen(Graphics &graphics, std::string message, int size, DrawColor color, Vector2 position)
    {
      SDL_Color cSDL;
      switch (color) {
        case white:
          cSDL = SDL_Color{255,255,255};
          break;
        case black:
          cSDL = SDL_Color{0,0,0};
        default:
          break;
      }
      graphics.blitSurfaceRenderText(message, "content/font/pixel-font.ttf", size, cSDL, position);
    }
};

#endif