#ifndef BLUESJACKRABBIT_COMMON_MESSAGEREADER_H
#define BLUESJACKRABBIT_COMMON_MESSAGEREADER_H


#include "CreateGameMessage.h"
#include "GamesMessage.h"
#include "MapsMessage.h"
#include "Socket.h"
#include "GameInitMessage.h"
#include "JoinGameMessage.h"
#include "PlayerInfoMessage.h"

class MessageReader {
 public:
  explicit MessageReader(Socket *socket);

  /**
   * Reads a message and parses its contents.
   * @throws InvalidMessageException
   */
  Message * read_message();

  /**
   * Reads a handshake from the socket (char 'A').
   * TODO(tinchou): send and read a message with the player id.
   * @throws InvalidMessageException
   */
  void read_player_id();

  /**
   * Reads a available maps from the socket.
   * @throws InvalidMessageException
   */
  MapsMessage read_available_maps();

  /**
   * Reads a available games from the socket.
   * @throws InvalidMessageException
   */
  GamesMessage read_available_games();

  /**
   * Reads a request to create a new game.
   * @throws InvalidMessageException
   */
  CreateGameMessage read_create_game();

  /**
   * Reads a message with the initial game state, like the object types and positions.
   * @throws InvalidMessageException
   */
  GameInitMessage read_game_init();

  /**
   * Reads a request to join a game.
   * @throws InvalidMessageException
   */
  JoinGameMessage read_join_game();

  /**
   * Reads a game object.
   * @throws InvalidMessageException
   */
  GameObjectMessage *read_game_object();

  /**
   * Reads the player info, like the remaining lives.
   * @throws InvalidMessageException
   */
  PlayerInfoMessage *read_player_info();

  /**
   * Reads a disconnect signal from the client.
   * @throws InvalidMessageException
   */
  void read_disconnect();

 private:
  Socket *socket_;
  void validate_message_type(char expected) const;
  char read_message_type() const;
};


#endif  // BLUESJACKRABBIT_COMMON_MESSAGEREADER_H
