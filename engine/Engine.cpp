#include <glibmm/main.h>
#include <vector>
#include "Engine.h"

Engine::Engine() {
#if __cplusplus > 199711L
  game_objects_.emplace_back();
#else
  // this shows a warning in C++11 because of move semantics
  game_objects_.push_back(GameObject());
#endif
  Glib::signal_timeout().connect(
      sigc::bind_return(sigc::mem_fun(*this, &Engine::FixedUpdate), true),
      fixedUpdateStep);
}

std::vector<GameObject> &Engine::game_objects() {
  return game_objects_;
}

void Engine::FixedUpdate() {
//    double step = static_cast<double>(event.TimeDelta()) / 1000000000;
  GameObject &game_object = game_objects_.front();
  game_object.rigid_body().update_fixed();
  game_object.update_fixed();
}

void Engine::apply_force(GameObject *game_object, Vector force) {
  game_object->rigid_body().apply_force(force);
}