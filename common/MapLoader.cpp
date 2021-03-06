#include <map>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <engine/GameObjectFloor.h>
#include <engine/GameObjectGoal.h>
#include <engine/GameObjectGreenTurtle.h>
#include <engine/GameObjectRedTurtle.h>
#include <engine/RigidBody.h>
#include <engine/StaticBody.h>
#include <engine/PolygonCollider.h>
#include <engine/GameObjectWall.h>
#include <server/Map.h>
#include "MapLoader.h"
#include "Logger.h"
#define PIXELS_PER_METER 64.0

MapLoader::MapLoader(Engine *engine, const Map &map, WinnerNotifier winner_notifier)
    : engine_(engine)
    , winner_notifier_(winner_notifier)
    , even_(false)
    , startpoint_cursor_(0)
    , parser_(map.path())
    , players_size_(static_cast<char>(parser_.get_document()->get_root_node()->eval_to_number("@players_size")))
    , levels_(parser_.get_document()->get_root_node()->get_children("level"))
    , level_(levels_.begin()) {
}

MapLoader::~MapLoader() {
}

void MapLoader::load() {
  load_level();
}

void MapLoader::load_level() {
  Logger::info("Cargando nivel");

  xmlpp::Node *visible_layer = (*level_)->get_first_child("visible-object-layer");
  const xmlpp::Node::NodeList &rectangles = visible_layer->get_children("rectangle");
  for (std::list<xmlpp::Node *>::const_iterator it = rectangles.begin();
       it != rectangles.end(); ++it) {
    // Lo agrega al engine
    add_floor(*it);
  }

  xmlpp::Node *control_layer = (*level_)->get_first_child("control-object-layer");
  const xmlpp::Node::NodeList &startpoints = control_layer->get_children("startpoint");
  clean_start_points();
  for (std::list<xmlpp::Node *>::const_iterator it = startpoints.begin();
       it != startpoints.end(); ++it) {
    // LO AGREGA EN EL MAP LOADER!!! SE NECESITA LIMPIAR ESTO!!!
    add_startpoint(*it);
  }

  const xmlpp::Node::NodeList &spawnpoints = control_layer->get_children("spawnpoint");
  for (std::list<xmlpp::Node *>::const_iterator it = spawnpoints.begin();
       it != spawnpoints.end(); ++it) {
    // Lo agrega al engine
    add_spawnpoint(*it);
  }

  const xmlpp::Node::NodeList &goals = control_layer->get_children("goal");
  for (std::list<xmlpp::Node *>::const_iterator it = goals.begin();
       it != goals.end(); ++it) {
    // Lo agrega al engine
    add_goal(*it);
  }
}

void MapLoader::place_player(ClientProxy *player) {
  Vector *pos = new Vector(*player_start_point());
  RigidBody *body = new RigidBody(pos);
  GameObjectPlayer *object = new GameObjectPlayer(body, new CircleCollider(body, 0.5));
  uint32_t object_id = engine_->add_game_object_player(object);
  player->add_object_id(object_id, object);
  players_[object_id] = player;
}


Vector *MapLoader::player_start_point() {
  return start_points_[(startpoint_cursor_++) % start_points_.size()];
}

void MapLoader::add_floor(xmlpp::Node *const &node) {
  std::vector<Vector> floor_points;
  double x = to_game_coordinates(node->eval_to_number("@x"));
  double y = to_game_coordinates(node->eval_to_number("@y"));
  double width = to_game_coordinates(node->eval_to_number("@width"));
  double height = to_game_coordinates(node->eval_to_number("@height"));
  bool breakable = node->eval_to_string("@breakable") == "true";
  floor_points.push_back(Vector(x, y));
  floor_points.push_back(Vector(x + width, y));
  floor_points.push_back(Vector(x + width, y - height));
  floor_points.push_back(Vector(x, y - height));
  StaticBody *body = new StaticBody(new Vector(x + width / 2, y - height / 2));
  GameObjectFloor *floor = new GameObjectFloor(body, new PolygonCollider(*body, floor_points), breakable);
  engine_->add_game_object(floor);
}

void MapLoader::clean_start_points() {
  Logger::info("Limpiando start points");
  for (std::vector<Vector*>::iterator it = start_points_.begin(); it != start_points_.end(); ++it) {
    delete *it;
  }
  start_points_.clear();
  startpoint_cursor_ = 0;
}

void MapLoader::add_startpoint(xmlpp::Node *const &node) {
  double x = to_game_coordinates(node->eval_to_number("@x"));
  double y = to_game_coordinates(node->eval_to_number("@y"));
  std::stringstream ss;
  ss << "Start point: ";
  ss << "(" << x << ", " << y << ")";
  Logger::info(ss.str());
  start_points_.push_back(new Vector(x, y));
}

void MapLoader::add_spawnpoint(xmlpp::Node *const &node) {
  double x = to_game_coordinates(node->eval_to_number("@x"));
  double y = to_game_coordinates(node->eval_to_number("@y"));
  RigidBody *r_body = new RigidBody(new Vector(x, y));
  GameObject *turtle;
  if (g_random_boolean()) {
    turtle = new GameObjectGreenTurtle(r_body, new CircleCollider(r_body, 0.5));
  } else {
    turtle = new GameObjectRedTurtle(r_body, new CircleCollider(r_body, 0.5));
  }
  engine_->add_game_object(turtle);
}


void MapLoader::reposition_players() {
  even_ = false;
  for (std::map<uint32_t, ClientProxy*>::iterator it= players_.begin();
      it != players_.end();
      it++) {
    Vector *vec = player_start_point();
    std::stringstream ss;
    ss << "Reposicionando jugador a coordenadas: ";
    ss << "(" << vec->x() << ", " << vec->y() << ")";
    Logger::info(ss.str());
    engine_->move_game_object_player(it->first, *vec);
  }
}

char MapLoader::needed_players() {
  return players_size_;
}

bool MapLoader::load_next_level() {
  ++level_;
  if (level_ != levels_.end()) {
    reload_level();
    return true;
  }

  return false;
}

void MapLoader::reload_level() {
  startpoint_cursor_ = 0;
  engine_->clean_objects();
  load_level();
  reposition_players();
}

void MapLoader::add_goal(xmlpp::Node *const &node) {
  std::vector<Vector> goal_points;
  double x = to_game_coordinates(node->eval_to_number("@x"));
  double y = to_game_coordinates(node->eval_to_number("@y"));
  double width = 1;
  double height = 1;

  goal_points.push_back(Vector(x, y));
  goal_points.push_back(Vector(x + width, y));
  goal_points.push_back(Vector(x + width / 2, y - height));

  StaticBody *goal_body = new StaticBody(new Vector(x + width / 2, y - height / 2));
  GameObjectGoal *goal = new GameObjectGoal(goal_body,
                                            new PolygonCollider(*goal_body, goal_points),
                                            winner_notifier_);
  engine_->add_game_object(goal);
}

double MapLoader::to_game_coordinates(double val) {
  return val / PIXELS_PER_METER;
}
