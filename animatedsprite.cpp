//
//  animatedsprite.cpp
//  sdl_engine
//
//  Created by Alex Fox on 7/18/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#include <stdio.h>
#include "animatedsprite.h"
#include "conf.h"

void AnimatedSprite::addAnimation(<#float _timeToUpdate#>, <#int _frames#>, <#int x#>, <#int y#>, <#std::string _name#>, <#int _width#>, <#int _height#>, Vector2 offset)
{
  ANIMATION newAnim;
  
  std::vector<SDL_Rect> anim_frames;
  for(int i = 0; i < _frames, i++){
    SDL_Rect newRect = { (i+x) * width, y, width, height};
    newAnim.frames.push_back(newRect);
  }
  newAnim.name = _name;
  newAnim.timeToUpdate = _timeToUpdate;
  newAnim.frames_no = _frames;
  newAnim.x = x;
  newAnim.y = y;
  newAnim.width = _width;
  newAnim.height = _height;
  newAnim.offset = offset;
  
  _animations.insert(std::pair<std::string, ANIMATION>(_name, newAnim));
}

void AnimatedSprite::resetAnimations(){
  _animations.clear();
}

void AnimatedSprite::resetCurrentAnimation(){
  _frameIndex = 0;
}

void AnimatedSprite::stopAnimation(){
  setVisible(false);
}

void AnimationSprite::setVisible(bool visible){
  _visible = visible;
}

void AnimatedSprite::playAnimation(std::string animName, bool once = false, bool reverse = false)
{
  _currentAnimationOnce = once;
  if(animName != _currentAnimation){
    _currentAnimation = animName;
    _frameIndex = 0;
  }
}

void AnimatedSprite::update(int elapsedTime){
  Sprite::update();
  _timeElapsed+=elapsedTime;
  if(_timeElapsed > _animations[_currentAnimation].timeToUpdate)
  {
    _timeElapsed -= _animations[_currentAnimation].timeToUpdate;
    if(frameIndex < _animations[_currentAnimation].frames_no)
    {
      _frameIndex++;
    }
    else
    {
      if(_currentAnimationOnce)
        stopAnimation();
      else
        _frameIndex = 0;
    }
  }
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y)
{
  if (_visible) {
    SDL_Rect destinationRectangle;
    destinationRectangle.x = x + _animations[_currentAnimation].offset.x;
    destinationRectangle.y = y + _animations[_currentAnimation].offset.y;
    destinationRectangle.w = _sourceRect.w * SPRITE_SCALE;
    destinationRectangle.h = _sourceRect.h * SPRITE_SCALE;
    
    SDL_Rect sourceRect = _animations[_currentAnimation].frames[_frameIndex];
    
    graphics.blitSurface(_spriteSheet, &sourceRect, &destinationRectangle);
  }
}





