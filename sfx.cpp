#include "sfx.h"


SFXManager::SFXManager(){}

SFXManager::SFXManager(Graphics &graphics){
  SFX_SPRITES dust(&graphics, 0.03, 3, 24, 0.05);
  SFX_SPRITES arrow_exp(&graphics, 0.01, 5, 27, 0.05);
  SFX_SPRITES splat(&graphics, 0.01, 6, 32, 0.06);
  SFX_SPRITES splat2(&graphics, 0.01, 5, 44,0.05);
  
  SFX_SPRITES explosion_(&graphics, 0.01, 3, 0, 0.03);
  SFX_SPRITES blood1_(&graphics, 0.01, 3, 3, 0.03);
  SFX_SPRITES blood2_(&graphics, 0.01, 3, 6, 0.03);
  
  _baseSprites[arrow_blast] = arrow_exp;
  _baseSprites[DUST] = dust;
  _baseSprites[splatter] = splat;
  _baseSprites[splatter2] = splat2;
  
  _baseSprites[explosion] = explosion_;
  _baseSprites[blood1] = blood1_;
  _baseSprites[blood2] = blood2_;
  
}

void SFXManager::addSFX(SFX_TYPES type, int x, int y, bool flipped, int scale){
  SFX newSFX(x,y,_baseSprites[type]);
  newSFX.setFlipped(flipped);
  newSFX.setScale(scale);
  _sfx.push_back(newSFX);
  
}
void SFXManager::update(float dt){
  for(int i = 0; i < _sfx.size(); i++){
    _sfx[i].update(dt);
    if(_sfx[i]._done){
      _sfx.erase(_sfx.begin() + i);
    }
  }
}
void SFXManager::draw(Graphics &graphics){
  for(SFX sfx : _sfx){
    sfx.draw(graphics);
  }
}