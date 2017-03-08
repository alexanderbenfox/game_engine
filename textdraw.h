#ifndef TEXTDRAW_H
#define TEXTDRAW_H

#include "conf.h"
#include <string>

class Graphics;

enum DrawColor{
  white, black
};

class TextRender{
public:
  static void RenderTextOnScreen(Graphics &graphics, std::string message, int size, DrawColor color, Vector2 position);
  static void GetTextSize(Graphics &graphics, int* width, int* height, std::string message);
};

#endif