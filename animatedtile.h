//
//  animatedtile.h
//  sdl_engine
//
//  Created by Alex Fox on 8/31/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#ifndef ANIMTILE_H
#define ANIMTILE_H

#include "tile.h"
#include "conf.h"
#include <vector>
#include <map>

struct TileInfo{
public:
  int GID;
  int TilesetStartID;
  
  int frames;
  int currFrame;
  float time_on_frame;
  std::map<int, Vector2> frame_map;
  
  
  bool loop = true;
  bool finished = false;
};

struct TileAnim{
  std::vector<Vector2> positions;
  float duration;
  int firstID;
  SDL_Texture *tileset;
  Vector2 size;
};

class AnimatedTile : public Tile{
public:
  AnimatedTile(std::vector<Vector2> tilesetPositions, float duration, SDL_Texture *tileset, Vector2 size, Vector2 location);
  AnimatedTile(TileAnim infoA, Vector2 location);
  void update(float elapsedTime);
  void draw(Graphics &graphics);
  
protected:
  std::vector<Vector2> _tilesetPositions;
  float _duration;
  SDL_Texture* _tileset;
  Vector2 _size;
  Vector2 _location;
  TileInfo _info;
  
};

#endif /* defined(__sdl_engine__animatedtile__) */
