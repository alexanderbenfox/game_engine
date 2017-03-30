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

void GameLoop::update(GameState * gamestate, float dt, Graphics *graphics) {
  // update game logic here
  gamestate->update(dt, graphics);
}

void GameLoop::draw(GameState* gamestate, Graphics *graphics) {
  // render stuff here
  graphics->clear();
  gamestate->draw(graphics);
  graphics->render();
}

void GameLoop::processInputs(GameState *gamestate, Input &input){
  gamestate->processInput(input);
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
  StartMenuState startMenuState = StartMenuState();
  //currState = &playState;
  StateTransition::getInstance()->setCurrentGameState(start_menu);
  currState = &startMenuState;
  SDL_Event event;
  
  cpu_clock clock;
  clock.setFPS();
  
  PlayState playState = PlayState(&graphics, &clock);
  
  float frames = 0;
  float startTime = SDL_GetTicks();

  auto previous_time = clock.now();
  bool quit_game = false;

  while(!quit_game) {
    if(StateTransition::getInstance()->getCurrentGameState() == game_play){
      currState = &playState;
    }
    
    if(StateTransition::getInstance()->getCurrentGameState() == game_play_no_tut){
      playState.setMap("Map 1-1", &graphics);
      currState = &playState;
      StateTransition::getInstance()->setCurrentGameState(game_play);
    }
    
    
    if(frames == 0){
      startTime = SDL_GetTicks();
    }
    frames++;
    fps.start();
    input.beginNewFrame();
    
    Uint32 dt = clock.now() - previous_time;
    previous_time = clock.now();
    
    clock.lag += dt;

    quit_game = handle_events(event);
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    input.updateKeys(keystates);
    
    processInputs(currState, input);

    // try to maintain 60 fps
    while(clock.lag >= clock.timestep) {
      clock.lag -= clock.timestep;
      update(currState, clock.frametime, &graphics);
    }
    
    draw(currState, &graphics);
    
    //Cap the frame rate
    if( fps.get_ticks() < clock.timestep)
    {
      SDL_Delay( ( clock.timestep ) - fps.get_ticks() );
    }
    
    if(frames == 1){
      float f = (1000*frames)/((SDL_GetTicks()-startTime));
      std::cout<<"FPS:"<<f<<std::endl;
      frames = 0;
    }
  }
  printf("Done\n");
}
