#include "gamestate.h"
#include "loop.h"

PlayState::PlayState(Graphics *graphics, cpu_clock *clock){
  transitionMap = false;
  camera.w = WINDOW_WIDTH;
  camera.h = WINDOW_HEIGHT;
  
  //this->map = Map("tut1",*graphics);
  this->map=Map("Map 5",*graphics);
  Vector2 spawnPoint = map.getPlayerSpawnPoint();
  
  this->player = Player(*graphics, "sprites/playerspritesheett.png",1 ,1, spawnPoint.x-32, spawnPoint.y - 60, 64, 32);
  player.playAnimation("Idle");
  player.setCamera(&camera, map.getSize(), true);
  player.changeRevivalPoint("Map 1-1", spawnPoint);
  pauseTime = 0;
  clockRef = clock;
}

void PlayState::setMap(std::string name, Graphics *graphics){
  this->map.deleteMap();
  this->map = Map(name, *graphics);
  player.setX(map.getPlayerSpawnPoint().x);
  player.setY(map.getPlayerSpawnPoint().y);
}

void PlayState::processInput(Input &input){
  float x = 0;
  float y = 0;
  bool crouching = false;
  
  if (input.keyWasPressed(SDL_SCANCODE_A))
  {
    if (input.keyIsHeld(SDL_SCANCODE_DOWN)){
      player.dropDown();
    }
    else
      player.jump();
  }
  if (input.keyIsHeld(SDL_SCANCODE_DOWN))
  {
    crouching = true;
  }
  if (input.keyIsHeld(SDL_SCANCODE_LEFT))
  {
    x -= 1000.0;
  }
  if (input.keyIsHeld(SDL_SCANCODE_RIGHT))
  {
    x += 1000.0;
  }
  if(input.keyIsHeld(SDL_SCANCODE_D)){
    player.chargeShot();
  }
  else{
    player.stopCharging();
  }
  if (input.keyWasReleased(SDL_SCANCODE_D))
  {
    player.shoot();
  }
  if(input.keyIsHeld(SDL_SCANCODE_S) && !player.chargeAttackInitiated){
    framesHeld++;
    float holdTime = 10*(clockRef->fps/60);
    if(framesHeld > holdTime){
      player.chargeAttack();
      player.chargeAttackInitiated = true;
    }
  }
  else{
    framesHeld = 0;
  }
  if (input.keyWasReleased(SDL_SCANCODE_S))
  {
    if(player.chargeAttackInitiated)
      player.chargeAttackInitiated = false;
  }
  if(input.keyWasPressed(SDL_SCANCODE_S)){
    player.attack();
  }
  if (input.keyWasPressed(SDL_SCANCODE_Q))
  {
    player.dodge();
  }
  
  if(input.keyWasPressed(SDL_SCANCODE_RETURN)){
    player.reset(this->map);
    if(player.getPopup())
      player.disablePopup();
  }
  
  if(input.keyWasPressed(SDL_SCANCODE_W)){
    player.useItem();
  }
  
  if(input.keyWasPressed(SDL_SCANCODE_UP)){
    transitionMap = true;
  }
  
  player.moveSprite(x,y);
  player.check_crouch(crouching);
  
}

void PlayState::update(float dt, Graphics *graphics){
  if(!player.getDead()){
    
    if (player.hitRegistered()){
      pauseTime = 8*(clockRef->fps/30);
    }
    
    if (player.hitRegistered() || pauseTime > 0){
      pauseTime -= (1);
    }
    else{
      if(!player.getPopup()){
        player.update(dt);
        player.applyGravity(dt);
        player.handleCollisions(map);
      }
      
      //transition to another room
      if (player.handleDoorCollisions(map) && transitionMap){
        if(!PersistentInfo::getRoomManager()->checkEnemyLock(map.getMapName())){
          Door newRoom = player.getNewRoom();
          this->map.deleteMap();
          this->map = Map(newRoom.getDestination(),*graphics);
          player.setX(newRoom.getSpawn().x);
          player.setY(newRoom.getSpawn().y);
          player.setCamera(&camera, map.getSize(),true);
        }
      }
    }
    
    //if(map.screenShakeTrigger())
    //  player.shakeCamera(5.0);
    if(map.screenBigShakeTrigger())
      player.shakeCamera(20.0);
    if(player.gotHit())
      player.shakeCamera(30.0);
    
    player.setCamera(&camera,map.getSize());
    
    
    map.setCamera(&camera);
    map.update(dt, player);
    player.updateAttachments(dt);
  }
  else{
    player.update(dt);
  }
  transitionMap = false;
}

void PlayState::draw(Graphics *graphics){
  if(!player.getDead())
    this->map.draw(*graphics);
  this->player.draw(*graphics);
  
  graphics->setCamera(&camera);
}