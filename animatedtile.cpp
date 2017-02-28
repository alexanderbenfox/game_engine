//
//  animatedtile.cpp
//  sdl_engine
//
//  Created by Alex Fox on 8/31/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#include "animatedtile.h"
#include <iostream>

AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPositions, float duration, SDL_Texture *tileset, Vector2 size, Vector2 location)
{
  this->_tilesetPositions = tilesetPositions;
  this->_duration = duration/5000.0;
  this->_tileset = tileset;
  this->_size = size;
  this->_location = location;
  
  int frames = (int)tilesetPositions.size()-1;
  
  TileInfo info;
  info.frames = frames;
  info.currFrame = 0;
  info.time_on_frame = 0.0;
  
  int i = 0;
  std::map<int, Vector2> frame_map;
  for(i = 0; i< frames; i++)
  {
    std::pair<int, Vector2> pair(i, tilesetPositions[i]);
    frame_map.insert(pair);
  }
  
  info.frame_map = frame_map;
  
  this->_info = info;
}

AnimatedTile::AnimatedTile(TileAnim infoA, Vector2 location){
  this->_tilesetPositions = infoA.positions;
  this->_duration = infoA.duration;
  this->_tileset = infoA.tileset;
  this->_size = infoA.size;
  this->_location = location;
  
  int frames = (int)infoA.positions.size();
  
  TileInfo info;
  info.frames = frames;
  info.currFrame = 0;
  info.time_on_frame = 0;
  
  int i = 0;
  std::map<int, Vector2> frame_map;
  for(i = 0; i< frames; i++)
  {
    std::pair<int, Vector2> pair(i, infoA.positions[i]);
    frame_map.insert(pair);
  }
  
  info.frame_map = frame_map;
  
  this->_info = info;
}

void AnimatedTile::update(float elapsedTime)
{
  this->_info.time_on_frame += elapsedTime;

  if(this->_info.time_on_frame >= _duration)
  {
    if(_info.currFrame == _info.frames){
      if(_info.loop){
        _info.currFrame = 0;
      }
    }
    else{
      _info.currFrame += 1;
    }
    _info.time_on_frame = 0;
  }
}

void AnimatedTile::draw(Graphics &graphics)
{
  //first get info for which frame you are on
  Vector2 currFramePosition = _tilesetPositions[_info.currFrame];
  
  SDL_Rect sourceRect = {currFramePosition.x, currFramePosition.y, _size.x, _size.y};
  SDL_Rect destRect = {_location.x*SPRITE_SCALE, _location.y*SPRITE_SCALE, SPRITE_SCALE*_size.x, SPRITE_SCALE*_size.y};
  
  graphics.blitSurface(&sourceRect, &destRect, _tileset);
}