#include <chrono>
#include "conf.h"
using namespace std::chrono_literals;


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

bool handle_events() {
  // poll for events

  return false; // true if the user wants to quit the game
}

void update(game_state *) {
  // update game logic here
}

void render(game_state const &) {
  // render stuff here
}

game_state interpolate(game_state const & current, game_state const & previous, float alpha) {
  game_state interpolated_state;

  // interpolate between previous and current by alpha here

  return interpolated_state;
}

int main() {
  cpu_clock clock;

  auto previous_time = clock.now();
  bool quit_game = false;

  game_state current_state;
  game_state previous_state;

  while(!quit_game) {
    auto delta_time = clock.now() - previous_time;
    previous_time = clock.now();
    clock.lag += clock.toNano(delta_time);

    quit_game = handle_events();

    // update game logic as lag permits
    while(clock.lag >= clock.timestep) {
      clock.lag -= clock.timestep;

      previous_state = current_state;
      update(&current_state); // update at a fixed rate each time
    }

    // calculate how close or far we are from the next timestep
    auto alpha = (float) clock.lag.count() / clock.timestep.count();
    auto interpolated_state = interpolate(current_state, previous_state, alpha);

    render(interpolated_state);
  }
}