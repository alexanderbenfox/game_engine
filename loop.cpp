#include "loop.h"
#include <iostream>

GameLoop::GameLoop()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL error %s\n", SDL_GetError());
  }
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
  this->player.draw(graphics, 100, 100);
  graphics.render();
}

game_state interpolate(game_state const & current, game_state const & previous, float alpha) {
  game_state interpolated_state;

  // interpolate between previous and current by alpha here

  return interpolated_state;
}

void GameLoop::loop() {
  std::cerr<<"sanity check";
  //create graphics objects
  Graphics graphics;
  SDL_Event event; 
  this->player = Sprite(graphics, "sprites/01.png", 1,1, 100, 100, 31, 63);
  cpu_clock clock;

  auto previous_time = clock.now();
  bool quit_game = false;

  game_state current_state;
  game_state previous_state;

  while(!quit_game) {
    auto delta_time = clock.now() - previous_time;
    previous_time = clock.now();
    clock.lag += clock.toNano(delta_time);

    quit_game = handle_events(event);

    // update game logic as lag permits
    while(clock.lag >= clock.timestep) {
      clock.lag -= clock.timestep;

      previous_state = current_state;
      update(&current_state, clock.timestep); // update at a fixed rate each time
    }

    // calculate how close or far we are from the next timestep
    auto alpha = (float) clock.lag.count() / clock.timestep.count();
    auto interpolated_state = interpolate(current_state, previous_state, alpha);

    graphics.clear();
    this->player.draw(graphics, 100, 100);
    graphics.render();

    //draw(interpolated_state, graphics);
  }
  printf("Done\n");
}
