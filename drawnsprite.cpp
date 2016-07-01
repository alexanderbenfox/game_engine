#include "sprite.h"
#include "graphics.h"

#include <string>

class DrawnSprite{
public:
  DrawnSprite(Graphics &graphics, int startX, int startY, int height, int width, const std::string &filePath);
  ~DrawnSprite();

  void draw(Graphics &graphics);
private:
  Sprite _theSprite;
  int _x;
  int _y;
};

DrawnSprite::DrawnSprite(Graphics &graphics, int startX, int startY, int height, int width, const std::string &filePath) : _x(startX), _y(startY){
  _theSprite = Sprite(graphics, filePath, 0, 0, startX, startY, width, height);

  graphics.loadImage(filePath);
}

DrawnSprite::~DrawnSprite(){}

void DrawnSprite::draw(Graphics &graphics){
  _theSprite.update();
  _theSprite.draw(graphics, _x, _y);
}
