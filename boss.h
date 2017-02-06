#ifndef BOSS_H
#define BOSS_H
#include "enemy.h"

class Boss : public Enemy{
public:
  Boss();
  Boss(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw (Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
  
protected:
  void setupAnimations();
  void moveCurCollider(float x, float y, float width, float height);
  
};

class Snake : public Boss{
  
  Snake();
  Snake(Graphics &graphics, Vector2 spawnPoint);
  void update(float dt, Player &player);
  void draw (Graphics &graphics);
  void playerCollision(Player* player);
  
  void handleRightCollision(Rectangle tile);
  void handleLeftCollision(Rectangle tile);
  void handleUpCollision(Rectangle tile);
  void handleDownCollision(Rectangle tile);
  
  void applyGravity(float dt);
  
protected:
  void setupAnimations();
  void moveCurCollider(float x, float y, float width, float height);
  
  void LeapingAttack();
  bool _inLeapingAttack, _jump, _leapAscend, _leapPeak, _fall, _layout, _getup;
  
  void VenomSpitAttack();
  bool _inVenomSpitAttack, _ready, _charge, _spit;
  
  void TailStrike();
  bool _inTailStrike, _raiseTail, _waitForStrike, _striking;
  
  void resetAttackBools(){
    //Leaping Attack
    _jump = false;
    _leapAscend = false;
    _leapPeak = false;
    _fall = false;
    _layout = false;
    _getup = false;
    
    //Venom Spit Attack
    _ready = false;
    _charge = false;
    _spit = false;
    
    //Tail Strike Attack
    _raiseTail = false;
    _waitForStrike = false;
    _striking = false;
  }
  
  bool startLeapingAttack(){
    return !(_jump || _leapAscend || _leapPeak || _fall || _layout || _getup);
  }
  
  bool startVenomSpitAttack(){
    return !(_ready || _charge || _spit);
  }
  
  bool startTailStrikeAttack(){
    return !(_raiseTail || _waitForStrike || _striking);
  }
  
};



#endif