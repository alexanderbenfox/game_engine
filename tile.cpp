//
//  tile.cpp
//  sdl_engine
//
//  Created by Alex Fox on 8/31/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#include "tile.h"

Tile::Tile()
{
  
}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 location)
{
  this->_tileset = tileset;
  this->_size = size;
  this->_tilesetPosition = tilesetPosition;
  this->_location = location;
}

void Tile::update(float elapsedTime)
{
  
}

void Tile::draw(Graphics &graphics)
{
  SDL_Rect sourceRect = {this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y};
  SDL_Rect destRect = {this->_location.x*SPRITE_SCALE, this->_location.y*SPRITE_SCALE, SPRITE_SCALE*this->_size.x, SPRITE_SCALE*this->_size.y};
  
  graphics.blitSurface(&sourceRect, &destRect, this->_tileset);
  
  
}
