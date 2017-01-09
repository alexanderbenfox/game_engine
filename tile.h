//
//  tile.h
//  sdl_engine
//
//  Created by Alex Fox on 8/31/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#ifndef TILE_H
#define TILE_H

#include "conf.h"
#include "SDL2_image/SDL_image.h"
#include "graphics.h"

struct SDL_Texture;
class Graphics;

struct Tileset{
  SDL_Texture* texture;
  int firstGID;
  
  Tileset(){
    this->firstGID = -1;
  }
  Tileset(SDL_Texture* Texture, int FirstGID){
    this->texture = Texture;
    this->firstGID = FirstGID;
  }
};

class Tile{
public:
  Tile();
  Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 location);
  void update(float elapsedTime);
  void draw(Graphics &graphics);
protected:
  SDL_Texture* _tileset;
  Vector2 _size;
  Vector2 _tilesetPosition;
  Vector2 _location;
};

#endif /* defined(__sdl_engine__tile__) */
