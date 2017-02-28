#ifndef INVENTORY_H
#define INVENTORY_H

#include "sprite.h"
#include "graphics.h"
#include <vector>

class Player;
class Sprite;

enum ItemType{
  gold, flask
};

class Item{
public:
  virtual void usePlayer(Player *player) = 0;
  
  virtual int getNum() = 0;
  virtual void addToInventory(int num) = 0;
  virtual Sprite* getSprite();
  virtual void carry();
  virtual bool isCarried();
  virtual ItemType getItemType();
  
protected:
  int _numInInventory;
  int _capacity;
  bool _carried = false;
  Sprite _graphic;
  ItemType _type;
};

class HealingFlask : public Item{
public:
  HealingFlask(){}
  HealingFlask(Graphics &graphics);
  void usePlayer(Player *player);
  int getNum();
  void addToInventory(int num);
  Sprite* getSprite();
};


class Inventory{
public:
  Inventory(){}
  Inventory(Graphics &graphics);
  void useCurrentItem(Player* player);
  Item* getCurrentItem();
  void cycleCurrentItem();
  void switchItem(ItemType type);
  void addToInventory(ItemType item);
  void changeCurrency(int num);
  int getCurrency();
private:
  Item* _currentItem;
  void setUpItems(Graphics &graphics);
  std::vector<ItemType> heldItems;
  int currency;
  
  HealingFlask _flask;
};

#endif