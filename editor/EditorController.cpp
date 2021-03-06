#include <gdk/gdk.h>
#include "EditorWindow.h"
#include "EditorLayer.h"
#include "LevelLoader.h"
#include "LevelObject.h"
#include "LevelObjectType.h"
#include "LevelWriter.h"
#include "EditorController.h"
#include "Map.h"

#include <iostream>

EditorController::EditorController()
    : map_(new Map()), level_(new Level("blsbls", 1000, 1000 , 2)),
    level_file_name_("level1.xml"), unsaved_changes_(false) {
}

void EditorController::start_new_level() {
  if (level_) {
    map_->add_level(level_);
    level_ = new Level("Mapa de ejemplo", 1000, 1000 , 2);
  }
}

void EditorController::start_new_level(const Glib::VariantBase& /* parameter */) {
  start_new_level();
}

void EditorController::load_level() {
  if (unsaved_changes_) {
  // TODO(Diego): mostrar cuadro de dialogo - guardar cambios?
  }
  // if (level_) {
  //  delete level_;
  // }
  LevelLoader loader;
  level_ = loader.load_level(level_file_name_);
}

void EditorController::save_file(const std::string& file_name) {
  LevelWriter writer(*level_);
  writer.write(file_name);
}

void EditorController::save_file(const Glib::VariantBase& /* parameter */) {
  save_file(level_file_name_);
}

void EditorController::export_map(const std::string& file_name) {
  LevelWriter writer(*level_);
  writer.write_map(file_name, *map_);
}

void EditorController::export_map(const Glib::VariantBase&) {
  export_map("map.xml");
}

void EditorController::register_object(LevelObject* object) {
  obj_by_id_lookup_table_[object->representation()->item_id()] = object;
  switch (object->object_type()) {
  case SPAWN_POINT:
    level_->add_spawn_point(object);
    break;
  case START_POINT:
    level_->add_start_point(object);
    break;
  case GOAL:
    level_->add_goal(object);
    break;
  default:
    level_->add_generic_object(object);
    break;
  }
}

LevelObject* EditorController::get_registered_object(uint64_t object_id) {
  return obj_by_id_lookup_table_[object_id];
}

EditorController::~EditorController() {
  if (level_) {
    delete level_;
  }
  if (map_) {
    delete map_;
  }
}
