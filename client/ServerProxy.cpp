#include <engine/Engine.h>
#include <glibmm/main.h>
#include <vector>
#include <stdlib.h>


#include <iostream>
#include <unistd.h>

#include "ServerProxy.h"
#include "Lock.h"


const double ServerProxy::step = 0.003;

// Socket write IN GAME
void ServerProxy::MoveUp() {
  engine_.apply_force(&engine_.game_objects().front(), Vector(0, -step));
}

// Socket write IN GAME
void ServerProxy::MoveDown() {
  engine_.apply_force(&engine_.game_objects().front(), Vector(0, step));
}

#include <stdio.h>
// Socket write IN GAME
void ServerProxy::MoveLeft() {
  Lock l(&mutex);
  engine_.apply_force(&engine_.game_objects().front(), Vector(-step, 0));
  std::cout << "ENVIANDO IZQ CON 1 DE LENGHT\n";
  char *c = (char*) malloc(sizeof(char));
  *c = 'i';
  ssize_t *len = (ssize_t*) malloc(sizeof(ssize_t));
  *len = 1;
  std::cout << "DIR MEMORIA: ";
  printf("%p\n", (void*)c);
  socket->enviar(c, *len);
  free(c);
  free(len);
  std::cout << "FIN IZQ\n";
}

// Socket write IN GAME
void ServerProxy::MoveRight() {
  Lock l(&mutex);
  engine_.apply_force(&engine_.game_objects().front(), Vector(step, 0));
  std::cout << "ENVIANDO DER CON 1 DE LENGHT\n";
  char c = 'd';
  socket->enviar(&c, 1);
  std::cout << "FIN DER\n";
}

// Socket recibir. This should be done after start game (not in constructor)
ServerProxy::ServerProxy() {
  for (std::vector<GameObject>::iterator game_object = engine_.game_objects().begin();
       game_object != engine_.game_objects().end();
       ++game_object) {
    renderers_.push_back(Renderer(&(*game_object)));
  }
}

ServerProxy::~ServerProxy() {
  delete socket;
}

// Nothing, it will be updated from other place
std::vector<Renderer> &ServerProxy::renderers() {
  return renderers_;
}

// recibir and write.
bool ServerProxy::connect() {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;

  getaddrinfo("localhost", "socks", &hints, &address_info);
  socket = new Socket("localhost", "socks", 0);
  socket->conectar();

  char caa[10];
  caa[0] = 'h';
  caa[1] = 'a';
  caa[2] = 'b';
  socket->enviar(caa, 1);

  char message_size;
  socket->recibir(&message_size, 1);
  std::cout << "SIZE: " << message_size << "\n";

  char c;
  char *pt = (char*) &message_size;
  if (*pt == 1) {
    std::cout << "ME VA A MANDAR " << message_size << " BYTE\n";
    socket->recibir(&c, message_size);
  } else {
    std::cout << "NO DETECTO EL BYTE\n";
    socket->recibir(&c, 1);
  }
  std::cout << "RECIBIDO: " << c << "\n";
  bool asd = c == 'A';
  c = 'd';
  socket->enviar(&c, 1);
  return asd;
}

// recibir and write
std::map<size_t, std::string> ServerProxy::list_maps() {
  std::map<size_t, std::string> map;
  map[1] = "Mapa 1";
  map[2] = "Mapa 2";
  map[3] = "Mapa 3";
  return map;
}

// Write... and recibir only to check game started.
bool ServerProxy::start_game(size_t map_id) {
  std::cout << "Start game with map id: " << map_id << std::endl;
  return true;
}
