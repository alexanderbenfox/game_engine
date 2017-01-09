#include "loop.h"
#include <iostream>


Timer::Timer()
{
  //Initialize the variables
  startTicks = 0;
  pausedTicks = 0;
  paused = false;
  started = false;
}

void Timer::start()
{
  //Start the timer
  started = true;
  
  //Unpause the timer
  paused = false;
  
  //Get the current clock time
  startTicks = SDL_GetTicks();
}

void Timer::stop()
{
  //Stop the timer
  started = false;
  
  //Unpause the timer
  paused = false;
}

void Timer::pause()
{
  //If the timer is running and isn't already paused
  if( ( started == true ) && ( paused == false ) )
  {
    //Pause the timer
    paused = true;
    
    //Calculate the paused ticks
    pausedTicks = SDL_GetTicks() - startTicks;
  }
}

void Timer::unpause()
{
  //If the timer is paused
  if( paused == true )
  {
    //Unpause the timer
    paused = false;
    
    //Reset the starting ticks
    startTicks = SDL_GetTicks() - pausedTicks;
    
    //Reset the paused ticks
    pausedTicks = 0;
  }
}

int Timer::get_ticks()
{
  //If the timer is running
  if( started == true )
  {
    //If the timer is paused
    if( paused == true )
    {
      //Return the number of ticks when the timer was paused
      return pausedTicks;
    }
    else
    {
      //Return the current time minus the start time
      return SDL_GetTicks() - startTicks;
    }
  }
  
  //If the timer isn't running
  return 0;
}

bool Timer::is_started()
{
  return started;
}

bool Timer::is_paused()
{
  return paused;
}


GameLoop::GameLoop()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  //if(SDL_Init(SDL_INIT_VIDEO) < 0)
  //{
  //  printf("SDL could not initialize! SDL error %s\n", SDL_GetError());
  //}
  this->loop();
}

GameLoop::~GameLoop()
{

}


bool GameLoop::handle_events(SDL_Event event) {
  // poll for events using SDL event calls
  if(SDL_PollEvent(&event))
    if(event.type == SDL_QUIT)
      return true;

  return false; // true if the user wants to quit the game
}

void GameLoop::update(game_state *, cpu_clock::ms elapsed_time) {
  // update game logic here
}

void GameLoop::draw(game_state const &, Graphics graphics) {
  // render stuff here
  graphics.clear();
  this->player.draw(graphics);
  graphics.render();
}

game_state interpolate(game_state const & current, game_state const & previous, float alpha) {
  game_state interpolated_state;

  // interpolate between previous and current by alpha here

  return interpolated_state;
}

float GameLoop::approach(float goal, float current, float dt){
  float diff = goal - current;
  if (diff > dt){
    return current + dt;
  }
  if (diff < -dt){
    return current - dt;
  }
  return goal;
}

void GameLoop::loop() {
  Timer fps;
  std::cerr<<"sanity check";
  //create graphics objects
  Graphics graphics;
  Input input;
  SDL_Event event;
  this->map = Map("Map 1",graphics);
  Vector2 spawnPoint = map.getPlayerSpawnPoint();
  this->player = Player(graphics, "sprites/playerspritesheett.png",1 ,1, spawnPoint.x-32, spawnPoint.y - 60, 64, 32);
  player.playAnimation("Idle");
  SDL_Rect camera;
  int pauseTime = 0;
  
  //graphics.setCamera(&player.camera.screen);
  
  cpu_clock clock;

  auto previous_time = clock.now();
  bool quit_game = false;

  game_state current_state;
  game_state previous_state;

  while(!quit_game) {
    fps.start();
    input.beginNewFrame();
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        if (event.key.repeat == 0) {
          input.keyDownEvent(event);
        }
      }
      else if (event.type == SDL_KEYUP) {
        input.keyUpEvent(event);
      }
    }
    
    Uint32 dt = clock.now() - previous_time;
    previous_time = clock.now();
    
    clock.lag += dt;

    quit_game = handle_events(event);
    
    float x = 0;
    float y = 0;
    bool crouching = false;
    
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    if (input.keyWasPressed(SDL_SCANCODE_A))
    {
      if (keystates[SDL_SCANCODE_DOWN]){
        player.dropDown();
      }
      else
        player.jump();
    }
    if (keystates[SDL_SCANCODE_DOWN])
    {
      crouching = true;
    }
    if (keystates[SDL_SCANCODE_LEFT])
    {
      x -= 1000.0;
    }
    if (keystates[SDL_SCANCODE_RIGHT])
    {
      x += 1000.0;
    }
    if (input.keyWasPressed(SDL_SCANCODE_D))
    {
      player.shoot();
    }
    if (input.keyWasPressed(SDL_SCANCODE_S))
    //if (keystates[SDL_SCANCODE_S])
    {
      player.attack();
    }
    if (input.keyWasPressed(SDL_SCANCODE_Q))
    {
      player.dodge();
    }
    
    //std::cout<<dt<<std::endl;
    //float nothing = approach(x,player._dx,dt*10);
    
    player.moveSprite(x,y);
    player.check_crouch(crouching);
   

    // try to maintain 60 fps
    while(clock.lag >= clock.timestep) {
      //dt = clock.toSeconds(clock.timestep);
      clock.lag -= clock.timestep;
      std::cout<<"STUCK"<<std::endl;
      
      if (player.hitRegistered())
        pauseTime = 7;
      
      if (player.hitRegistered() || pauseTime > 0){
        pauseTime -= (1);
      }
      else{
        player.update(0.0071667);
        player.applyGravity(0.0071667);
        player.handleCollisions(map);
        player.setCamera(&camera,map);
      }
      
      map.update(0.0071667, player);
      player.updateAttachments(0.0071667);
      

      //previous_state = current_state;
      //update(&current_state, clock.timestep); // update at a fixed rate each time
      
    }

    // calculate how close or far we are from the next timestep
    //auto alpha = (float) clock.lag.count() / clock.timestep.count();
    //auto interpolated_state = interpolate(current_state, previous_state, alpha);

    graphics.clear();
    this->map.draw(graphics);
    this->player.draw(graphics);
    graphics.setCamera(&camera);
    graphics.render();
    
    //Cap the frame rate
    if( fps.get_ticks() < clock.timestep )
    {
      SDL_Delay( ( clock.timestep ) - fps.get_ticks() );
      std::cout<<"ITGETSHERE"<<std::endl;
    }
    

    //draw(interpolated_state, graphics);
  }
  printf("Done\n");
}
