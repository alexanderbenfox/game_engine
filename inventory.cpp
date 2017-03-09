#include "inventory.h"
#include "player.h"

Sprite* Item::getSprite(){
  return &_graphic;
}

void Item::carry(){
  _carried = true;
}

bool Item::isCarried(){
  return _carried;
}

ItemType Item::getItemType(){
  return _type;
}

HealingFlask::HealingFlask(Graphics &graphics)
{
  _graphic = Sprite(graphics, "sprites/healingflask.png", 0, 0, 1, 1, 32, 32);
  _graphic.setHUD();
  _graphic._spriteScale = 2;
  _numInInventory = 2;
  _capacity = 2;
  _type = flask;
}

void HealingFlask::usePlayer(Player *player){
  if(_numInInventory > 0){
    addToInventory(-1);
    player->changeHealth(5);
  }
  
}
int HealingFlask::getNum(){
  return _numInInventory;
}
void HealingFlask::addToInventory(int num){
  if((_numInInventory + num) <= _capacity){
    _numInInventory += num;
    if(_numInInventory < 0){
      _numInInventory = 0;
    }
  }
  else{
    _numInInventory = _capacity;
  }
}

void HealingFlask::addToCapacity(int num){
  _capacity += num;
}

Sprite* HealingFlask::getSprite(){
  return Item::getSprite();
}

Inventory::Inventory(Graphics &graphics) : _flask(HealingFlask(graphics)), currency(0){
  setUpItems(graphics);
  
  //add flask item to inventory and set it to the current item
  addToInventory(flask);
  _currentItem = &_flask;
}

void Inventory::useCurrentItem(Player *player){
  _currentItem->usePlayer(player);
}

Item* Inventory::getCurrentItem(){
  return _currentItem;
}

void Inventory::cycleCurrentItem(){
  for(int i = 0; i < heldItems.size(); i++){
    if(heldItems.at(i) == _currentItem->getItemType()){
      //get next item
      int index = i+1;
      
      if(index == heldItems.size()){
        index = 0;
      }
      switchItem(heldItems.at(index));
      break;
    }
  }
}

void Inventory::switchItem(ItemType type){
  switch(type){
    case flask:
      if(_flask.isCarried())
        _currentItem = &_flask;
      break;
    default:
      break;
  }
}

void Inventory::addToInventory(ItemType item){
  switch(item){
    case flask:
      if(!_flask.isCarried()){
        _flask.carry();
        heldItems.push_back(item);
      }
      break;
    default:
      break;
  }
}

void Inventory::changeCurrency(int num){
  currency += num;
}

int Inventory::getCurrency(){
  return currency;
}

void Inventory::setUpItems(Graphics &graphics){
  //_flask = HealingFlask(graphics);
}

