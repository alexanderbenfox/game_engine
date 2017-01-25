//
//  tile.cpp
//  sdl_engine
//
//  Created by Alex Fox on 8/31/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#include "tile.h"
#include <iostream>
#include "player.h"

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

void Tile::update(float elapsedTime, Player &player)
{
  
}

void Tile::draw(Graphics &graphics)
{
  SDL_Rect sourceRect = {this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y};
  SDL_Rect destRect = {this->_location.x*SPRITE_SCALE, this->_location.y*SPRITE_SCALE, SPRITE_SCALE*this->_size.x, SPRITE_SCALE*this->_size.y};
  
  graphics.blitSurface(&sourceRect, &destRect, this->_tileset);
  
  
}



MovingTile::MovingTile()
{
  
}

MovingTile::MovingTile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 location) : Tile(tileset, size, tilesetPosition,location)
{
  this->_tileset = tileset;
  this->_size = size;
  this->_tilesetPosition = tilesetPosition;
  this->_location = location;
  _x = location.x;
  _y = location.y;
  _collider = Rectangle(_location.x, _location.y, _size.x*SPRITE_SCALE, _size.y*SPRITE_SCALE/2);
  _size = size;
  
  _speedx = 200;
  _speedy = 0;
  
}

void MovingTile::update(float elapsedTime, Player &player)
{
  
  float oldX = _x*SPRITE_SCALE;
  _x += _direction*ceil(_speedx*elapsedTime);
  _y += _direction*ceil(_speedy*elapsedTime);
  _collider.moveAnchor(_x*SPRITE_SCALE, _y*SPRITE_SCALE);
  float newX = _x *SPRITE_SCALE;
  
  if(_collider.collidesWith(player.getCollider())){
    collision_sides::Side side = player.Sprite::getCollisionSide(_collider);
    if (side != collision_sides::NONE){
      switch(side){
        case(collision_sides::BOTTOM) :
          player.movingPlatformStuff(_collider,(newX-oldX));
          break;
      }
    }
  }
  
  _directionTimer -= elapsedTime;
  if(_directionTimer <= 0){
    _direction*=-1;
    _directionTimer = _directionTimerMax;
  }
  
}

void MovingTile::draw(Graphics &graphics)
{
  SDL_Rect sourceRect = {this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y};
  SDL_Rect destRect = {_x*SPRITE_SCALE, _y*SPRITE_SCALE, SPRITE_SCALE*this->_size.x, SPRITE_SCALE*this->_size.y};
  
  graphics.blitSurface(&sourceRect, &destRect, this->_tileset);
}
