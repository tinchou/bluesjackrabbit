#ifndef BLUESJACKRABBIT_EDITOR_LEVELWRITER_H
#define BLUESJACKRABBIT_EDITOR_LEVELWRITER_H
#include <string>
#include <libxml++/libxml++.h>
#include "Level.h"
#include "Map.h"

class LevelWriter {
 public:
  LevelWriter(const Level& level);
  void write(const std::string& file_name);
  void write_map(const std::string& file_name, const Map& map);
  void append_level_to_node(xmlpp::Element* node_root);
 private:
  void add_visible_child_nodes(xmlpp::Element* root_element);
  void add_control_child_nodes(xmlpp::Element* root_element);
  void add_control_child_nodes_by_type(xmlpp::Element* root_element,
      std::string elem_type, std::vector<LevelObject*> objects);
  void add_metadata_to_node(xmlpp::Element* root_element, LevelObject* obj);
  template <class T> std::string to_string(const T& val) {
    std::stringstream ss;
    std::string to_convert;

    ss << val;
    ss >> to_convert;

    return to_convert;
  }

  Level level_;
};

#endif // BLUESJACKRABBIT_EDITOR_LEVELWRITER_H