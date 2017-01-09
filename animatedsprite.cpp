//
//  animatedsprite.cpp
//  sdl_engine
//
//  Created by Alex Fox on 7/18/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#include "animatedsprite.h"
#include "conf.h"

AnimatedSprite::AnimatedSprite()
{
}
AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath, int startX, int startY,float posX, float posY,int height, int width) : Sprite(graphics, filePath, startX, startY, posX, posY, width, height), _frameIndex(0), _visible(true), _currentAnimationOnce(false), _currentAnimation(""), _currentAnimationTimeToUpdate(0)
{
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::addAnimation(float _timeToUpdate, int _frames, int x, int y, std::string _name, int _width, int _height, Vector2 offset, std::string sheet)
{
  ANIMATION newAnim;
  
  std::vector<SDL_Rect> anim_frames;
  for(int i = 0; i < _frames; i++){
    SDL_Rect newRect = { (i+x) * _width, y, _width, _height};
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
  newAnim.sheet = sheet;
  
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

void AnimatedSprite::setVisible(bool visible){
  _visible = visible;
}

void AnimatedSprite::playAnimation(std::string animName, bool once, bool reverse)
{
  _currentAnimationOnce = once;
  //_currentAnimationDone = false;
  if(animName != _currentAnimation){
    _currentAnimation = animName;
    _frameIndex = 0;
    _timeElapsed = 0;
    std::string sheetName = _animations[_currentAnimation].sheet;
    _spriteSheet = _spriteSheets[sheetName].sheet;
    _sourceRect.w = _spriteSheets[sheetName].width;
    _sourceRect.h = _spriteSheets[sheetName].height;
  }
}

void AnimatedSprite::update(float elapsedTime){
  //Sprite::update();
  _timeElapsed+=elapsedTime;
  if(_timeElapsed > _animations[_currentAnimation].timeToUpdate)
  {
    float time = _timeElapsed;
    _timeElapsed -= _animations[_currentAnimation].timeToUpdate;
    if(_frameIndex < _animations[_currentAnimation].frames_no - 1)
    {
      int frameJump = time/_animations[_currentAnimation].timeToUpdate;
      int framesLeft = (_animations[_currentAnimation].frames_no - 1) - _frameIndex;
      if (frameJump > framesLeft){
        frameJump = framesLeft;
      }
      _frameIndex += frameJump;
    }
    else
    {
      if(_currentAnimationOnce){
        _frameIndex = _animations[_currentAnimation].frames_no-1;
        _currentAnimationDone = true;
      }
      else
        _frameIndex = 0;
    }
  }
}

float AnimatedSprite::getAnimationTime(std::string name){
  return (float)_animations[name].timeToUpdate * (float)_animations[name].frames_no;
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y)
{
  if(_animations.size() <= 0)
  {
    Sprite::draw(graphics, x, y);
  }
  else if (_visible) {
    SDL_Rect destinationRectangle;
    destinationRectangle.x = x + _animations[_currentAnimation].offset.x*_spriteScale;
    destinationRectangle.y = y + _animations[_currentAnimation].offset.y*_spriteScale;
    destinationRectangle.w = _sourceRect.w * _spriteScale;//SPRITE_SCALE;
    destinationRectangle.h = _sourceRect.h * _spriteScale;//SPRITE_SCALE;
    
    SDL_Rect sourceRect = _animations[_currentAnimation].frames[_frameIndex];
    SDL_RendererFlip flip = _flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    
    if(_flipped){
      SHEET currSheet = _spriteSheets[_animations[_currentAnimation].sheet];
      int sheet_width = currSheet.width;
      destinationRectangle.x = x + (_collider.getWidth() - sheet_width*_spriteScale -_animations[_currentAnimation].offset.x*_spriteScale);
    }
      
    
    graphics.blitSurface(&sourceRect, &destinationRectangle,_spriteSheet, 0, NULL,flip);
  }
}





