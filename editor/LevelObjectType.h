#ifndef BLUESJACKRABBIT_EDITOR_LEVELOBJECTTYPE_H
#define BLUESJACKRABBIT_EDITOR_LEVELOBJECTTYPE_H

typedef enum LevelObjectType_ {
  /* Generic objects */
  GENERIC_IMAGE,
  BREAKABLE_RECT,
  RECTANGLE,
  CIRCLE,
  /* Control objects */
  START_POINT,
  SPAWN_POINT,
  GOAL
} LevelObjectType;

#endif // BLUESJACKRABBIT_EDITOR_LEVELOBJECTTYPE_H