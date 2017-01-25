#ifndef DOOR_H
#define DOOR_H

#include "rectangle.h"
#include "conf.h"
#include <string>

struct SpawnPoint{
  std::string mapName;
  Vector2 point;
};

class Door : public Rectangle {
public:
  Door() {}
  Door(Rectangle r, SpawnPoint dest_map) : Rectangle(r.getLeft(), r.getTop(), r.getWidth(), r.getHeight()), _destination(dest_map)
  {}

  const inline Rectangle getRectangle() const
  {
    return this->getRect();
  }

  const inline std::string getDestination() const
  {
    return _destination.mapName;
  }
  
  const inline Vector2 getSpawn() const{
    return _destination.point;
  }

protected:
  SpawnPoint _destination;
};

#endif
