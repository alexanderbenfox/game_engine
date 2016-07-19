//
//  animatedsprite.cpp
//  sdl_engine
//
//  Created by Alex Fox on 7/15/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#include <string>
#include <vector>
#include <map>

#include "sprite.h"

class Graphics;

struct ANIMATION{
  float timeToUpdate;
  int frames_no;
  int x, y;
  std::string name;
  int width;
  int height;
  Vector2 offset;
  std::vector<SDL_Rect> frames;
};

class AnimatedSprite : public Sprite{
public:
  AnimatedSprite() {}
  AnimatedSprite(Graphics &graphics, int startX, int startY, int height, int width, const std::string &filePath, float posX, float posY) : Sprite(graphics, filePath, startX, startY, posX, posY, width, height), _frameIndex(0), _visible(true), _currentAnimationOnce(false), _currentAnimation(""), _currentAnimationTimeToUpdate(0)
  {}
  ~AnimatedSprite();

  void playAnimation(std::string animName, bool once = false, bool reverse = false);
  void update(int elapsedTime);
  void draw(Graphics &graphics, int x, int y);
  std::string _currentAnimation;

protected:
  bool _currentAnimationOnce;
  bool _currentAnimationTimeToUpdate;
  int _x;
  int _y;
  
  void addAnimation(float _timeToUpdate, int _frames, int x, int y, std::string _name, int _width, int _height, Vector2 offset);

  void resetAnimations();
  void resetCurrentAnimation();

  void stopAnimation();

  void setVisible(bool visibile);

  virtual void setupAnimations() = 0;
  
private:

  std::map<std::string, ANIMATION > _animations;
  
  int _frameIndex;
  double _timeElapsed;
  bool _visible;
  
};

