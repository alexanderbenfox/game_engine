#include "sfx.h"


SFXManager::SFXManager(){}

SFXManager::SFXManager(Graphics &graphics, bool arrow){
  if (!arrow){
    SFX_SPRITES dust(&graphics, 0.03, 3, 24, 0.05);
    SFX_SPRITES arrow_exp(&graphics, 0.01, 5, 27, 0.05);
    SFX_SPRITES splat(&graphics, 0.01, 6, 32, 0.06);
    SFX_SPRITES splat2(&graphics, 0.01, 5, 44,0.05);
    
    SFX_SPRITES explosion_(&graphics, 0.01, 3, 0, 0.03);
    SFX_SPRITES blood1_(&graphics, 0.01, 3, 3, 0.03);
    SFX_SPRITES blood2_(&graphics, 0.01, 3, 6, 0.03);
    
    SFX_SPRITES charge1(&graphics, 0.01,4,49,100, CHARGE1);
    SFX_SPRITES charge2(&graphics, 0.02,5,53,100, CHARGE2);
    
    SFX_SPRITES charged(&graphics, 0.03,4,59,100,CHARGED);
    
    SFX_SPRITES wings(&graphics, 0.015,5,63,(5*.015),WINGS);
    
    SFX_SPRITES pick(&graphics, 0.005,6,12,0.03,drop);
    
    _baseSprites[drop] = pick;
    _baseSprites[arrow_blast] = arrow_exp;
    _baseSprites[DUST] = dust;
    _baseSprites[splatter] = splat;
    _baseSprites[splatter2] = splat2;
    
    _baseSprites[explosion] = explosion_;
    _baseSprites[blood1] = blood1_;
    _baseSprites[blood2] = blood2_;
    _baseSprites[CHARGE1] = charge1;
    _baseSprites[CHARGE2] = charge2;
    _baseSprites[CHARGED] = charged;
    _baseSprites[WINGS] = wings;
  }
  else{
    SFX_SPRITES arrow_exp(&graphics, 0.01, 5, 27, 0.05);
    SFX_SPRITES splat(&graphics, 0.01, 6, 32, 0.06);
    _baseSprites[splatter] = splat;
    _baseSprites[arrow_blast] = arrow_exp;
    
  }
  
}

void SFXManager::addSFX(SFX_TYPES type, int x, int y, bool flipped, int scale){
  SFX newSFX(x,y,_baseSprites[type]);
  newSFX.setFlipped(flipped);
  newSFX.setScale(scale);
  _sfx.push_back(newSFX);
}

void SFXManager::endSFX(SFX_TYPES type){
  for (int i = 0; i < _sfx.size() ; i++){
    if(_sfx.at(i).type == type)
    {
      _sfx.at(i).finished();
      return;
    }
  }
}

void SFXManager::moveSFX(SFX_TYPES type, float dx, float dy){
  for (int i = 0; i < _sfx.size() ; i++){
    if(_sfx.at(i).type == type)
    {
      _sfx.at(i).move(dx,dy);
      return;
    }
  }
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
  for(int i = 0; i<_sfx.size(); i++){
    _sfx.at(i).draw(graphics);
  }
}