//
//  animatedsprite.cpp
//  sdl_engine
//
//  Created by Alex Fox on 7/15/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//
#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <string>
#include <vector>
#include <map>

#include "sprite.h"
#include "graphics.h"

struct Ghost{
  SDL_Rect screenRect;
  SDL_Rect sheetRect;
  
  SDL_Surface* sheet;
  SDL_Surface* blendedSheet;
  
  SDL_RendererFlip flip;
  
  float maxLifetime,lifetime;
  
  Ghost(SDL_Rect screen, SDL_Rect srect, SDL_Surface* s, SDL_RendererFlip f){
    screenRect = screen;
    sheetRect = srect;
    sheet = s;
    flip = f;
    
    maxLifetime = .1;
    lifetime = .1;
    
    blendedSheet = NULL;
  }
  
  ~Ghost(){
    if(blendedSheet != NULL){
      SDL_FreeSurface(blendedSheet);
    }
  }
};

struct ANIMATION{
  float timeToUpdate;
  int frames_no;
  int x, y;
  std::string name, sheet;
  int width;
  int height;
  Vector2 offset;
  std::vector<SDL_Rect> frames;
};

class Graphics;

class AnimatedSprite : public Sprite{
public:
  AnimatedSprite();
  AnimatedSprite(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width);
  ~AnimatedSprite();
  void addAnimation(float _timeToUpdate, int _frames, int x, int y, std::string _name, int _width, int _height, Vector2 offset, std::string sheet);

  void playAnimation(std::string animName, bool once = false, bool reverse = false);
  void update(float elapsedTime);
  void draw(Graphics &graphics, int x, int y);
  void drawTest(Graphics &graphics, int x, int y);
  void drawGhost(Graphics &graphics, Ghost ghost);
  void createGhost(int x, int y);
  std::string _currentAnimation;
  float getAnimationTime(std::string name);
  
  void setVisible(bool visibile);
  
  void setXPlease(float x){
    _x = x;
  }
  
  void setYPlease(float y){
    _y = y;
  }

protected:
  bool _currentAnimationOnce;
  bool _currentAnimationTimeToUpdate;
  int _x;
  int _y;
  
  bool _currentAnimationDone;

  void resetAnimations();
  void resetCurrentAnimation();

  void stopAnimation();
  
  std::vector<Ghost> _ghosts;
  //std::vector<SDL_Surface*>

  //virtual void setupAnimations() = 0;
  
private:

  std::map<std::string, ANIMATION > _animations;
  
  int _frameIndex;
  float _timeElapsed;
  bool _visible;
  
};

#endif

