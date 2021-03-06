#ifndef BLUESJACKRABBIT_COMMON_CREATEGAMEMESSAGE_H
#define BLUESJACKRABBIT_COMMON_CREATEGAMEMESSAGE_H

#include <string>
#include "Socket.h"
#include "Message.h"

#define MAX_NAME_LENGTH 256


class CreateGameMessage: public Message {
 public:
  explicit CreateGameMessage(Socket *socket);

  static char type_id();

  virtual char type();

  /**
   * Sends a request to create a new game in the server, which opens a lobby.
   * Players will wait until the game has enough players to start.
   */
  void send(size_t map_id, const std::string &game_name, int players_size);

  /**
   * Reads a request to create a new game.
   */
  void read();

  /**
   * Returns this message's map id. Should be called after read.
   */
  char map_id();

  /**
   * Returns this message's game name. Should be called after read.
   */
  std::string game_name();

  char player_size();

 private:
  Socket *socket_;
  char map_id_;
  char game_name_[MAX_NAME_LENGTH];
  char players_size_;
};


#endif  // BLUESJACKRAB_COMMONBIT_CREATEGAMEMESSAGE_H
