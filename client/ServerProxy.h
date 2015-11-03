#ifndef BLUESJACKRABBIT_CLIENT_SERVERPROXY_H
#define BLUESJACKRABBIT_CLIENT_SERVERPROXY_H


#include <sigc++/functors/slot.h>
#include <engine/Engine.h>
#include <engine/GameObject.h>
#include <map>
#include <string>
#include "GameObjectProxy.h"

/**
 * A functor object complying to void functor().
 */
typedef sigc::slot<void> Subscriber;

class ServerProxy {
 public:
  ServerProxy();
  void MoveUp();
  void MoveDown();
  void MoveLeft();
  void MoveRight();
  std::vector<Renderer> &renderers();

 private:
  static const double step;
  Engine engine_;
  std::vector<Renderer> renderers_;
  std::vector<Subscriber> subscribers;
};


#endif  // BLUESJACKRABBIT_CLIENT_SERVERPROXY_H