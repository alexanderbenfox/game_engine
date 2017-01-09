//
//  level.cpp
//  sdl_engine
//
//  Created by Alex Fox on 8/31/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#include "map.h"
#include "graphics.h"
#include "conf.h"
#include "enemy.h"

#include "tinyxml2.h"

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>


using namespace tinyxml2;

Map::Map(){
  
}

Map::Map(std::string mapName, Graphics &graphics){
  this->_mapName = mapName;
  this->_size = Vector2(0,0);
  
  loadMap(this->_mapName, graphics);
}

Map::~Map(){
  
}

const Vector2 Map::getPlayerSpawnPoint() const{
  return _spawnPoint;
}

void Map::loadMap(std::string mapName, Graphics &graphics){
  
  XMLDocument doc;
  std::stringstream ss;
  
  //ss << "maps/" << mapName << ".tmx";
  ss << "/Users/alex/GithubProjects/game_engine/maps/" << mapName << ".tmx";
  const char* map_file_name = ss.str().c_str();
  FILE* mapFile;
  mapFile = fopen(map_file_name, "r");
  doc.LoadFile(mapFile);
  std::cout<<doc.ErrorID();
  
  XMLElement* mapNode = doc.FirstChildElement("map");
  
  this->_size = Vector2(getElementsFromXML("width", mapNode),getElementsFromXML("height", mapNode));
  this->_tilesize = Vector2(getElementsFromXML("tilewidth", mapNode), getElementsFromXML("tileheight", mapNode));
  
  //Load Tilesets
  LoadTilesets((int*)mapNode, graphics);
  
  //Load Layers
  LoadLayers((int*)mapNode, graphics);
  
  //Load slopes and other objects
  LoadObjects((int*)mapNode, graphics);
  
  std::cout<<"done"<<std::endl;
}

void Map::handleEnemyCollisions(Enemy* enemy){
  Rectangle enemyCol = enemy->getCollider();
  std::vector<Rectangle> tiles = checkTileCollisions(enemyCol);
  for (Rectangle tile : tiles){
    collision_sides::Side side = enemy->Sprite::getCollisionSide(tile);
    if (side != collision_sides::NONE){
      switch(side){
        case(collision_sides::RIGHT) :
          enemy->handleRightCollision(tile);
          break;
        case(collision_sides::LEFT) :
          enemy->handleLeftCollision(tile);
          break;
        case(collision_sides::TOP) :
          enemy->handleUpCollision(tile);
          break;
        case(collision_sides::BOTTOM) :
          enemy->handleDownCollision(tile);
          break;
      }
    }
  }
}

void Map::update(float elapsedTime, Player &player){
  for (int i = 0; i<_animTiles.size();i++){
    _animTiles.at(i).update(elapsedTime);
  }
  
  for (int i = 0; i < _enemies.size(); i++){
    handleEnemyCollisions(_enemies.at(i));
    _enemies.at(i)->update(elapsedTime, player);
    if(_enemies.at(i)->isDead()){
      _enemies.erase(_enemies.begin() + i);
    }
  }
}

void Map::draw(Graphics &graphics){
  for (int i = 0; i<_tiles.size(); i++){
    _tiles.at(i).draw(graphics);
  }
  
  for (int i = 0; i<_animTiles.size(); i++){
    _animTiles.at(i).draw(graphics);
  }
  
  for (int i = 0; i < _enemies.size(); i++){
    _enemies.at(i)->draw(graphics);
  }
  
  /*for (Enemy e : _enemies){
    Rectangle c = e.getCollider();
    SDL_RenderDrawLine(graphics.getRenderer(), c.getLeft(),c.getTop(),c.getLeft()+c.getWidth(),c.getTop());
    SDL_RenderDrawLine(graphics.getRenderer(), c.getLeft(),c.getTop(),c.getLeft(),c.getBottom());
    SDL_RenderDrawLine(graphics.getRenderer(), c.getRight(),c.getTop(),c.getRight(),c.getBottom());
    SDL_RenderDrawLine(graphics.getRenderer(), c.getLeft(),c.getBottom(),c.getLeft()+c.getWidth(),c.getBottom());
  }*/
}

int Map::getElementsFromXML(const char* element, tinyxml2::XMLElement *node){
  int temp;
  XMLElement* temp_node = (XMLElement*)node;
  temp_node->QueryIntAttribute(element,&temp);
  return temp;
}

Vector2 Map::getTilesetPosition(int GID, int tile_w, int tile_h, SDL_Texture* tileset)
{
  Vector2 positionOnTileset;
  
  int tileset_w, tileset_h;
  int pos_x, pos_y;
  
  SDL_QueryTexture(tileset, NULL, NULL, &tileset_w, &tileset_h);
  
  pos_y = (GID * tile_w / tileset_w) * tile_w;
  pos_x = (GID * tile_w) % tileset_w;
  
  positionOnTileset.x = pos_x;
  positionOnTileset.y = pos_y;
  return positionOnTileset;
  
}

void Map::LoadTilesets(int* mapNode, Graphics &graphics)
{
  
  
  XMLElement* pTileset = ((XMLElement*)mapNode)->FirstChildElement("tileset");
  if (pTileset != NULL)
  {
    while(pTileset)
    {
      int firstGID;
      const char* source = pTileset->FirstChildElement("image")->Attribute("source");
      char* path;
      
      std::stringstream ss;
      
      ss << source;
      firstGID = getElementsFromXML("firstgid", pTileset);
      
      SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
      
      //put new tileset into the data structure
      this->_tilesets.push_back(Tileset(tex,firstGID));
      
      XMLElement* pTileAnim = pTileset->FirstChildElement("tile");
      if(pTileAnim != NULL)
      {
        while(pTileAnim)
        {
          int tileGID = pTileAnim->IntAttribute("id") + firstGID;
          XMLElement* pTileAnimInfo = pTileAnim -> FirstChildElement("animation");
          
          if(pTileAnimInfo != NULL)
          {
            std::vector<Vector2> framePositions;
            TileAnim anim;
            float frame_duration = 0.0;
            
            while(pTileAnimInfo)
            {
              XMLElement* frame = pTileAnimInfo->FirstChildElement("frame");
              if(frame != NULL)
              {
                //pull the vector info out of the frame info
                while(frame)
                {
                  int frame_id = frame->IntAttribute("tileid");
                  frame_duration = frame->FloatAttribute("duration");
                  
                  int tile_w = this->_tilesize.x;
                  int tile_h = this->_tilesize.y;
                  
                  
                  framePositions.push_back(getTilesetPosition(frame_id, tile_w, tile_h, tex));
                  frame = frame->NextSiblingElement("frame");
                }
              }
              
              anim.positions = framePositions;
              anim.duration = frame_duration;
              anim.size = this->_tilesize;
              anim.tileset = tex;
              anim.firstID = tileGID;
              this->_animInfos.push_back(anim);
              
              pTileAnimInfo = pTileAnimInfo->NextSiblingElement("animation");
            }
          }
          pTileAnim = pTileAnim->NextSiblingElement("tile");
        }
      }
      pTileset = pTileset->NextSiblingElement("tileset");
    }
  }
}

void Map::LoadLayers(int *mapNode, Graphics &graphics)
{
  XMLElement* layer = ((XMLElement*)mapNode)->FirstChildElement("layer");
  
  if(layer != NULL)
  {
    while(layer)
    {
      const char* name = layer->Attribute("name");
      XMLElement* data = layer->FirstChildElement("data");
      if (data != NULL)
      {
        while(data)
        {
          XMLElement* tile = data->FirstChildElement("tile");
          if(tile != NULL)
          {
            int tileCounter = 0;
            while(tile)
            {
              if(tile->IntAttribute("gid") == 0)
              {
                tileCounter++;
                if(tile->NextSiblingElement("tile"))
                  {tile = tile->NextSiblingElement("tile"); continue;}
                else
                  break;
              }
              
              
              int gid = tile->IntAttribute("gid");
              if (gid > 0)
              {
                std::cout<<"ok."<<std::endl;
              }
              Tileset tls;
              int closest = 0;
              
              
              for(int i = 0; i < this->_tilesets.size(); i++)
              {
                if(_tilesets[i].firstGID <= gid)
                {
                  if(_tilesets[i].firstGID > closest)
                  {
                    //get tileset with the closest GID
                    closest = _tilesets[i].firstGID;
                    //set used tileset to that tileset
                    tls = _tilesets.at(i);
                  }
                }
              }
              
              
              //if no tileset was found
              if(tls.firstGID == -1)
              {
                tileCounter++;
                if(tile->NextSiblingElement("tile"))
                  {tile = tile->NextSiblingElement("tile"); continue;}
                else
                  break;
              }
              
              GenerateTile(tileCounter, tls, gid);
              tileCounter++;
              tile = tile->NextSiblingElement("tile");
              
            }
          }
          data = data -> NextSiblingElement("data");
        }
      }
      layer = layer -> NextSiblingElement("layer");
    }
    
  }
  
}

void Map::LoadObjects(int *mapNode, Graphics &graphics)
{
  XMLElement* objectGroup = ((XMLElement*)mapNode)->FirstChildElement("objectgroup");
  if(objectGroup)
  {
    while(objectGroup)
    {
      const char* objName = objectGroup->Attribute("name");
      if(strcmp(objName, "collisions") == 0)
      {
        XMLElement* collision = objectGroup->FirstChildElement("object");
        while(collision != NULL)
        {
          int x = std::ceil(collision->FloatAttribute("x")) * SPRITE_SCALE;
          int y = std::ceil(collision->FloatAttribute("y")) * SPRITE_SCALE;
          int width = std::ceil(collision->FloatAttribute("width")) * SPRITE_SCALE;
          int height = std::ceil(collision->FloatAttribute("height")) * SPRITE_SCALE;
          
          Rectangle col = Rectangle(x, y, width, height);
          _crectangles.push_back(col);
          
          collision = collision->NextSiblingElement("object");
        }
      }
      
      if (strcmp(objName, "onewayplatform") == 0)
      {
        XMLElement* collision = objectGroup->FirstChildElement("object");
        while(collision != NULL)
        {
          int x = std::ceil(collision->FloatAttribute("x")) * SPRITE_SCALE;
          int y = std::ceil(collision->FloatAttribute("y")) * SPRITE_SCALE;
          int width = std::ceil(collision->FloatAttribute("width")) * SPRITE_SCALE;
          int height = std::ceil(collision->FloatAttribute("height")) * SPRITE_SCALE;
          
          Rectangle col = Rectangle(x, y, width, height);
          _onewayrectangles.push_back(col);
          
          collision = collision->NextSiblingElement("object");
        }
        
      }
      
      if(strcmp(objName,"slopes") == 0)
      {
        XMLElement* slope = objectGroup->FirstChildElement("object");
        while(slope != NULL)
        {
          int x = std::ceil(slope->FloatAttribute("x")) * SPRITE_SCALE;
          int y = std::ceil(slope->FloatAttribute("y")) * SPRITE_SCALE;
          //int width = std::ceil(slope->FloatAttribute("width")) * SPRITE_SCALE;
          //int height = std::ceil(slope->FloatAttribute("height")) * SPRITE_SCALE;
          
          int width = 32*SPRITE_SCALE;
          int height = 32 *SPRITE_SCALE;
          
          XMLElement* property = slope->FirstChildElement("properties")->FirstChildElement();
          
          const char* direction = property->Attribute("value");
          
          Vector2 p1 = Vector2(0, 0);
          Vector2 p2 = Vector2(0, 0);
          
          if (strcmp(direction, "RL")) //if slope is rising to the right
          {
            int p1_x = x;
            int p1_y = y+height;
            int p2_x = x+width;
            int p2_y = y;
            
            p1 = Vector2(p1_x,p1_y);
            p2 = Vector2(p2_x,p2_y);
          }
          else if (strcmp(direction, "LR")) //if slope is rising to the left
          {
            int p1_x = x;
            int p1_y = y;
            int p2_x = x+width;
            int p2_y = y+height;
            
            p1 = Vector2(p1_x,p1_y);
            p2 = Vector2(p2_x,p2_y);
          }
          
          _slopes.push_back(Slope(p1,p2));
          slope = slope->NextSiblingElement("object");
          
        }
        
      }
      
      if(strncmp(objName, "dddoors",5) == 0)
      {
        XMLElement* door = objectGroup->FirstChildElement("object");
        while(door!=NULL)
        {
          int x = std::ceil(door->FloatAttribute("x")) * SPRITE_SCALE;
          int y = std::ceil(door->FloatAttribute("y")) * SPRITE_SCALE;
          int width = std::ceil(door->FloatAttribute("width")) * SPRITE_SCALE;
          int height = std::ceil(door->FloatAttribute("height")) * SPRITE_SCALE;
          
          Rectangle doorCol = Rectangle(x, y, width, height);
          door = objectGroup->NextSiblingElement("object");
          
          
        }
        
      }
      
      if(strncmp(objName, "spawn points",5) == 0)
      {
        XMLElement* spawn = objectGroup->FirstChildElement("object");
        while(spawn != NULL){
          int x = std::ceil(spawn->FloatAttribute("x")) * SPRITE_SCALE;
          int y = std::ceil(spawn->FloatAttribute("y")) * SPRITE_SCALE;
          _spawnPoint = Vector2(x,y);
          spawn = spawn ->NextSiblingElement("object");
        }
        
      }
      
      if(strcmp(objName,"enemies") == 0)
      {
        XMLElement* enemy = objectGroup->FirstChildElement("object");
        while(enemy != NULL){
          int x = std::ceil(enemy->FloatAttribute("x")) * SPRITE_SCALE;
          int y = std::ceil(enemy->FloatAttribute("y")) * SPRITE_SCALE;
        
          const char* enemyType = enemy->Attribute("name");
        
          if(strcmp(enemyType, "Walker") == 0){
            _enemies.push_back(new Walker(graphics,Vector2(x,y)));
            
          }
          
          enemy = enemy->NextSiblingElement("object");
        }
        
      }
      
      objectGroup = objectGroup->NextSiblingElement("objectgroup");
    }
  }
  
}

Vector2 Map::getSize(){
  return Vector2(_size.x*_tilesize.x,_size.y*_tilesize.y);
}

void Map::GenerateTile(int tileCounter, Tileset tileset, int gid)
{
  int mapWidth = _size.x;
  int mapHeight = _size.y;
  bool isAnimated = false;
  
  int xx = (tileCounter % mapWidth) * _tilesize.x;
  int yy = (tileCounter / mapWidth) * _tilesize.y;
  
  Vector2 tilePosition = Vector2(xx,yy);
  Vector2 tilesetPosition = getTilesetPosition(gid-1, _tilesize.x, _tilesize.y, tileset.texture);
  
  TileAnim animTileInfo;
  for (int i = 0; i<_animInfos.size(); i++)
  {
    if(_animInfos.at(i).firstID == gid)
    {
      animTileInfo = _animInfos.at(i);
      isAnimated = true;
      break;
    }
  }
  
  if (isAnimated)
  {
    _animTiles.push_back(AnimatedTile(animTileInfo.positions, animTileInfo.duration, tileset.texture, animTileInfo.size, tilePosition));
  }
  else
  {
     _tiles.push_back(Tile(tileset.texture, _tilesize, tilesetPosition, tilePosition));
  }
}


std::vector<Rectangle> Map::checkTileCollisions(const Rectangle &other){
  return overlapRectangles(other,_crectangles);
}

std::vector<Rectangle> Map::checkOneWayCollisions(const Rectangle &other){
  return overlapRectangles(other, _onewayrectangles);
}

std::vector<Slope> Map::checkSlopeCollisions(const Rectangle &other){
  return overlapRectangles(other,_slopes);
}

std::vector<Door> Map::checkDoorCollisions(const Rectangle &other){
  return overlapRectangles(other, _doors);
}

std::vector<Enemy*> Map::checkEnemyCollisions(const Rectangle &other){
  std::vector<Enemy*> overlaps;
  for(int i = 0; i< _enemies.size(); i++)
  {
    if(_enemies.at(i)->collidesWith(other))
    {
      overlaps.push_back(_enemies.at(i));
    }
  }
  return overlaps;
}


