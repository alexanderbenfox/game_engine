#include "textdraw.h"
#include "graphics.h"

void TextRender::RenderTextOnScreen(Graphics &graphics, std::string message, int size, DrawColor color, Vector2 position)
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
  std::string filePath = "content/fonts/pixel-font.ttf";
  graphics.blitSurfaceRenderText(message, filePath.c_str(), size, cSDL, position);
}

void TextRender::GetTextSize(Graphics &graphics, int *width, int *height, std::string message){
  graphics.getTextSize(width, height, message);
}