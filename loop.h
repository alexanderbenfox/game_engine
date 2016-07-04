#include <chrono>
#include <SDL2/SDL.h>

#include "conf.h"
#include "graphics.h"
#include "sprite.h"
#include "input.h"

#ifndef GAMELOOP_H
#define GAMELOOP_H


using namespace std::literals::chrono_literals;


// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
//constexpr std::chrono::nanoseconds timestep(16ms);

struct game_state {
  // this contains the state of your game, such as positions and velocities
};

struct cpu_clock {

  typedef std::chrono::milliseconds ms;
  typedef std::chrono::nanoseconds ns;

  ms timestep = 16ms;
  ns lag = 0ns;

  using rep = std::clock_t;
  using period = std::ratio<1, CLOCKS_PER_SEC>;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<cpu_clock, duration>;
  static const bool is_steady = false;

  static time_point now() noexcept {
    return time_point{duration{std::clock()}};
  }

  static std::chrono::nanoseconds toNano(duration t) noexcept {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t);
  }

};


class Graphics;

class GameLoop{
public:
	GameLoop();
	~GameLoop();
private:
	bool handle_events(SDL_Event event);
	void update(game_state *, cpu_clock::ms elapsed_time);;
	void draw(game_state const &, Graphics graphics);
	void loop();
  Sprite player; 
};

#endif
