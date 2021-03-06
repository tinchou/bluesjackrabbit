#ifndef BLUESJACKRABBIT_SERVER_GAME_H
#define BLUESJACKRABBIT_SERVER_GAME_H

#include <map>
#include <string>
#include <common/Mutex.h>
#include <engine/Engine.h>
#include <common/MapLoader.h>

#include "ClientProxy.h"
#include "GameRunner.h"
#include "Map.h"

class Game {
 public:
  Game(const Configuration &config, const Map &map, ClientProxy *admin, const std::string &game_name, char player_size);
  virtual ~Game();
  void add_player(ClientProxy *player);
  bool can_join();
  void action(uint32_t object_id, char option);
  void jump(uint32_t object_id);
  void shoot(uint32_t object_id);
  std::string name();

  void finalize();

 private:
  Engine engine_;
  Mutex engine_mutex_;
  std::map<char, ClientProxy*> players_;
  GameRunner runner_;
  MapLoader map_loader_;
  char player_index_;
  bool in_game;
  std::string game_name_;
  char player_size_;

  void start_game();
  bool load_next_level(bool there_was_winner);
  void reset_players_lives();
};

#endif /* BLUESJACKRABBIT_SERVER_GAME_H */
