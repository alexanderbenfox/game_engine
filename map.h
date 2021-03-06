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
#include "savepoint.h"
#include "pickups.h"

#include "rectangle.h"
#include "slope.h"
#include "door.h"
#include "tinyxml2.h"
#include <string>
#include <vector>

class Enemy;
class Boss;
class Player;
class EnemyHitbox;

class TilesetLoader{
public:
  static TilesetLoader* getInstance(){
    static TilesetLoader instance;
    return &instance;
  }
  
  static SDL_Texture* getTexture(std::string filePath){
    if(getInstance()->tilesetLoaded(filePath)){
      return getInstance()->_textures[filePath];
    }
    else return NULL;
  }
  
  static std::map<std::string, SDL_Texture*> Textures(){
    return getInstance()->_textures;
  }
  
  static bool tilesetLoaded(std::string filePath){
    return (getInstance()->_textures.count(filePath) != 0);
  }
  
  static void addTexture(std::string filePath, SDL_Texture* texture){
    getInstance()->_textures[filePath] = texture;
  }
  
  TilesetLoader(TilesetLoader const&) = delete;
  void operator=(TilesetLoader const&) = delete;
private:
  TilesetLoader(){}
  std::map<std::string,SDL_Texture*> _textures;
};

//map contains world information and tile collision information
class Map{
public:
  Map();
  Map(std::string mapName, Graphics &graphics);
  ~Map();
  
  void deleteMap();
  
  template<typename T>
  void deleteAllObjects(std::vector<T> objects){
    
    if(objects.size() > 0){
      for(int i = 0; i<objects.size(); i++){
        delete objects.at(i);
        objects.erase(objects.begin() + i);
      }
    }
  }
  
  void update(float elapsedTime, Player &player);
  void draw(Graphics &graphics);
  
  std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
  std::vector<Rectangle> checkOneWayCollisions(const Rectangle &other);
  std::vector<Slope> checkSlopeCollisions(const Rectangle &other);
  std::vector<Door> checkDoorCollisions(const Rectangle &other);
  std::vector<Enemy*> checkEnemyCollisions(const Rectangle &other);
  std::vector<EnemyHitbox*> checkEnemyHitboxCollisions(const Rectangle &other);
  std::vector<EnemyHitbox*> checkEnemyHitboxForDestruction(const Rectangle &other);
  std::vector<Rectangle> checkCheckPointCollisions(const Rectangle &other);
  
  const Vector2 getPlayerSpawnPoint() const;
  void setCamera(SDL_Rect *camera);
  
  bool screenShakeTrigger();
  bool screenBigShakeTrigger();
  
  Vector2 getSize();
  
  std::string getMapName(){
    return _mapName;
  }
  
  void addItemToMap(Pickup* item);
  
protected:
  Sprite doorLock;
  bool _loaded;
  SDL_Rect _camera;
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
  std::vector<Tile*> _specialTiles;
  std::vector<SavePoint*> _checkpoints;
  std::vector<Pickup*> _items;
  
  //NPC Information
  //enemies, npcs, etc. will go here
  
  std::vector<Enemy*> _enemies;
  std::vector<Boss*> _bosses;
  
  void handleEnemyCollisions(Enemy* enemy);
  void handlePickupCollisions(Pickup* pickup);
  
  
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
  
private:
  bool _hitTrigger = false;
  bool _deathTrigger = false;
  static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch) {
    int pos = txt.find(ch);
    int initialPos = 0;
    strs.clear();
    while (pos != std::string::npos) {
      strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
      initialPos = pos + 1;
      
      pos = txt.find(ch, initialPos);
    }
    strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));
    
    return strs.size();
  }
};

#endif /* defined(MAP_H) */
