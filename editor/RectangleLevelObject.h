#ifndef BLUESJACKRABBIT_EDITOR_RECTANGLELEVELOBJECT_H
#define BLUESJACKRABBIT_EDITOR_RECTANGLELEVELOBJECT_H
#include <goocanvasmm/item.h>
#include "LevelObject.h"
#include "LevelObjectType.h"

class RectangleLevelObject : public LevelObject {
 public:
  RectangleLevelObject(double x, double y, int width, int height,
      const CanvasItem& representation, bool breakable);

  LevelObjectType object_type() const;

  void set_rotation_angle(double angle);
  double rotation_angle() const;
  void set_width(int width);
  int width() const;
  void set_height(int height);
  int height() const;
  bool breakable() const;

 private:
  double rotation_angle_;
  int width_;
  int height_;
  bool breakable_;
};

#endif // BLUESJACKRABBIT_EDITOR_RECTANGLELEVELOBJECT_H