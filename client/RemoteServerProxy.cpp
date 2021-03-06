#include <glibmm/main.h>
#include <vector>
#include <stdlib.h>

#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include <common/Lock.h>
#include <common/MessageReader.h>
#include <common/MapsMessage.h>
#include <common/CreateGameMessage.h>
#include <common/MessageWriter.h>

#include "RemoteServerProxy.h"

#include "BulletRenderer.h"
#include "FloorRenderer.h"
#include "MeCharacterRenderer.h"
#include "NewLifeRenderer.h"
#include <common/Constants.h>
#include <common/Logger.h>
#include <common/GameInitMessage.h>
#include <common/GameObjectMessage.h>
#include "OtherCharacterRenderer.h"
#include "TurtleRenderer.h"
#include "GreenTurtleRenderer.h"
#include "RedTurtleRenderer.h"




void RemoteServerProxy::MoveUp() {
  Lock l(&mutex_);
  char move = UP;
  socket_->send_buffer(&move, 1);
}

void RemoteServerProxy::MoveDown() {
  Lock l(&mutex_);
  char move = DOWN;
  socket_->send_buffer(&move, 1);
}

void RemoteServerProxy::MoveLeft() {
  Lock l(&mutex_);
  char move = LEFT;
  socket_->send_buffer(&move, 1);
}

void RemoteServerProxy::MoveRight() {
  Lock l(&mutex_);
  char move = RIGHT;
  socket_->send_buffer(&move, 1);
}

void RemoteServerProxy::jump() {
  Lock l(&mutex_);
  char move = JUMP;
  socket_->send_buffer(&move, 1);
}

void RemoteServerProxy::shoot() {
  Lock l(&mutex_);
  char move = SHOOT;
  socket_->send_buffer(&move, 1);
}

RemoteServerProxy::RemoteServerProxy(const Configuration &config, FinishGame finish, Notifier notifier)
    : config_(config)
    , socket_(NULL)
    , updater_(new RemoteServerProxyUpdater(sigc::mem_fun(*this, &RemoteServerProxy::update_lives),
               sigc::mem_fun(*this, &RemoteServerProxy::update_object),
               sigc::mem_fun(*this, &RemoteServerProxy::clean_renderers),
               sigc::mem_fun(*this, &RemoteServerProxy::create_object_renderer),
               finish,
               notifier))
    , object_id_(0)
    , alive_(true)
    , notify_dead_(true)
    , finish_functor_(finish)
    , notifier_(notifier) {
}

void RemoteServerProxy::clean_renderers() {
  for (std::map<uint32_t, Renderer *>::iterator game_object = renderers_.begin();
       game_object != renderers_.end();
       ++game_object) {
    delete game_object->second;
  }
  renderers_.clear();
  Logger::info("RemoteServerProxy::clean_renderers limpio");
}

RemoteServerProxy::~RemoteServerProxy() {
  for (std::map<uint32_t, Renderer *>::iterator game_object = renderers_.begin();
       game_object != renderers_.end();
       ++game_object) {
    delete game_object->second;
  }
  socket_->close_connection();
  updater_->join();
  delete updater_;
  delete socket_;
}

void RemoteServerProxy::reset_updater() {
  Logger::info("Nuevo updater");
  updater_->join();
  delete updater_;
  updater_ = new RemoteServerProxyUpdater(sigc::mem_fun(*this, &RemoteServerProxy::update_lives),
               sigc::mem_fun(*this, &RemoteServerProxy::update_object),
               sigc::mem_fun(*this, &RemoteServerProxy::clean_renderers),
               sigc::mem_fun(*this, &RemoteServerProxy::create_object_renderer),
               finish_functor_,
               notifier_);
  updater_->set_socket(socket_);
  clean_renderers();
}

Vector RemoteServerProxy::character_position() {
  if (renderers_.find(object_id_) != renderers_.end()) {
    return renderers_[object_id_]->position();
  } else {
    return Vector(0, 0);
  }
}

LivesRenderer &RemoteServerProxy::lives_renderer() {
  return lives_renderer_;
}

std::map<uint32_t, Renderer *> &RemoteServerProxy::renderers() {
  return renderers_;
}

void RemoteServerProxy::connect() {
  socket_ = new Socket(config_["server_host"], config_["server_port"], 0);
  updater_->set_socket(socket_);
  socket_->connect_socket();
  MessageReader reader(socket_);
  reader.read_player_id();
  const std::string &peer_name_ = socket_->peer_name();
  Logger::info(std::string("Server ").append(peer_name_).append(" conectado"));
}

bool RemoteServerProxy::start_game(size_t map_id, std::string game_name, int players_size) {
  MessageWriter writer(socket_);
  writer.send_create_game(map_id, game_name, players_size);

  read_object_id(&object_id_);
  init_game();
  updater_->start();

  return true;
}

void RemoteServerProxy::join_game(size_t game_id) {
  MessageWriter writer(socket_);
  writer.send_join_game(game_id);

  read_object_id(&object_id_);
  init_game();
  updater_->start();
}

void RemoteServerProxy::read_object_id(uint32_t *object_id) {
  uint32_t read;
  char *buffer = static_cast<char *>(static_cast<void *>(&read));
  socket_->read_buffer(buffer, UINT32_T_LENGTH);
  *object_id = ntohl(read);
}

void RemoteServerProxy::init_game() {
  Logger::info("RemoteServerProxy::init_game");
  MessageReader reader(socket_);
  GameInitMessage message = reader.read_game_init();
  message.read();
  update_lives(message.info().remaining_lives());
  for (std::vector<GameObjectMessage *>::const_iterator i = message.objects().begin();
       i != message.objects().end(); i++) {
    create_object_renderer((*i)->object_id(), (*i)->object_type(), (*i)->position(), (*i)->points());
  }
}

void RemoteServerProxy::create_object_renderer(uint32_t object_id, char object_type, const Vector &position,
                                               std::vector<Vector> points) {
  // As this function is called in each new level, I have to update notify dead variable
  // although it will be set to true as many objects are transmitted by the server.
  Renderer *render = NULL;
  switch (object_type) {
    case 'p':
      if (object_id == object_id_) {
        render = new MeCharacterRenderer(position, points.front().x());
      } else {
        render = new OtherCharacterRenderer(position, points.front().x());
      }
      break;
    case 't':
      render = new GreenTurtleRenderer(position, points.front().x());
      break;
    case 'r':
      render = new RedTurtleRenderer(position, points.front().x());
      break;
    case 'f':
    case 'w':
      render = new FloorRenderer(position, points);
      break;
    case 'o':
      render = new FloorRenderer(position, points);
      break;
    case 'l':
      render = new NewLifeRenderer(position, points.front().x());
      break;
  }
  renderers_[object_id] = render;
}

void RemoteServerProxy::shutdown() {
  Logger::info("Shutdown de updater");
  MessageWriter writer(socket_);
  writer.send_disconnect();
  updater_->shutdown();
  updater_->join();
}

void RemoteServerProxy::update_lives(char remaining_lives) {
  lives_renderer_.set(remaining_lives);
  if (!notify_dead_ && remaining_lives != 0) {
    notify_dead_ = true;
  }
}

void RemoteServerProxy::update_object(uint32_t object_id, double x, double y, char type, point_type points,
  bool alive) {
  if (alive) {
    if (renderers_.find(object_id) != renderers_.end()) {
      renderers_[object_id]->update_position(Vector(x, y));
    } else if (type == 'b') {
        renderers_[object_id] = new BulletRenderer(Vector(x, y), points.front().x());
    } else if (type == 'l') {
      renderers_[object_id] = new NewLifeRenderer(Vector(x, y), points.front().x());
    }
  } else if (object_id != object_id_) {
    if (renderers_.find(object_id) != renderers_.end()) {
      Renderer *render = renderers_[object_id];
      renderers_.erase(object_id);
      delete render;
    }
  } else {
    if (notify_dead_) {
      Logger::info("Perdio todas las vidas");
      notifier_("PERDISTE TODAS LAS VIDAS");
      notify_dead_ = false;
    }
  }
}

std::map<size_t, std::string> RemoteServerProxy::list_maps() {
  char option = LIST_MAPS;
  socket_->send_buffer(&option, 1);
  MessageReader reader(socket_);
  MapsMessage maps = reader.read_available_maps();
  return maps.read();
}

std::map<size_t, std::string> RemoteServerProxy::list_games() {
  char option = LIST_GAMES;
  socket_->send_buffer(&option, 1);
  MessageReader reader(socket_);
  GamesMessage games = reader.read_available_games();
  return games.read();
}

