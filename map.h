//
//  map.h
//  sdl_engine
//
//  Created by Alex Fox on 8/31/16.
//  Copyright (c) 2016 Alex Fox. All rights reserved.
//

#ifndef MAP_H
#define MAP_H

#include "conf.h"
#include "tile.h"
#include "animatedtile.h"

#include "rectangle.h"
#include "slope.h"
#include "door.h"
#include "tinyxml2.h"
#include <string>
#include <vector>

class Enemy;
class Player;

//map contains world information and tile collision information
class Map{
public:
  Map();
  Map(std::string mapName, Graphics &graphics);
  ~Map();
  
  void update(float elapsedTime, Player &player);
  void draw(Graphics &graphics);
  
  std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
  std::vector<Rectangle> checkOneWayCollisions(const Rectangle &other);
  std::vector<Slope> checkSlopeCollisions(const Rectangle &other);
  std::vector<Door> checkDoorCollisions(const Rectangle &other);
  std::vector<Enemy*> checkEnemyCollisions(const Rectangle &other);
  
  const Vector2 getPlayerSpawnPoint() const;
  
  Vector2 getSize();
  
protected:
  std::string _mapName;
  Vector2 _spawnPoint;
  
  Vector2 _size;
  Vector2 _tilesize;
  
  //Graphics Information
  //tile and tileset data structures only contain graphics information for the tiles
  std::vector<Tile> _tiles;
  std::vector<Tileset> _tilesets;
  
  std::vector<AnimatedTile> _animTiles;
  std::vector<TileAnim> _animInfos;
  //this is where background information will most likely go
  std::vector<SDL_Texture*> _backgroundTextures;
  
  //Collision Information
  //Rectangles, doors, slopes, etc. contain collision information and no graphics info
  std::vector<Rectangle> _crectangles;
  std::vector<Rectangle> _onewayrectangles;
  std::vector<Slope> _slopes;
  std::vector<Door> _doors;
  
  //NPC Information
  //enemies, npcs, etc. will go here
  
  std::vector<Enemy*> _enemies;
  
  void handleEnemyCollisions(Enemy* enemy);
  
  
  //Helper Methods
  void loadMap(std::string mapName, Graphics &graphics);
  int getElementsFromXML(const char* element, tinyxml2::XMLElement* node);
  Vector2 getTilesetPosition(int GID, int tile_w, int tile_h, SDL_Texture* tileset);
  
  void LoadTilesets(int* mapNode, Graphics &graphics);
  void LoadLayers(int* mapNode, Graphics &graphics);
  void LoadObjects(int* mapNode, Graphics &graphics);
  void GenerateTile(int tileCounter, Tileset tileset, int gid);
  
  template<typename T>
  std::vector<T> overlapRectangles(const Rectangle &other, std::vector<T> mapRectangles)
  {
    std::vector<T> overlaps;
    for(int i = 0; i< mapRectangles.size(); i++)
    {
      if(mapRectangles.at(i).collidesWith(other))
      {
        overlaps.push_back(mapRectangles.at(i));
      }
    }
    return overlaps;
  }
};

#endif /* defined(MAP_H) */
