#ifndef BLUESJACKRABBIT_CLIENT_SERVERPROXY_H
#define BLUESJACKRABBIT_CLIENT_SERVERPROXY_H

#include <sigc++/functors/slot.h>
#include <engine/GameObject.h>
#include <engine/Engine.h>
#include <map>
#include <string>
#include <vector>
#include "Renderer.h"
#include "LivesRenderer.h"

/**
 * A functor object complying to void functor().
 */
typedef sigc::slot<void> Subscriber;

class ServerProxy {
 public:
  virtual ~ServerProxy() {}
  virtual void MoveUp() = 0;
  virtual void MoveDown() = 0;
  virtual void MoveLeft() = 0;
  virtual void MoveRight() = 0;
  virtual void jump() = 0;
  virtual void shoot() = 0;
  virtual Vector character_position() = 0;
  virtual LivesRenderer &lives_renderer() = 0;
  virtual std::map<uint32_t, Renderer*> &renderers() = 0;

  virtual void connect() = 0;
  virtual std::map<size_t, std::string> list_maps() = 0;
  virtual std::map<size_t, std::string> list_games() = 0;
  virtual bool start_game(size_t map_id, std::string, int players_size) = 0;
  virtual void join_game(size_t game_id) = 0;
  virtual void init_game() = 0;
  virtual void shutdown() = 0;
  virtual void reset_updater() = 0;
};


#endif  // BLUESJACKRABBIT_CLIENT_SERVERPROXY_H
