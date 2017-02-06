#include "boss.h"

void Snake::setupAnimations(){
  Vector2 offset = Vector2(0,0);
  
  //Animations for the leaping attack
  this->addAnimation(.05, 4, 0, 0, "Jump", 64, 32, offset,"main");
  this->addAnimation(.05, 4, 0, 0, "LeapAsend", 64, 32, offset,"main");
  this->addAnimation(.01, 1, 2, 0, "LeapPeak", 64, 32, offset,"main");
  this->addAnimation(.01, 1, 2, 0, "Fall", 64, 32, offset,"main");
  this->addAnimation(.01, 1, 2, 0, "Layout", 64, 32, offset,"main");
  this->addAnimation(.01, 1, 2, 0, "GetUp", 64, 32, offset,"main");
  
  //Animations for VenomSpitAttack
  this->addAnimation(.05, 4, 0, 0, "Ready", 64, 32, offset,"main");
  this->addAnimation(.05, 4, 0, 0, "Charge", 64, 32, offset,"main");
  this->addAnimation(.05, 4, 0, 0, "Spit", 64, 32, offset,"main");
  
  //Animations for TailStrike
  this->addAnimation(.05, 4, 0, 0, "RaiseTail", 64, 32, offset,"main");
  this->addAnimation(.05, 4, 0, 0, "WaitForStrike", 64, 32, offset,"main");
  this->addAnimation(.05, 4, 0, 0, "Striking", 64, 32, offset,"main");
  
  //Other animations
  this->addAnimation(.01, 1, 3, 0, "Idle", 64, 32, offset,"main");
  this->addAnimation(.04, 4, 4, 0, "Death", 64, 32, offset,"main");
  
  this->playAnimation("Idle");
  
}

void Snake::LeapingAttack(){
  if(startLeapingAttack()){
    resetAttackBools();
    _jump = true;
    _actionTimer = this->getAnimationTime("Jump");
    playAnimation("Jump");
  }
  else{
    if(_actionTimer >= 0){
      //switch to leap peak here
      if(_leapAscend){
        if(_dy > -500){
          _actionTimer = 0;
        }
      }
      if(_leapPeak){
        if(_dy > 0){
          _actionTimer = 0;
        }
      }
      if(_fall){
        if(_dy <= 0){
          _actionTimer = 0;
        }
      }
    }
    else{
      if(_jump){
        _dy -= 3000;
        _dx = 500*_direction;
        _jump = false;
        _leapAscend = true;
        this->playAnimation("LeapAscend");
        _actionTimer = 100;
        //change collider here?
      }
      else if(_leapAscend){
        _leapAscend = false;
        _leapPeak = true;
        this->playAnimation("LeapPeak");
        _actionTimer = 100;
      }
      else if(_leapPeak){
        _leapPeak = false;
        _fall = true;
        this->playAnimation("Fall");
        _actionTimer = 100;
      }
      else if(_fall){
        _fall = false;
        _layout = true;
        this->playAnimation("Layout");
        //change collider here
        _actionTimer = this->getAnimationTime("Layout");
      }
      else if(_layout){
        _dx = 0;
        _layout = false;
        _getup = true;
        this->playAnimation("GetUp");
        //change collider again here
        _actionTimer = this->getAnimationTime("GetUp");
      }
      else if(_getup){
        _getup = false;
        _inLeapingAttack = false;
      }
    }
  }
}

void Snake::VenomSpitAttack(){
  if(startVenomSpitAttack()){
    resetAttackBools();
    _ready = true;
    _actionTimer = this->getAnimationTime("Ready");
    playAnimation("Ready");
  }
  else{
    if(_actionTimer >= 0){
      //switch to leap peak here
      if(_leapAscend){
        if(_dy > -500){
          _actionTimer = 0;
        }
      }
      if(_leapPeak){
        if(_dy > 0){
          _actionTimer = 0;
        }
      }
      if(_fall){
        if(_dy <= 0){
          _actionTimer = 0;
        }
      }
    }
    else{
      if(_jump){
        _dy -= 3000;
        _dx = 500*_direction;
        _jump = false;
        _leapAscend = true;
        this->playAnimation("LeapAscend");
        _actionTimer = 100;
        //change collider here?
      }
      else if(_leapAscend){
        _leapAscend = false;
        _leapPeak = true;
        this->playAnimation("LeapPeak");
        _actionTimer = 100;
      }
      else if(_leapPeak){
        _leapPeak = false;
        _fall = true;
        this->playAnimation("Fall");
        _actionTimer = 100;
      }
      else if(_fall){
        _fall = false;
        _layout = true;
        this->playAnimation("Layout");
        //change collider here
        _actionTimer = this->getAnimationTime("Layout");
      }
      else if(_layout){
        _dx = 0;
        _layout = false;
        _getup = true;
        this->playAnimation("GetUp");
        //change collider again here
        _actionTimer = this->getAnimationTime("GetUp");
      }
      else if(_getup){
        _getup = false;
        _inLeapingAttack = false;
      }
    }
  }
}

void Snake::TailStrike(){
  
}