#include <chrono>
#include <SDL2/SDL.h>

#include "conf.h"
#include "graphics.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "gamestate.h"

#ifndef GAMELOOP_H
#define GAMELOOP_H


using namespace std::literals::chrono_literals;

class Timer
{
private:
  //The clock time when the timer started
  int startTicks;
  
  //The ticks stored when the timer was paused
  int pausedTicks;
  
  //The timer status
  bool paused;
  bool started;
  
public:
  //Initializes variables
  Timer();
  
  //The various clock actions
  void start();
  void stop();
  void pause();
  void unpause();
  
  //Gets the timer's time
  int get_ticks();
  
  //Checks the status of the timer
  bool is_started();
  bool is_paused();
};


// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
//constexpr std::chrono::nanoseconds timestep(16ms);

struct cpu_clock {

  typedef std::chrono::milliseconds ms;
  typedef std::chrono::nanoseconds ns;

  //ms timestep = 16ms;
  Uint32 lag = 0;
  
  int fps = 60;
  
  int timestep = 1000/fps;
  
  float frametime = 0.0075*(30.0/(float)fps);

  using rep = std::clock_t;
  using period = std::ratio<1, CLOCKS_PER_SEC>;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<cpu_clock, duration>;
  static const bool is_steady = false;

  static Uint32 now() noexcept {
    //return time_point{duration{std::clock()}};
    return SDL_GetTicks();
  }

  static std::chrono::nanoseconds toNano(duration t) noexcept {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t);
  }
  
  static float toSeconds(duration t) noexcept {
    return std::chrono::duration_cast<std::chrono::duration<float>>(t).count();
  }
  
  void setFPS(){
    SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0};
    SDL_GetDisplayMode(0,0,&mode);
    fps = mode.refresh_rate;
  }

};


class Graphics;

class GameLoop{
public:
	GameLoop();
	~GameLoop();
private:
	bool handle_events(SDL_Event event);
	void update(GameState* gamestate, float dt, Graphics *graphics);
	void draw(GameState* gamestate, Graphics *graphics);
  void processInputs(GameState* gamestate, Input &input);
	void loop();
  float approach(float goal, float current, float dt);
  GameState* currState;
  
};

#endif
